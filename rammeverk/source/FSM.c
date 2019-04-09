#include "FSM.h"
#include "elev.h"

typedef enum {
    FLOOR_CLOSED, 
    FLOOR_OPEN,
    MOVING, 
    STATIONARY,
    TEST
} FSM_states_t;

typedef enum {
    ONE_TWO, 
    TWO_THREE, 
    THREE_FOUR
}FSM_between_floor_pos_t;

static FSM_states_t current_state = FLOOR_CLOSED;

static int prev_pos = -1;

static elev_motor_direction_t prev_dir = DIRN_STOP;

void FSM_state_machine(){

    switch (current_state)
    {
        case FLOOR_CLOSED:
            /* code */
            break;

        case FLOOR_OPEN:
            /* code */
            break;

        case MOVING:
            /* code */
            break;

        case STATIONARY:
            /* code */
            break;

        case TEST:
            /* code */
            break;

        default:
            break;
    }
}

void FSM_system_init(){
    elev_set_motor_direction(DIRN_DOWN);
    while(1){
        if(elev_get_floor_sensor_signal() == 0){
            break;
        }
    }
}