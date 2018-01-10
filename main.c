#include <stdio.h>
#include <stdlib.h>
#include <p30F4011.h>
#include "dspic-libs/keyboard.h"
#include "dspic-libs/LCDlib.h"
#include "dspic-libs/libCAN.h"
#include "dspic-libs/delay.h"
#include "dspic-libs/leds.h"
#include "dspic-libs/terminal.h"
#include "dspic-libs/timer.h"
#include "dspic-libs/adc.h"
#include "dspic-libs/uart/uart.h"


#define TRUE 1
#define FALSE 0
#define MAX_HEIGHT 50 // Altura maxima del ascensor
#define MIN_HEIGHT 0 // Altura minima del ascensor
#define FLOOR_QUEUE_SIZE 6 // Numero de plantas / Capacidad de la cola de plantas
#define ELECTRIC_POWER_TRESHOLD 220 // Valor limite de potencia electrica necesaria (igual o superior)

// ESTADOS DEL ASCENSOR
#define STOPPED 0
#define ASCENDING 1
#define DESCENDING 2

// ASCII
#define ESC_KEY 27

// IDENTIFICADORES DE PAQUETES
#define STOP_REQUEST_SID 3
#define MOVE_REQUEST_SID 4
#define ELECTRIC_POWER_STATUS_SID 5
#define MOVEMENT_STATUS_SID 6

// PAQUETES
struct MovementStatus {
    unsigned char height;
    unsigned char state;
};

struct MoveRequest {
    unsigned char goal_height;
};

struct ElectricPowerStatus {
    unsigned char electric_power;
};

/* FUNCIONES Y VARIABLES PARA CONTROL */
struct FloorQueue {
    unsigned short queue[FLOOR_QUEUE_SIZE];
    unsigned short len, tail, head;
};

struct FloorQueue fq;

/* Inicializa la cola de plantas vacia */
inline void floorQueueInit(void);

/* 
 *  Anade una planta a la cola.
 *  
 *  Si la planta ya se encuentra en la cola, no se vuelve a anadir.
 *  De esta forma, nunca habra problemas de cola llena.
 */
inline void floorQueuePut(unsigned short floor);

/* Devuelve 1 si la cola esta llena, 0 si no. */
inline char floorQueueIsEmpty(void);

/* Elimina la primera planta de la cola y la devuelve */
inline unsigned short floorQueuePop(void);

/* Devuelve 1 si la planta ya esta en la cola, 0 si no. */
inline unsigned char floorQueueContains(unsigned short floor);

/* Apaga todos los leds y vacia la cola */
inline void floorQueueReset(void);

inline void processStopRequest(void);
/* END PLANTA */


/* FUNCIONES Y VARIABLES PARA CONTROL */
unsigned char control_elevator_current_height;
unsigned char control_elevator_goal_height;
unsigned char control_current_electric_power;
unsigned char control_elevator_state;

// Con estas tres funciones se mantiene una relacion planta-altura
inline unsigned short getFloor(unsigned char height);
inline unsigned char getHeight(unsigned short floor);
inline unsigned char isFloor(unsigned char height);

// Encola la planta si aun no esta encolada y enciende su correspondiente LED
inline void enqueueFloor(unsigned short floor);
// Desencola una planta y la envia a Planta
inline void dequeueAndSendFloor(void);

// Envio y procesamiento de paquetes
inline void processMovementStatus(struct MovementStatus *ms);
inline void processElectricPowerStatus(struct ElectricPowerStatus *eps);
inline void sendMoveRequest(struct MoveRequest *mr);
inline void sendStopRequest(void);
inline void activateSosMode(void);

void main_control(void);
/* END CONTROL */

/* FUNCIONES Y VARIABLES PARA PLANTA */
unsigned char planta_elevator_current_height;
unsigned char planta_elevator_goal_height;
unsigned char planta_current_electric_power;
unsigned char stop_protocol_activated;
unsigned char timer_interrupts_counter;

inline void increaseHeight(void);
inline void decreaseHeight(void);
inline void printHeight(void);

// Envio y procesamiento de paquetes
inline void processMoveRequest(struct MoveRequest *mr);
inline void sendMovementStatus(struct MovementStatus *ms);
inline void sendElectricPowerStatus(struct ElectricPowerStatus *eps);

void main_planta(void);
/* END PLANTA */

