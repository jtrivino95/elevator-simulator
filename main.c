/* 
 * File:   main.c
 * Author: jtm648
 *
 * Created on 12 de diciembre de 2017, 18:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30F4011.h>
#include "dspic-libs/keyboard.h"
#include "dspic-libs/LCDlib.h"
#include "dspic-libs/libCAN.h"
#include "dspic-libs/delay.h"
#include "dspic-libs/leds.h"
#include "dspic-libs/terminal.h"


#define TRUE 1
#define FALSE 0
#define MAX_HEIGHT 60 // La altura maxima soportada es de 65536
#define MIN_HEIGHT 0
#define NUM_FLOORS 6
#define FLOOR_QUEUE_SIZE 6

// Estados del ascensor
#define STOPPED 0
#define BLOCKED 1
#define DISABLED 2
#define ASCENDING 3
#define DESCENDING 4

// ASCII
#define ESC_KEY 27

/* IDENTIFICADORES DE PAQUETES */
#define MOVE_REQUEST_SID 4
#define MOVEMENT_STATUS_SID 5
/* END IDENTIFICADORES DE PAQUETES */


/* ESTRUCTURAS DE PAQUETES */
struct MovementStatus {
    unsigned char height;
    unsigned char height_reached;
    unsigned char status;
};

struct MoveRequest {
    unsigned char goal_height;
};
/* END ESTRUCTURAS DE PAQUETES */


/* COLA DE PLANTAS */
struct FloorQueue {
    unsigned short queue[FLOOR_QUEUE_SIZE];
    unsigned short len, tail, head;
};

inline void floorQueueInit(void);
inline void floorQueuePut(unsigned short floor);
inline char floorQueueIsEmpty(void);
inline unsigned short floorQueuePop(void);
inline unsigned char floorQueueContains(unsigned short floor);

static struct FloorQueue fq;
/* END COLA DE PLANTAS */


/* CONTROL */
unsigned char control_elevator_stopped = TRUE;
unsigned char control_elevator_current_height = 0;
unsigned char control_current_electric_power = 0;
unsigned char control_elevator_state = 0;

inline unsigned short getFloor(unsigned char height);
inline unsigned char getHeight(unsigned short floor);

inline void enqueueFloor(unsigned short floor);
inline void dequeueAndSendFloor(void);

inline void processMovementStatus(struct MovementStatus *ms);
inline void sendMoveRequest(struct MoveRequest *mr);

inline void activateSosMode(void);

void main_control(void);
/* END CONTROL */


/* PLANTA */
unsigned char planta_elevator_current_height;
unsigned char planta_elevator_goal_height;

inline void processMoveRequest(struct MoveRequest *mr);
inline void sendMovementStatus(struct MovementStatus *ms);
inline void increaseHeight(void);
inline void decreaseHeight(void);
inline void printHeight(void);

void main_planta(void);
/* END PLANTA */


int main(void) {
    keyboardInit();
    LCDInit();
    initCAN();
    keyboardInit();
    initLeds();
    
    LCDPrint("0. Control");
    LCDMoveSecondLine();
    LCDPrint("1. Planta");

    short opt = -1;
    while(opt != 0 && opt != 1){
        opt = waitForKeyPress();
        LCDClear();
        switch(opt){
            case 0:
                LCDPrint(" Control");
                main_control();
                break;
            case 1:
                LCDPrint(" Planta");
                main_planta();
                break;
            default:
                LCDPrint(" Opcion incorrecta");
        }
    }

    return 0;
}

void main_planta(void){
    planta_elevator_current_height = MAX_HEIGHT;
    planta_elevator_goal_height = MAX_HEIGHT;

    while(1){
        while (planta_elevator_current_height != planta_elevator_goal_height){
            if (planta_elevator_current_height < planta_elevator_goal_height){
                increaseHeight();
            } else if (planta_elevator_current_height > planta_elevator_goal_height){
                decreaseHeight();
            }
        }
        int i;
        for (i = 0; i < 100; i++) Delay15ms(); // Simulacion de abrir y cerrar puertas
    }
}

void main_control(void){
    uartConfig();
    floorQueueInit();

    while(1){
        int i;
        for(i = 0; i < 10; ++i) Delay15ms(); // Para evitar que la tecla de seleccion anterior se solape
        unsigned short selected_floor = waitForKeyPress();

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
            case 9:
                activateSosMode();
                break;
            case 11:
                // sendStopRequest();
                break;
            default:
                continue;
        }
    }
}

inline void increaseHeight(void){
    planta_elevator_current_height += 1;
    int i; for (i = 0; i < 20; i++) Delay15ms();
    printHeight();

    struct MovementStatus ms;
    ms.height = planta_elevator_current_height;
    ms.height_reached = (planta_elevator_current_height == planta_elevator_goal_height);
    ms.status = ms.height_reached ? STOPPED : ASCENDING;
    sendMovementStatus(&ms);
}

inline void decreaseHeight(void){
    planta_elevator_current_height -= 1;
    int i; for (i = 0; i < 10; i++) Delay15ms();
    printHeight();

    struct MovementStatus ms;
    ms.height = planta_elevator_current_height;
    ms.height_reached = (planta_elevator_current_height == planta_elevator_goal_height);
    ms.status = ms.height_reached ? STOPPED : DESCENDING;
    sendMovementStatus(&ms);
}

