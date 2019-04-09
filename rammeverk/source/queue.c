#include "queue.h"

//Ble nødvendig å inkludere elev.h for å kunne lese av knappene ved bestillinger
#include "elev.h"

#include <stdlib.h>

//A list of the orders. Each floor has three spaces for each type of order
//DOWN, COMMAND and UP
//The first and last index is not in use since there is no DOWN in floor 1 nor UP in 4
const int ORDER_SIZE = 12;
static int orders[ORDER_SIZE] = {0};

void queue_set_order(){

}

void queue_delete_order();

int queue_get_order();

int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor){
    if (floor < 0){ //in case is called between floors
        return 0;
    }
    else if (((motor_dir == -1) & (floor == 0)) | ((motor_dir == 1) & (floor == 3))){
        return 1;
    }
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