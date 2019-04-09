#include "FSM.h"
#include "elev.h"
#include "timer.h"
#include "queue.h"

#include <stdio.h>


typedef enum {
    FLOOR_CLOSED, 
    FLOOR_OPEN,
    MOVING, 
    STATIONARY,
    TEST
} FSM_states_t;

typedef enum {
    NONE,
    ONE_TWO, 
    TWO_THREE, 
    THREE_FOUR
}FSM_between_floor_pos_t;

static FSM_states_t current_state = FLOOR_CLOSED;

//Gir posisjonen i 0-3. -1 er ugyldig verdi.
static int prev_pos = -1;
static FSM_between_floor_pos_t pos_between = NONE;

static elev_motor_direction_t prev_dir = DIRN_STOP;


void FSM_system_init(){
    elev_set_motor_direction(DIRN_DOWN);
    while(1){
        if(elev_get_floor_sensor_signal() == 0){
            elev_set_motor_direction(DIRN_STOP);
            current_state = FLOOR_CLOSED;
            prev_pos = 0;
            break;
        }
    }
}

void FSM_update_pos_between(elev_motor_direction_t dir, int pos){
    if (dir == DIRN_UP){
        pos_between = pos + 1;
    } else if (dir == DIRN_DOWN){
        pos_between = pos;
    }
}

void FSM_state_machine(){

    queue_take_order();
    
    printf("current state: %d\n", current_state);

    switch (current_state)
    {
        case FLOOR_CLOSED:
            if (elev_get_stop_signal()){
                current_state = FLOOR_OPEN;
            } else if (queue_have_orders()){
                prev_dir = queue_get_order(prev_dir, prev_pos);
                elev_set_motor_direction(prev_dir);
                FSM_update_pos_between(prev_dir, prev_pos);
                current_state = MOVING;
                
            } else if (!queue_have_orders()){
                prev_dir = DIRN_STOP;
            }
            break;

        case FLOOR_OPEN:
            queue_delete_order(elev_get_floor_sensor_signal());

            if (timer_expired()){
                current_state = FLOOR_CLOSED;
            } else if (elev_get_stop_signal()){
                timer_reset();
                queue_delete_all_orders();
            }
            break;

        case MOVING:
            if (elev_get_floor_sensor_signal() + 1){
                if (queue_should_stop_at_floor(prev_dir, elev_get_floor_sensor_signal())){
                    prev_pos = elev_get_floor_sensor_signal();
                    elev_set_motor_direction(DIRN_STOP);
                    timer_reset();
                    current_state = FLOOR_OPEN;
                }
            } else if (elev_get_stop_signal()){
                elev_set_motor_direction(DIRN_STOP);
                queue_delete_all_orders();
                current_state = STATIONARY;
            }
            break;

        case STATIONARY:
            if (elev_get_stop_signal()){
                queue_delete_all_orders();
            } else if (queue_have_orders()){
                prev_dir = queue_get_order(prev_dir, prev_pos); //Funker ikke nødvendigvis som den skal
                elev_set_motor_direction(prev_dir);
                current_state = MOVING;
            }
            break;

        case TEST:
            /* code */
            break;

        default:
            break;
    }
}