inline void printHeight(void){
    static char buffer[20];
    sprintf(buffer, " %u", planta_elevator_current_height);
    LCDClear();
    LCDPrint(buffer);
}

inline void processMoveRequest(struct MoveRequest *mr){
    planta_elevator_goal_height = mr->goal_height;
}

inline void sendMovementStatus(struct MovementStatus *ms){
    unsigned char data[2];
    data[0] = ms->height;
    data[1] = ms->height_reached;
    data[2] = ms->status;
    transmitCAN(MOVEMENT_STATUS_SID, data, 3);
}

inline unsigned short getFloor(unsigned char height){
    return (height % 10 == 0) ? height / 10 : -1;
}
inline unsigned char getHeight(unsigned short floor){
    return floor * 10;
}

inline void enqueueFloor(unsigned short floor){
    if (floor == getFloor(control_elevator_current_height)){
        return; // Si se selecciona la misma planta donde se encuentra, se omite
    }

    setLed(floor, LED_ON);

    if (control_elevator_stopped && floorQueueIsEmpty()){
        floorQueuePut(floor);
        dequeueAndSendFloor();
    } else {
        floorQueuePut(floor);
    }
}

inline void dequeueAndSendFloor(void){
    struct MoveRequest mr;
    unsigned char floor = floorQueuePop();
    mr.goal_height = getHeight(floor);
    sendMoveRequest(&mr);
}

inline void floorQueueInit(){
    fq.head = 0;
    fq.tail = 0;
    fq.len = 0;
}

inline void floorQueuePut(unsigned short floor){
    if (floorQueueContains(floor) || fq.len == FLOOR_QUEUE_SIZE){
        return; // Evitar plantas duplicadas y cola llena
    }

    fq.queue[fq.head] = floor;
    fq.head = (fq.head + 1) % FLOOR_QUEUE_SIZE;
    fq.len += 1;
}

inline unsigned short floorQueuePop(void){
    if (fq.len == 0){
        return 0; // TODO que hacer
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

inline void sendMoveRequest(struct MoveRequest *mr){
    transmitCAN(MOVE_REQUEST_SID, (unsigned char *) mr, sizeof(struct MoveRequest));
}

inline void processMovementStatus(struct MovementStatus *ms){
    control_elevator_current_height = ms->height;
    control_elevator_state = ms->status;
    static unsigned short floor;

    if (ms->height_reached){
        LCDClear();
        static char buffer[10];
        sprintf(buffer, " Planta %u.", getFloor(ms->height));
        LCDPrint(buffer);

        floor = getFloor(ms->height);
        setLed(floor, LED_OFF);

        if (floorQueueIsEmpty()){
            control_elevator_stopped = TRUE;
        } else {
            dequeueAndSendFloor();
        }
    }
    else {
        LCDClear();
        switch(ms->status){
            case ASCENDING:
                LCDPrint(" Subiendo.");
                break;
            case DESCENDING:
                LCDPrint(" Bajando.");
                break;
        }
        control_elevator_stopped = FALSE;
    }
}

inline void activateSosMode(void){
    int i;
    LCDClear();
    LCDPrint("    Modo S.O.S");
    LCDMoveSecondLine();
    LCDPrint("    activado");
    char buffer[255];
    sprintf(buffer, "MODO S.O.S ACTIVADO"
            "\r\n-------------------\r\n"
            "Altura: %u\r\n"
            "Potencia electrica: %u\r\n"
            "Estado: %u\r\n"
            "Pulse un valor del 0 al 5 para seleccionar planta.\r\n"
            "Pulse ESC para desactivar modo S.O.S.\r\n",
            control_elevator_current_height,
            control_current_electric_power,
            control_elevator_state);

    uartColocarCursor(0, 0);
    i = 0;
    while(buffer[i] != '\0'){
        uartImprimir(buffer[i]);
        ++i;
    }

    while(1){
        switch(uartGetUltimaTecla()){
            case '0':
                enqueueFloor(0);
                break;
            case '1':
                enqueueFloor(1);
                break;
            case '2':
                enqueueFloor(2);
                break;
            case '3':
                enqueueFloor(3);
                break;
            case '4':
                enqueueFloor(4);
                break;
            case '5':
                enqueueFloor(5);
                break;
            case ESC_KEY:
                sprintf(buffer, "Desactivando modo S.O.S...\r\n\r\n");
                i = 0;
                while(buffer[i] != '\0'){
                    uartImprimir(buffer[i]);
                    ++i;
                }
                return;
                break;
        }
        for(i = 0; i < 10; ++i) Delay15ms(); // Para evitar que la tecla de seleccion anterior se solape
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

        switch(sid){
            case MOVE_REQUEST_SID:
                mr.goal_height = data[0];
                processMoveRequest(&mr);
                break;
            case MOVEMENT_STATUS_SID:
                ms.height = data[0];
                ms.height_reached = data[1];
                ms.status = data[2];
                processMovementStatus(&ms);
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