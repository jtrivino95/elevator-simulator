#include <stdio.h>
#include <stdlib.h>
#include <p30F4011.h>
#include "dspic-libs/LCDlib.h"
#include "dspic-libs/libCAN.h"
#include "dspic-libs/delay.h"
#include "communication_data_structs.h"


#define MAX_HEIGHT 1000 // La altura máxima soportada es de 65536
#define MIN_HEIGHT 200

static unsigned short elevator_current_height;
static unsigned short elevator_goal_height;

inline void increaseHeight(void);
inline void decreaseHeight(void);
inline void printHeight(void);
inline void processMoveRequest(struct MoveRequest *mr);
inline void sendMovementStatus(void);

void planta(void){
    LCDInit();
    initCAN();
    elevator_current_height = MIN_HEIGHT;
    elevator_goal_height = MIN_HEIGHT;

    while(1){
        while (elevator_current_height != elevator_goal_height){
            if (elevator_current_height < elevator_goal_height){
                increaseHeight();

            } else if (elevator_current_height > elevator_goal_height){
                decreaseHeight();

            }
        }
    }
}

inline void increaseHeight(void){
    elevator_current_height += 1;
    Delay15ms();
    printHeight();
    sendMovementStatus();
}

inline void decreaseHeight(void){
    elevator_current_height -= 1;
    Delay15ms();
    printHeight();
    sendMovementStatus();
}

inline void printHeight(void){
    static char buffer[10];
    sprintf(buffer, "%d", elevator_current_height);
    LCDClear();
    LCDPrint(buffer)
}

inline void processMoveRequest(struct MoveRequest *mr){
    elevator_goal_height = mr->goal_height;
}

inline void sendMovementStatus(void){
    static struct MovementStatus ms;
    ms.height = elevator_current_height;
    ms.height_reached = (elevator_current_height == elevator_goal_height);
    transmitCAN(&ms, sizeof(struct MovementStatus));
}

void _ISR _C1Interrupt(void) {
    static unsigned char data[9];
    static char dlc;

    if (C1INTFbits.RX0IF == 1) {
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
        static struct MoveRequest *mr;
        mr = &data; // TODO esto funciona??
        processMoveRequest(mr);

        // Clear rx buffer 0
        C1RX0CONbits.RXFUL = 0;
        // Clear CAN reception buffer 0 interrupt
        C1INTFbits.RX0IF = 0;
    }
    // Clear CAN interrupt
    IFS1bits.C1IF = 0;
}
