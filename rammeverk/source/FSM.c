#include "FSM.h"
#include "elev.h"
#include "timer.h"
#include "queue.h"
#include "lights.h"

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

//Midlertidig variabel for å forhindre feilavlesning fra etasjesensor
static int temp_pos = -1;

//Holder rede på posisjonene mellom etasjer. 1-3 for alle mellomtilstander
static int pos_between = 0;

static elev_motor_direction_t prev_dir = DIRN_STOP;


void FSM_system_init(){
    elev_set_motor_direction(DIRN_DOWN);
    while(1){
        if(elev_get_floor_sensor_signal() == 0){
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(0);
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

    lights_update_ordering_lights();

    elev_set_stop_lamp(elev_get_stop_signal());
    
    if(!(elev_get_stop_signal())){
        queue_take_order();
        lights_set_lights();
    }

    //printf("prev_pos: %d\n", prev_pos);

    switch (current_state)
    {
        case FLOOR_CLOSED:
            if (elev_get_stop_signal()){
                elev_set_door_open_lamp(1);
                current_state = FLOOR_OPEN;
                break;
            } else if (queue_have_orders()){
                prev_dir = queue_calculate_direction(prev_dir, prev_pos, 0);
                if(prev_dir){
                    elev_set_motor_direction(prev_dir);
                    FSM_update_pos_between(prev_dir, prev_pos);
                    current_state = MOVING;
                    break;
                } else {
                    timer_reset();
                    elev_set_door_open_lamp(1);
                    current_state = FLOOR_OPEN;
                    break;
                }
            } else if (!queue_have_orders()){
                prev_dir = DIRN_STOP;
            }
            break;

        case FLOOR_OPEN:
            if(queue_get_order(prev_pos)){
                timer_reset();
            }
            queue_delete_order(prev_pos);
            lights_reset_lights(prev_pos);
            if (elev_get_stop_signal()){
                timer_reset();
                queue_delete_all_orders();
                lights_reset_all_lights();
                break;
            }
            if (timer_expired()){
                elev_set_door_open_lamp(0);
                current_state = FLOOR_CLOSED;
                break;
            }
            
            break;

        case MOVING:
            temp_pos = elev_get_floor_sensor_signal();
            if (elev_get_stop_signal()){
                elev_set_motor_direction(DIRN_STOP);
                queue_delete_all_orders();
                lights_reset_all_lights();
                current_state = STATIONARY;
                break;
            }
            if (temp_pos + 1){
                prev_pos = temp_pos;
                elev_set_floor_indicator(prev_pos);
                if (queue_should_stop_at_floor(prev_dir, prev_pos)){
                    elev_set_motor_direction(DIRN_STOP);
                    timer_reset();
                    elev_set_door_open_lamp(1);
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
                lights_reset_all_lights();
            } else if (queue_have_orders()){
                prev_dir = queue_calculate_direction(prev_dir, prev_pos, pos_between); 
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
