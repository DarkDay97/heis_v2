#include "FSM.h"
#include "elev.h"

typedef enum {
    INIT, 
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

static FSM_states_t current_state = INIT;

static int prev_pos = -1;

static elev_motor_direction_t prev_dir = DIRN_STOP;

void FSM_state_machine(){

    switch (current_state)
    {
        case INIT:
            /* code */
            break;

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