int main(void) {
    keyboardInit();
    LCDInit();
    initCAN();
    initLeds();

    // Seleccion del rol de la placa
    LCDClear();
    LCDMoveHome();
    LCDPrint("0. Control");
    LCDMoveSecondLine();
    LCDPrint("1. Planta");

    short opt = -1;
    while(opt != 0 && opt != 1){
        opt = waitForKeyPress();
        LCDClear();
        LCDMoveHome();
        switch(opt){
            case 0:
                LCDPrint("Control");
                main_control();
                break;
            case 1:
                LCDPrint("Planta");
                main_planta();
                break;
        }
    }
    return 0;
}

void main_planta(void){
    timerInit();
    ADCInit();

    planta_elevator_current_height = MIN_HEIGHT;
    planta_elevator_goal_height = MIN_HEIGHT;
    stop_protocol_activated = FALSE;
    timer_interrupts_counter = 0;
    planta_current_electric_power = ELECTRIC_POWER_TRESHOLD;
    struct MovementStatus ms;
    ms.state = STOPPED;

    printHeight();

    // Configurar el timer para lanzar una interrupcion cada medio segundo
    timerConfig(61402, PRESCALING_FACTOR_256, 4);
    timerStart();

    while(1){
        // Planta busca mantener la altura del ascensor en el valor de $planta_elevator_goal_height
        while (planta_elevator_current_height != planta_elevator_goal_height){

            // Si se activa el protocolo STOP, la altura objetivo cambia a la actual
            if(stop_protocol_activated){
                planta_elevator_goal_height = planta_elevator_current_height;
            }

            if (planta_elevator_current_height < planta_elevator_goal_height){
                increaseHeight();
                ms.state = ASCENDING;
            } else if (planta_elevator_current_height > planta_elevator_goal_height) {
                decreaseHeight();
                ms.state = DESCENDING;
            }

            // Si ha llegado a la altura despues de incrementarla/decrementarla,
            // el estado es parado
            if(planta_elevator_current_height == planta_elevator_goal_height){
                ms.state = STOPPED;
            }

            // Envia un paquete MovementStatus a cada iteracion
            ms.height = planta_elevator_current_height;
            sendMovementStatus(&ms);
        }

        // Tiempo entre cambio de planta
        int i;
        for (i = 0; i < 30; i++) Delay15ms();
    }
}

void main_control(void){
    char str_buffer[20];
    
    control_elevator_current_height = 0;
    control_elevator_goal_height = 0;
    control_current_electric_power = ELECTRIC_POWER_TRESHOLD;
    control_elevator_state = STOPPED;

    uartConfig();
    floorQueueInit();

    while(1){
        LCDClear();
        LCDMoveHome();

        /* Si la potencia de Planta esta por debajo del limite,
         * se bloquea la actividad de Control hasta que vuelva
         * a estar igual o por encima */
        if(control_current_electric_power < ELECTRIC_POWER_TRESHOLD){
            sendStopRequest();
            LCDPrint("Potencia baja.");
            LCDMoveSecondLine();
            LCDPrint("Bloqueado.");
            while(control_current_electric_power < ELECTRIC_POWER_TRESHOLD);
        }

        unsigned short selected_floor = getPressedKey();
        switch(selected_floor){
            case 0:
                enqueueFloor(0);
                break;
            case 2:
                enqueueFloor(1);
                break;
            case 3:
                enqueueFloor(2);
                break;
            case 5:
                enqueueFloor(3);
                break;
            case 6:
                enqueueFloor(4);
                break;
            case 8:
                enqueueFloor(5);
                break;
            case 1:
                activateSosMode();
                break;
            case 4:
                sendStopRequest();
                break;
        }

        switch(control_elevator_state){
            case STOPPED:

                /* Si el ascensor esta detenido
                 y se encuentra a una altura que corresponde a una planta,
                 muestra la planta en la que se encuentra */
                if(isFloor(control_elevator_current_height)){
                    sprintf(str_buffer, "Planta %u.", getFloor(control_elevator_current_height));
                    LCDPrint(str_buffer);
                }

                /* Si el ascensor esta detenido
                 y la altura no se corresponde a una planta
                 significa que el ascensor esta parado entre dos pisos */
                else {
                    LCDPrint("Ascensor");
                    LCDMoveSecondLine();
                    LCDPrint("parado.");
                }

                /*
                 * Si el ascensor esta detenido,
                 * hay plantas en la cola
                 * y se encuentra en una planta o
                 * no se encuentra en una planta pero el ascensor
                 * se ha parado intencionadamente (o ambos),
                 * el protocolo stop se anula y se envia la siguiente
                 * planta de la cola
                 */
                char floor_reached = (control_elevator_current_height == control_elevator_goal_height);
                if((floor_reached || stop_protocol_activated) && !floorQueueIsEmpty()){
                    stop_protocol_activated = FALSE;
                    dequeueAndSendFloor();
                }
                break;

            case ASCENDING:
                LCDPrint("Ascendiendo.");
                break;
                
            case DESCENDING:
                LCDPrint("Descendiendo.");
                break;
        }

        // Delay para evitar solapamiento de teclas
        int i;
        for (i = 0; i < 20; i++) Delay15ms();
    }
}

