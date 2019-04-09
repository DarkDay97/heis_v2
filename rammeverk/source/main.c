#include "elev.h"
#include "FSM.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

   FSM_system_init();


    while (1) {
        // Stop elevator and exit program if the obstruction signal is high
        if (elev_get_obstruction_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        FSM_state_machine();
    }

    return 0;
}
