#include "queue.h"

//Ble nødvendig å inkludere elev.h for å kunne lese av knappene ved bestillinger

#include <stdlib.h>

//A list of the orders. 
//Order is sorted by floor. Each floor has DOWN, COMMAND and UP except for 1. and 4. floor. 
const int ORDER_SIZE = 10;
static int orders[ORDER_SIZE] = {0};

//Hjelpefunksjoner

//Undersøker om det finnes noen bestillinger over den gitte posisjonen
int queue_check_if_order_above(int pos){
    int i;
    for (i = pos*3 + 2; i < ORDER_SIZE; i++){
        if(orders[i]){
            return 1;
        }
    }
    return 0;
}

//Undersøker om det finnes noen bestillinger under den gitte posisjonen
int queue_check_if_order_below(int pos){
    int i;
    for (i = pos*3 - 2; i >= 0; i--){
        if(orders[i]){
            return 1;
        }
    }
    return 0;
}

//Leter etter og lagrer bestillinger fra innsiden av heisen
void queue_set_order_commands(){
    int c;
    for (c = 0; c < N_FLOORS; c++){
        if(elev_get_button_signal(BUTTON_COMMAND, c)){
            orders[c*3] = 1;
        }
        
    }
}

//Leter etter og lagrer bestillinger med oppover-knapper
void queue_set_order_up(){
    int u;
    for (u = 0; u < N_FLOORS - 1; u++){
        if(elev_get_button_signal(BUTTON_CALL_UP, u)){
            orders[1 + (u*3)] = 1;
        }
    }
}

//Leter etter og lagrer bestillinger med nedover-knapper
void queue_set_order_down(){
    int d;
    for (d = 1; d < N_FLOORS; d++){
        if(elev_get_button_signal(BUTTON_CALL_DOWN, d)){
            orders[(d*3)-1] = 1;
        }
    }
}

//-------------------------------------------------------------------------
//Public funksjoner

void queue_take_order(){
    queue_set_order_commands(); //Ser etter og lagrer bestillinger fra inni heisen
    queue_set_order_up();       //Ser etter og lagrer bestillinger med oppover-knapper
    queue_set_order_down();     //Ser etter og lagrer bestillinger med nedover-knapper
    
}

void queue_delete_order(int floor){
    switch (floor)
    {
        case 0:
            orders[0] = 0;
            orders[1] = 0;
            break;
    
        case 1:
            orders[2] = 0;
            orders[3] = 0;
            orders[4] = 0;
            break;
        
        case 2:
            orders[5] = 0;
            orders[6] = 0;
            orders[7] = 0;
            break;

        case 3:
            orders[8] = 0;
            orders[9] = 0;
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

elev_motor_direction_t queue_get_order(elev_motor_direction_t prev_dir, int pos){
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
    if (floor < 0){ //in case is called between floors
        return 0;
    }
    if (((motor_dir == DIRN_DOWN) & (floor == 0)) | ((motor_dir == DIRN_UP) & (floor == 3))){   //Sørger for at heisen ikke kjører forbi endene
        return 1;
    }
    if (orders[floor*3]){
        return 1;
    }
    if(orders[floor*3 + motor_dir]){
        return 1;
    }
    if ((motor_dir == DIRN_UP) & (!(queue_check_if_order_above(floor)))){
        return 1;
    }
    if ((motor_dir == DIRN_DOWN) & (!(queue_check_if_order_below(floor)))){
        return 1;
    }
    else {return 0;}
}

//Undersøker om det finnes noen bestillinger
int queue_have_orders(){
    int i;
    for (i = 0; i < ORDER_SIZE; i++){
        if(orders[i]){
            return 1;
        }
    }
    return 0;
}