inline void increaseHeight(void){
    planta_elevator_current_height += 1;

    // Delay para que el desplazamiento sea mas lento
    int i;
    for (i = 0; i < 10; i++) Delay15ms();
    printHeight();
}

inline void decreaseHeight(void){
    planta_elevator_current_height -= 1;

    // Delay para que el desplazamiento sea mas lento
    int i;
    for (i = 0; i < 10; i++) Delay15ms();
    printHeight();
}

inline void printHeight(void){
    static char buffer[20];
    sprintf(buffer, " %u", planta_elevator_current_height);
    LCDClear();
    LCDPrint(buffer);
}

inline void processMoveRequest(struct MoveRequest *mr){
    planta_elevator_goal_height = mr->goal_height;
    stop_protocol_activated = FALSE;
    setLed(0, LED_OFF);
}

inline void sendMovementStatus(struct MovementStatus *ms){
    unsigned char data[2];
    data[0] = ms->height;
    data[1] = ms->state;
    transmitCAN(MOVEMENT_STATUS_SID, data, 2);
}

inline unsigned short getFloor(unsigned char height){
    return (height % 10 == 0) ? height / 10 : -1;
}
inline unsigned char getHeight(unsigned short floor){
    return floor * 10;
}

inline unsigned char isFloor(unsigned char height){
    return height >= MIN_HEIGHT && height <= MAX_HEIGHT && (height % 10 == 0);
}

inline void enqueueFloor(unsigned short floor){
    if (floor == getFloor(control_elevator_current_height)){
        return; // Si se selecciona la misma planta donde se encuentra, se ignora
    }

    setLed(floor, LED_ON);
    floorQueuePut(floor);
}

inline void dequeueAndSendFloor(void){
    struct MoveRequest mr;
    unsigned char floor = floorQueuePop();
    mr.goal_height = getHeight(floor);
    control_elevator_goal_height = mr.goal_height;
    sendMoveRequest(&mr);
}

inline void floorQueueInit(){
    fq.head = 0;
    fq.tail = 0;
    fq.len = 0;
}

inline void floorQueuePut(unsigned short floor){
    if (floorQueueContains(floor) || fq.len == FLOOR_QUEUE_SIZE){
        return; // Evitar plantas duplicadas
    }

    fq.queue[fq.head] = floor;
    fq.head = (fq.head + 1) % FLOOR_QUEUE_SIZE;
    fq.len += 1;
}

inline unsigned short floorQueuePop(void){
    if (fq.len == 0){
        return 0;
    }
    
    unsigned short floor = fq.queue[fq.tail];
    fq.tail = (fq.tail + 1) % FLOOR_QUEUE_SIZE;
    fq.len -= 1;

    return floor;
}

inline unsigned char floorQueueContains(unsigned short floor){
    unsigned short i, idx;
    for (i = 0; i < fq.len; ++i) {
        idx = (fq.tail + i) % FLOOR_QUEUE_SIZE;
        if(fq.queue[idx] == floor) return TRUE;
    }
    return FALSE;
}

inline char floorQueueIsEmpty(void){
    return fq.len == 0;
}

inline void floorQueueReset(void){
    fq.len = 0;
    fq.head = 0;
    fq.tail = 0;

    int i;
    for (i = 0; i < 6; ++i) {
        setLed(i, LED_OFF);
    }
}

inline void sendMoveRequest(struct MoveRequest *mr){
    unsigned char data[1];
    data[0] = mr->goal_height;
    transmitCAN(MOVE_REQUEST_SID, data, 1);
}

inline void processMovementStatus(struct MovementStatus *ms){
    control_elevator_current_height = ms->height;
    control_elevator_state = ms->state;

    // Se apaga el LED correspondiente a la planta en la que esta parado
    if(ms->state == STOPPED && isFloor(control_elevator_current_height)){
        char current_floor = getFloor(control_elevator_current_height);
        setLed(current_floor, LED_OFF);
    }
}

inline void sendStopRequest(void){
    floorQueueReset();
    stop_protocol_activated = TRUE;
    transmitCAN(STOP_REQUEST_SID, NULL, 0);
}

inline void sendElectricPowerStatus(struct ElectricPowerStatus *eps){
    unsigned char data[1];
    data[0] = eps->electric_power;
    transmitCAN(ELECTRIC_POWER_STATUS_SID, data, 1);
}

