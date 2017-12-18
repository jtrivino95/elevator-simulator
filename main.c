/* 
 * File:   main.c
 * Author: jtm648
 *
 * Created on 12 de diciembre de 2017, 18:26
 */

#include "dspic-libs/keyboard.h"
#include "dspic-libs/LCDlib.h"
#include "control.c"
#include "planta.c"


int main(void) {
    keyboardInit();
    LCDInit();
    LCDPrint("Indica el cometido de la placa.");
    LCDMoveSecondLine();
    LCD("1. Control, 2. Planta");

    short opt = waitForKeyPress();
    menu:
    switch(opt){
        case 1:
            control();
        case 2:
            planta();
        default:
            goto menu; // TODO Correcto?
    }

    return (EXIT_SUCCESS);
}

