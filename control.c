#include <stdio.h>
#include <stdlib.h>
#include <p30F4011.h>
#include "dspic-libs/LCDlib.h"
#include "dspic-libs/leds.h"
#include "dspic-libs/libCAN.h"
#include "dspic-libs/delay.h"
#include "dspic-libs/keyboard.h"
#include "communication_data_structs.h"


#define NUM_FLOORS 6
#define FLOOR_QUEUE_SIZE 6
#define TRUE 1
#define FALSE 0

struct FloorQueue {
    unsigned short *queue;
    unsigned char len;
    unsigned char tail, head;
}

static struct FloorQueue fq;
static unsigned short height_floor_map[NUM_FLOORS] = { 200, 300, 400, 500, 600, 800 };
static unsigned char elevator_stopped = TRUE; // TODO buena estrategia??
static unsigned char elevator_current_height = TRUE; // TODO buena estrategia??

inline void selectFloor(unsigned short floor);

inline void floorQueueInit(void);
inline void floorQueuePut(unsigned short floor);
inline char floorQueueIsEmpty(void);
inline unsigned short floorQueuePop(void);
inline unsigned char floorQueueContains(unsigned short floor);

inline void processMovementStatus(struct MovementStatus *ms);
inline void sendMoveRequest(struct MoveRequest *mr);

void control(void){
    LCDInit();
    initCAN();
    keyboardInit();
    initLeds();
    floorQueueInit();

    while(1){
        unsigned short selected_floor = waitForKeyPress();

        switch(selected_floor){
            case 0:
                selectFloor(0);
                break;
            case 2:
                selectFloor(1);
                break;
            case 3:
                selectFloor(2);
                break;
            case 5:
                selectFloor(3);
                break;
            case 6:
                selectFloor(4);
                break;
            case 8:
                selectFloor(5);
                break;
            case 9:
                // sendSosRequest();
                break;
            case 11:
                // sendStopRequest();
                break;
            default:
                continue;
        }
    }
}

inline void selectFloor(unsigned short floor){
    struct MoveRequest mr;
    mr.goal_height = height_floor_map[floor];

    if (elevator_stopped && floorQueueIsEmpty())    sendMoveRequest(&mr);
    else                                            floorQueuePut(floor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  
 *                      Gestion de la cola de plantas                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

inline void floorQueueInit(){
    fq.head = 0;
    fq.tail = 0;
    fq.len = 0;
    fq.queue = (unsigned short *) malloc(sizeof(unsigned short) * FLOOR_QUEUE_SIZE); // array de tamanio FLOOR_QUEUE_SIZE
}

inline void floorQueuePut(unsigned short floor){
    if (floorQueueContains(floor)) return; // Evitar plantas duplicadas

    setLed(floor, LED_ON);

    fq.queue[fq.head] = floor;
    fq.head = (fq.head + 1) % FLOOR_QUEUE_SIZE;
    fq.len += 1
}

inline unsigned short floorQueuePop(void){
    if (floorQueueIsEmpty()) return;

    setLed(floor, LED_OFF);

    unsigned short floor = fq.queue[fq.tail];
    fq.tail = (fq.tail + 1) % FLOOR_QUEUE_SIZE;
    fq.len -= 1;
    return floor;
}

inline unsigned char floorQueueContains(short floor){
    unsigned char i;
    for (i = fq.tail; i <= fq.head; i = (i + 1) % FLOOR_QUEUE_SIZE) {
        if(fq.queue[i] == floor) return TRUE;
    }
    return FALSE;
}

inline char floorQueueIsEmpty(void){
    return fq.len == 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                    Gestion de la red                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

inline void sendMoveRequest(struct MoveRequest *mr){
    transmitCAN(mr, sizeof(struct MoveRequest));
}

inline void processMovementStatus(struct MovementStatus *ms){ //TODO: Demasiado compleja para estar en interrupcion?
    static struct MoveRequest mr;

    elevator_current_height = mr->height;

    if (ms->height_reached){

        if (floorQueueIsEmpty()){
            elevator_stopped = TRUE;
        } else {
            static unsigned short next_floor = floorQueuePop();
            mr.goal_height = next_floor;
            sendMoveRequest(&mr);
        }
    }
    else {
        elevator_stopped = FALSE;
    }

    if (elevator_stopped){
        LCDClear();
        LCDPrint("Parado");
    } else {
        LCDClear();
        LCDPrint("En movimiento");
    }
}

void _ISR _C1Interrupt(void) {
    if (C1INTFbits.RX0IF == 1) {
        static unsigned char data[9];
        static char dlc;

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

        // TODO como identificar tipo de paquete
        static struct MovementStatus *ms;
        ms = &data; // TODO esto funciona??
        processMovementStatus(ms);

        // Clear rx buffer 0
        C1RX0CONbits.RXFUL = 0;
        // Clear CAN reception buffer 0 interrupt
        C1INTFbits.RX0IF = 0;
    }
    // Clear CAN interrupt
    IFS1bits.C1IF = 0;
}