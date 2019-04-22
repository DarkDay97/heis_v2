/**
 * @file
 * @brief Implementation file for the queue module
 */

#include "queue.h"
#include <stdlib.h>

//Variabler

const int ORDER_SIZE = 10;              //Antall ulike bestillinger lagret i en array
static int m_orders[ORDER_SIZE] = {0};    //Array som har oversikt over alle bestillinger

//Hjelpefunksjoner


/**
 * @brief Checks if there are any orders above the current position
 * 
 * @param[int] pos The current position
 * 
 * @return 1 if there are any orders above the current position. 0 otherwise.
 */
int queue_check_if_order_above(int pos);    

/**
 * @brief Checks if there are any orders belovw the current position
 * 
 * @param[int] pos The current position
 * 
 * @return 1 if there are any orders below the current position. 0 otherwise.
 */
int queue_check_if_order_below(int pos); 

/**
 * @brief Checks if there are any command orders and stores them at the corresponding element in the array "orders"
 *  */
void queue_set_order_commands();

/**
 * @brief Checks if there are any up orders and stores them at the corresponding element in the array "orders"
 *  */
void queue_set_order_up();

/**
 * @brief Checks if there are any down orders and stores them at the corresponding element in the array "orders"
 *  */
void queue_set_order_down();

//Public-funksjoner

void queue_take_order(){
    queue_set_order_commands(); 
    queue_set_order_up();     
    queue_set_order_down();
    
}

int queue_get_order(int floor){
    switch (floor)
    {
        case 0:
            if (m_orders[0] || m_orders[1]) { return 1; }
            break;
    
        case 1:
            if (m_orders[2] || m_orders[3] || m_orders[4]) { return 1; }
            break;
        
        case 2:
            if (m_orders[5] || m_orders[6] || m_orders[7]) { return 1; }
            break;

        case 3:
            if (m_orders[8] || m_orders[9]) { return 1; }
            break;

        default:
            break;
    }
    return 0;
}

void queue_delete_order(int floor){
    switch (floor)
    {
        case 0:
            m_orders[0] = 0;
            m_orders[1] = 0;
            break;
    
        case 1:
            m_orders[2] = 0;
            m_orders[3] = 0;
            m_orders[4] = 0;
            break;
        
        case 2:
            m_orders[5] = 0;
            m_orders[6] = 0;
            m_orders[7] = 0;
            break;

        case 3:
            m_orders[8] = 0;
            m_orders[9] = 0;
            break;

        default:
            break;
    }
}

void queue_delete_all_orders(){
    int i;
    for (i = 0; i < N_FLOORS; i++){
        queue_delete_order(i);
    }
}

elev_motor_direction_t queue_calculate_direction (elev_motor_direction_t prev_dir, int pos, int pos_between){
    if(pos_between){
        if (queue_check_if_order_above(pos_between - 1)){
            return DIRN_UP;
        }
        if (queue_check_if_order_below(pos_between)){
            return DIRN_DOWN;
        }
    }

    if (prev_dir == DIRN_UP){
        if(queue_check_if_order_above(pos)){
            return DIRN_UP;
        }
    }
    if (prev_dir == DIRN_DOWN){
        if(queue_check_if_order_below(pos)){
            return DIRN_DOWN;
        }
    }
    if (queue_check_if_order_above(pos)){
        return DIRN_UP;
    }
    if (queue_check_if_order_below(pos)){
        return DIRN_DOWN;
    }
    return DIRN_STOP;
}

int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor){
    if (floor < 0){ //I tilfelle funskjonene kalles mellom etasjer
        return 0;
    }
    if (((motor_dir == DIRN_DOWN) && (floor == 0)) || ((motor_dir == DIRN_UP) && (floor == 3))){   //Sørger for at heisen ikke kjører forbi endene
        return 1;
    }
    if (m_orders[floor*3]){
        return 1;
    }
    if(m_orders[floor*3 + motor_dir]){
        return 1;
    }
    if ((motor_dir == DIRN_UP) && (!(queue_check_if_order_above(floor)))){
        return 1;
    }
    if ((motor_dir == DIRN_DOWN) && (!(queue_check_if_order_below(floor)))){
        return 1;
    }
    else {return 0;}
}

int queue_have_orders(){
    int i;
    for (i = 0; i < ORDER_SIZE; i++){
        if(m_orders[i]){
            return 1;
        }
    }
    return 0;
}

//Implementasjon av hjelpefunksjoner

int queue_check_if_order_above(int pos){
    int i;
    for (i = pos*3 + 2; i < ORDER_SIZE; i++){
        if(m_orders[i]){
            return 1;
        }
    }
    return 0;
}

int queue_check_if_order_below(int pos){
    int i;
    for (i = pos*3 - 2; i >= 0; i--){
        if(m_orders[i]){
            return 1;
        }
    }
    return 0;
}

void queue_set_order_commands(){
    int c;
    for (c = 0; c < N_FLOORS; c++){
        if(elev_get_button_signal(BUTTON_COMMAND, c)){
            m_orders[c*3] = 1;
        }
        
    }
}

void queue_set_order_up(){
    int u;
    for (u = 0; u < N_FLOORS - 1; u++){
        if(elev_get_button_signal(BUTTON_CALL_UP, u)){
            m_orders[1 + (u*3)] = 1;
        }
    }
}

void queue_set_order_down(){
    int d;
    for (d = 1; d < N_FLOORS; d++){
        if(elev_get_button_signal(BUTTON_CALL_DOWN, d)){
            m_orders[(d*3)-1] = 1;
        }
    }
}