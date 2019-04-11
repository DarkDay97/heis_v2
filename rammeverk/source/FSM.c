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

static FSM_states_t m_current_state = FLOOR_CLOSED;       //Holder rede på nåværende tilstand
static elev_motor_direction_t m_direction = DIRN_STOP;    //Holder rede på retningen heisen kjører i
static int m_prev_pos = -1;                               //Gir posisjonen i 0-3. -1 er ugyldig verdi.
static int m_temp_pos = -1;                               //Midlertidig variabel for å forhindre feilavlesning fra etasjesensor
static int m_pos_between = 0;                             //Holder rede på posisjonene mellom etasjer. 1-3 for alle mellomtilstander

//Hjelpefunksjoner

void FSM_update_m_pos_between(elev_motor_direction_t dir, int pos);

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
            m_prev_pos = elev_get_floor_sensor_signal();
            elev_set_floor_indicator(m_prev_pos);
            m_current_state = FLOOR_CLOSED;
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

    //printf("m_prev_pos: %d\n", m_prev_pos);

    switch (m_current_state)
    {
        case FLOOR_CLOSED:
            if (elev_get_stop_signal()){
                elev_set_door_open_lamp(1);
                m_current_state = FLOOR_OPEN;
                break;
            }
            if (queue_have_orders()){
                m_direction = queue_calculate_direction(m_direction, m_prev_pos, 0);
                if(m_direction){
                    elev_set_motor_direction(m_direction);
                    FSM_update_m_pos_between(m_direction, m_prev_pos);
                    m_current_state = MOVING;
                    break;
                } else {
                    timer_reset();
                    elev_set_door_open_lamp(1);
                    m_current_state = FLOOR_OPEN;
                    break;
                }
            }
            break;

        case FLOOR_OPEN:
            if(queue_get_order(m_prev_pos)){
                timer_reset();
            }
            queue_delete_order(m_prev_pos);
            lights_reset_ordering_lights_array(m_prev_pos);
            if (elev_get_stop_signal()){
                timer_reset();
                queue_delete_all_orders();
                lights_reset_all_ordering_lights_array();
                break;
            }
            if (timer_expired()){
                elev_set_door_open_lamp(0);
                m_current_state = FLOOR_CLOSED;
                break;
            }
            
            break;

        case MOVING:
            m_temp_pos = elev_get_floor_sensor_signal();
            if (elev_get_stop_signal()){
                elev_set_motor_direction(DIRN_STOP);
                queue_delete_all_orders();
                lights_reset_all_ordering_lights_array();
                m_current_state = STATIONARY;
                break;
            }
            if (m_temp_pos + 1){
                m_prev_pos = m_temp_pos;
                elev_set_floor_indicator(m_prev_pos);
                if (queue_should_stop_at_floor(m_direction, m_prev_pos)){
                    elev_set_motor_direction(DIRN_STOP);
                    timer_reset();
                    elev_set_door_open_lamp(1);
                    m_current_state = FLOOR_OPEN;
                } else {
                    FSM_update_m_pos_between(m_direction, m_prev_pos);
                    // oppdater etasjelys
                }
            }
            break;

        case STATIONARY:
            if (elev_get_stop_signal()){
                queue_delete_all_orders();
                lights_reset_all_ordering_lights_array();
            } else if (queue_have_orders()){
                m_direction = queue_calculate_direction(m_direction, m_prev_pos, m_pos_between); 
                elev_set_motor_direction(m_direction);
                m_current_state = MOVING;
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

void FSM_update_m_pos_between(elev_motor_direction_t dir, int pos){
    if (dir == DIRN_UP){
        m_pos_between = pos + 1;
    } else if (dir == DIRN_DOWN){
        m_pos_between = pos;
    }
}