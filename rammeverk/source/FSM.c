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


static FSM_states_t current_state = FLOOR_CLOSED;

//Gir posisjonen i 0-3. -1 er ugyldig verdi.
static int prev_pos = -1;

static int current_pos = -1;

//Holder rede på posisjonene mellom etasjer. 1-3 for alle mellomtilstander
static int pos_between = 0;

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
    
    printf("prev_pos: %d\n", prev_pos);

    switch (current_state)
    {
        case FLOOR_CLOSED:
            if (elev_get_stop_signal()){
                current_state = FLOOR_OPEN;
                break;
            } else if (queue_have_orders()){
                prev_dir = queue_get_order(prev_dir, prev_pos, 0);
                if(prev_dir){
                    elev_set_motor_direction(prev_dir);
                    FSM_update_pos_between(prev_dir, prev_pos);
                    current_state = MOVING;
                    break;
                } else {
                    timer_reset();
                    current_state = FLOOR_OPEN;
                    break;
                }
            } else if (!queue_have_orders()){
                prev_dir = DIRN_STOP;
            }
            break;

        case FLOOR_OPEN:
            queue_delete_order(elev_get_floor_sensor_signal());
            if (elev_get_stop_signal()){
                timer_reset();
                queue_delete_all_orders();
                break;
            } else if (timer_expired()){
                current_state = FLOOR_CLOSED;
                break;
            }
            break;

        case MOVING:
            current_pos = elev_get_floor_sensor_signal();
            if (elev_get_stop_signal()){
                elev_set_motor_direction(DIRN_STOP);
                queue_delete_all_orders();
                current_state = STATIONARY;
                break;
            }
            if (current_pos + 1){
                prev_pos = current_pos;
                if (queue_should_stop_at_floor(prev_dir, prev_pos)){
                    elev_set_motor_direction(DIRN_STOP);
                    timer_reset();
                    current_state = FLOOR_OPEN;
                } else {
                    FSM_update_pos_between(prev_dir, prev_pos);
                    // oppdater etasjelys
                }
            }
            break;

        case STATIONARY:
            if (elev_get_stop_signal()){
                queue_delete_all_orders();
            } else if (queue_have_orders()){
                prev_dir = queue_get_order(prev_dir, prev_pos, pos_between); 
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