inline void processStopRequest(void){
    stop_protocol_activated = TRUE;
    setLed(0, LED_ON);
}

inline void processElectricPowerStatus(struct ElectricPowerStatus *eps){
    control_current_electric_power = eps->electric_power;
}

inline void activateSosMode(void){
    sendStopRequest();
    
    // Imprimir en LCD
    LCDClear();
    LCDMoveHome();
    LCDPrint(" Modo S.O.S");
    LCDMoveSecondLine();
    LCDPrint("activado.");

    // Imprimir en Terminal
    char buffer[256];

    sprintf(buffer, "MODO S.O.S ACTIVADO"
            "\r\n-------------------\r\n"
            "Altura: %u\r\n"
            "Potencia electrica: %u\r\n"
            "\r\n"
            "Pulse un valor del 0 al 5 para seleccionar planta.\r\n"
            "Pulse ESC para desactivar modo S.O.S.\r\n",
            control_elevator_current_height,
            control_current_electric_power);

    uartImprimir(12); // Limpiamos la pantalla
    uartColocarCursor(0, 0);
    uartImprimirString(buffer);

    struct MoveRequest mr;
    char key = 0, prev_key = 0;
    // Pulling del teclado de Terminal
    while(1){
        prev_key = key;
        if(DataRdyUART1()) key = uartGetUltimaTecla();

        // Evitamos teclas duplicadas o valor nulo
        if(key == prev_key || !key) continue;

        switch(key){
            case '0':
                mr.goal_height = getHeight(0);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case '1':
                mr.goal_height = getHeight(1);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case '2':
                mr.goal_height = getHeight(2);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case '3':
                mr.goal_height = getHeight(3);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case '4':
                mr.goal_height = getHeight(4);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case '5':
                mr.goal_height = getHeight(5);
                sendMoveRequest(&mr);

                sprintf(buffer, "Desplazando a altura %u\r\n", mr.goal_height);
                uartImprimirString(buffer);
                break;
            case ESC_KEY:
                sprintf(buffer, "Desactivando modo S.O.S...\r\n");
                uartImprimirString(buffer);
                return;
        }
    }
}

void _ISR _C1Interrupt(void) {
    if (C1INTFbits.RX0IF == 1) {
        static unsigned int sid;
        static unsigned char data[9];
        static short dlc;

        sid = C1RX0SIDbits.SID;
        dlc = C1RX0DLCbits.DLC & 0x0F;

        switch (dlc) {
            case 8:
                data[7] = (C1RX0B4 & 0xFF00) >> 8;
            case 7:
                data[6] = C1RX0B4 & 0x00FF;
            case 6:
                data[5] = (C1RX0B3 & 0xFF00) >> 8;
            case 5:
                data[4] = C1RX0B3 & 0x00FF;
            case 4:
                data[3] = (C1RX0B2 & 0xFF00) >> 8;
            case 3:
                data[2] = C1RX0B2 & 0x00FF;
            case 2:
                data[1] = (C1RX0B1 & 0xFF00) >> 8;
            case 1:
                data[0] = C1RX0B1 & 0x00FF;
                break;
        }

        static struct MovementStatus ms;
        static struct MoveRequest mr;
        static struct ElectricPowerStatus eps;

        switch(sid){
            case MOVE_REQUEST_SID:
                mr.goal_height = data[0];
                processMoveRequest(&mr);
                break;

            case MOVEMENT_STATUS_SID:
                ms.height = data[0];
                ms.state = data[1];
                processMovementStatus(&ms);
                break;

            case STOP_REQUEST_SID:
                processStopRequest();
                break;

            case ELECTRIC_POWER_STATUS_SID:
                eps.electric_power = data[0];
                processElectricPowerStatus(&eps);
                break;
        }

        // Clear rx buffer 0
        C1RX0CONbits.RXFUL = 0;
        // Clear CAN reception buffer 0 interrupt
        C1INTFbits.RX0IF = 0;
    }
    // Clear CAN interrupt
    IFS1bits.C1IF = 0;
}

void _ISR _T1Interrupt(void){
    static struct ElectricPowerStatus eps;
    
    // Envia el paquete cada dos interrupciones del timer
    if(timer_interrupts_counter % 2) {
        // La potencia electrica tiene un valor de 0 a 255
        planta_current_electric_power = (unsigned char) (ADCGetValue() / 4);
        eps.electric_power = planta_current_electric_power;

        sendElectricPowerStatus(&eps);
    }

    ++timer_interrupts_counter;

    IFS0bits.T1IF = 0; // Limpiar flag de estado
}