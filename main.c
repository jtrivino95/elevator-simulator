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


#define TRUE 1
#define FALSE 0
#define MAX_HEIGHT 1000 // La altura máxima soportada es de 65536
#define MIN_HEIGHT 200
#define NUM_FLOORS 6
#define FLOOR_QUEUE_SIZE 6

/* IDENTIFICADORES DE PAQUETES */
#define MOVE_REQUEST_SID 4
#define MOVEMENT_STATUS_SID 5
/* END IDENTIFICADORES DE PAQUETES */


/* ESTRUCTURAS DE PAQUETES */
struct MovementStatus {
    unsigned short height;
    unsigned char height_reached;
};

struct MoveRequest {
    unsigned short goal_height;
};
/* END ESTRUCTURAS DE PAQUETES */


/* COLA DE PLANTAS */
struct FloorQueue {
    unsigned short *queue;
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
static unsigned short height_floor_map[NUM_FLOORS] = { 200, 300, 400, 500, 600, 800 };
static unsigned char elevator_stopped = TRUE; // TODO buena estrategia??
static unsigned char control_elevator_current_height = TRUE; // TODO buena estrategia??

inline void selectFloor(unsigned short floor);

inline void processMovementStatus(struct MovementStatus *ms);
inline void sendMoveRequest(struct MoveRequest *mr);

void main_control(void);
/* END CONTROL */


/* PLANTA */
unsigned short planta_elevator_current_height;
unsigned short elevator_goal_height;

inline void processMoveRequest(struct MoveRequest *mr);
inline void sendMovementStatus(void);
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
                LCDPrint("Control");
                main_control();
                break;
            case 1:
                LCDPrint("Planta");
                main_planta();
                break;
            default:
                LCDPrint("Opcion incorrecta");
        }
    }

    return 0;
}

void main_planta(void){
    planta_elevator_current_height = MAX_HEIGHT;
    elevator_goal_height = MIN_HEIGHT;

    while(1){
        while (planta_elevator_current_height != elevator_goal_height){
            if (planta_elevator_current_height < elevator_goal_height){
                increaseHeight();

            } else if (planta_elevator_current_height > elevator_goal_height){
                decreaseHeight();

            }
        }
    }
}

void main_control(void){
    floorQueueInit();


    LCDClear();
    floorQueuePut(0);
    floorQueuePut(1);
    floorQueuePut(3);
    LCDMoveSecondLine();
    floorQueuePut(5);
    floorQueuePut(4);
    floorQueuePut(2);
//    floorQueuePut(5);
//    floorQueuePut(2);
//    floorQueuePut(4);
    floorQueueContains(0);

//    floorQueuePut(1);
//    floorQueuePut(2);
//    floorQueuePut(3);
//    floorQueuePut(4);
//    floorQueuePut(5);
//    floorQueuePut(5);
//    floorQueuePut(2);
//    floorQueuePut(3);

//    LCDClear();
//    static char buffer[20];
//    while(!floorQueueIsEmpty()){
//        sprintf(buffer, " %u", fq.len);
//        LCDPrint(buffer);
//        LCDPrint(",");
//    }
    while(1);

    while(1){
        int i;
        for(i = 0; i < 10; ++i) Delay15ms(); // Para evitar que la tecla de seleccion anterior se solape
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

        switch(sid){
            case MOVE_REQUEST_SID:
                break;
            case MOVEMENT_STATUS_SID:
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

inline void increaseHeight(void){
    planta_elevator_current_height += 1;
    Delay15ms();Delay15ms();
    printHeight();
    //sendMovementStatus();
}

inline void decreaseHeight(void){
    planta_elevator_current_height -= 1;
    Delay15ms();
    printHeight();
    //sendMovementStatus();
}

inline void printHeight(void){
    static char buffer[20];
    sprintf(buffer, " %u", planta_elevator_current_height);
    LCDClear();
    LCDPrint(buffer);
}

inline void processMoveRequest(struct MoveRequest *mr){
    elevator_goal_height = mr->goal_height;
}

inline void sendMovementStatus(void){
    static struct MovementStatus ms;
    ms.height = planta_elevator_current_height;
    ms.height_reached = (planta_elevator_current_height == elevator_goal_height);
    transmitCAN(MOVEMENT_STATUS_SID, (unsigned char *)&ms, sizeof(struct MovementStatus));
}

inline void selectFloor(unsigned short floor){
    floorQueuePut(floor); // TEST
    return;

    struct MoveRequest mr;
    mr.goal_height = height_floor_map[floor];

    if (elevator_stopped && floorQueueIsEmpty())    sendMoveRequest(&mr);
    else                                            floorQueuePut(floor);
}

inline void floorQueueInit(){
    fq.head = 0;
    fq.tail = 0;
    fq.len = 0;
    fq.queue[FLOOR_QUEUE_SIZE]; // array de tamanio FLOOR_QUEUE_SIZE
}

inline void floorQueuePut(unsigned short floor){
//    if (floorQueueContains(floor)) return; // Evitar plantas duplicadas

    setLed(floor, LED_ON);


    fq.queue[fq.head] = floor;
    fq.head = (fq.head + 1) % FLOOR_QUEUE_SIZE;
    fq.len += 1;

    char buffer[10];
    sprintf(buffer, " %u:%u", fq.tail, fq.head);
    LCDPrint(buffer);

}

inline unsigned short floorQueuePop(void){
    if (floorQueueIsEmpty()) return 0; // TODO que hacer

    unsigned short floor = fq.queue[fq.tail];
    fq.tail = (fq.tail + 1) % FLOOR_QUEUE_SIZE;
    fq.len -= 1;

    setLed(floor, LED_OFF);
    return floor;
}

inline unsigned char floorQueueContains(unsigned short floor){
    unsigned short i;
    static char buffer[20];

    for (i = fq.tail; i < fq.head; i = (i + 1) % FLOOR_QUEUE_SIZE) {

//        sprintf(buffer, " %u", fq.queue[i]);
//        LCDPrint(buffer); LCDPrint(",");
//        if(fq.queue[i] == floor) return TRUE;
    }
    return FALSE;
}

inline char floorQueueIsEmpty(void){
    return fq.len == 0;
}

inline void sendMoveRequest(struct MoveRequest *mr){
    transmitCAN(MOVE_REQUEST_SID, (unsigned char *) mr, sizeof(struct MoveRequest));
}

inline void processMovementStatus(struct MovementStatus *ms){ //TODO: Demasiado compleja para estar en interrupcion?
    static struct MoveRequest mr;
    static unsigned short next_floor;

    control_elevator_current_height = ms->height;

    if (ms->height_reached){

        if (floorQueueIsEmpty()){
            elevator_stopped = TRUE;
        } else {
            next_floor = floorQueuePop();
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