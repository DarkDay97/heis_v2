#include "queue.h"

//Ble nødvendig å inkludere elev.h for å kunne lese av knappene ved bestillinger
#include "elev.h"

#include <stdlib.h>

//A list of the orders. 
//Order is sorted by floor. Each floor has DOWN, COMMAND and UP except for 1. and 4. floor. 

const ORDER_SIZE 10;
static int orders[ORDER_SIZE] = {0};

void queue_set_order();

void queue_delete_order();

int queue_get_order(elev_motor_direction_t dir, int pos){

}

int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor){
    if (floor < 0){ //in case is called between floors
        return 0;
    }
    else if (((motor_dir == DIRN_DOWN) & (floor == 0)) | ((motor_dir == DIRN_UP) & (floor == 3))){   //Sørger for at heisen ikke kjører forbi endene
        return 1;
    }
    else if (orders[floor*3]){
        return 1;
    }
    else if(orders[floor*3 + motor_dir]){
        return 1;
    }
    else {return 0;}
}

//--------------------------------------------------------------------

//Hjelpefunksjoner

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

int queue_check_if_order_above(int pos){
    int i;
    for (i = pos + 1; i < N_FLOORS; i++){
        if(orders[i*3]){
            return 1;
        }
        switch (i)
        {
            case 1:
                if(orders[4]){
                    return 1;
                }
                break;

            case 2:
                if(orders[7]){
                    return 1;
                }
                break;

            case 3:
                if(orders[8]){
                    return 1;
                }
                break;

            default:
                break;
        }
    }
    return 0;
}

int queue_check_if_order_below(int pos){
    int i;
    for (i = pos - 1; i >= 0; i--){
        if(orders[i*3]){
            return 1;
        }
        switch (i)
        {
            case 2:
                if(orders[5]){
                    return 1;
                }
                break;
            case 1:
                if(orders[2]){
                    return 1;
                }
                break;
            case 0:
                if(orders[1]){
                    return 1;
                }
                break;
        
            default:
                break;
        }
    }
    return 0;
}