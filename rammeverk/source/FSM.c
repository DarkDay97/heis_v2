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

//Variabler

static FSM_states_t current_state = FLOOR_CLOSED;       //Holder rede på nåværende tilstand
static elev_motor_direction_t direction = DIRN_STOP;    //Holder rede på retningen heisen kjører i
static int prev_pos = -1;                               //Gir posisjonen i 0-3. -1 er ugyldig verdi.
static int temp_pos = -1;                               //Midlertidig variabel for å forhindre feilavlesning fra etasjesensor
static int pos_between = 0;                             //Holder rede på posisjonene mellom etasjer. 1-3 for alle mellomtilstander

//Hjelpefunksjoner

void FSM_update_pos_between(elev_motor_direction_t dir, int pos);

//Public-funksjoner

int FSM_system_init(){
    if (!elev_init()) {
            printf("Unable to initialize elevator hardware!\n");
            return 0;
        }

    elev_set_motor_direction(DIRN_DOWN);
    while(1){
        if(elev_get_floor_sensor_signal() + 1){
            elev_set_motor_direction(DIRN_STOP);
            prev_pos = elev_get_floor_sensor_signal();
            current_state = FLOOR_CLOSED;
            break;
        }
    }
    return 1;
}

void FSM_state_machine(){

    lights_update_ordering_lights();

    elev_set_stop_lamp(elev_get_stop_signal());
    
    if(!(elev_get_stop_signal())){
        queue_take_order();
        lights_set_ordering_lights_array();
    }

    //printf("prev_pos: %d\n", prev_pos);

    switch (current_state)
    {
        case FLOOR_CLOSED:
            if (elev_get_stop_signal()){
                elev_set_door_open_lamp(1);
                current_state = FLOOR_OPEN;
                break;
            }
            if (queue_have_orders()){
                direction = queue_calculate_direction(direction, prev_pos, 0);
                if(direction){
                    elev_set_motor_direction(direction);
                    FSM_update_pos_between(direction, prev_pos);
                    current_state = MOVING;
                    break;
                } else {
                    timer_reset();
                    elev_set_door_open_lamp(1);
                    current_state = FLOOR_OPEN;
                    break;
                }
            }
            break;

        case FLOOR_OPEN:
            if(queue_get_order(prev_pos)){
                timer_reset();
            }
            queue_delete_order(prev_pos);
            lights_reset_ordering_lights_array(prev_pos);
            if (elev_get_stop_signal()){
                timer_reset();
                queue_delete_all_orders();
                lights_reset_all_ordering_lights_array();
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
                lights_reset_all_ordering_lights_array();
                current_state = STATIONARY;
                break;
            }
            if (temp_pos + 1){
                prev_pos = temp_pos;
                elev_set_floor_indicator(prev_pos);
                if (queue_should_stop_at_floor(direction, prev_pos)){
                    elev_set_motor_direction(DIRN_STOP);
                    timer_reset();
                    elev_set_door_open_lamp(1);
                    current_state = FLOOR_OPEN;
                } else {
                    FSM_update_pos_between(direction, prev_pos);
                    // oppdater etasjelys
                }
            }
            break;

        case STATIONARY:
            if (elev_get_stop_signal()){
                queue_delete_all_orders();
                lights_reset_all_ordering_lights_array();
            } else if (queue_have_orders()){
                direction = queue_calculate_direction(direction, prev_pos, pos_between); 
                elev_set_motor_direction(direction);
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

//Implementasjon av hjelpefunksjoner

void FSM_update_pos_between(elev_motor_direction_t dir, int pos){
    if (dir == DIRN_UP){
        pos_between = pos + 1;
    } else if (dir == DIRN_DOWN){
        pos_between = pos;
    }
}