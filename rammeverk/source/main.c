/**
 * @file
 * @brief File for the main function of the program
 */

#include "FSM.h"
#include <stdio.h>

int main() {

    // Initialize hardware
   if(!FSM_system_init()){
       return 1;
   }

    while (1) {
        FSM_state_machine();
    }

    return 0;
}