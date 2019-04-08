#include "queue.h"

//Ble nødvendig å inkludere elev.h for å kunne lese av knappene ved bestillinger
#include "elev.h"

#include <stdlib.h>

//A list of the orders. Each floor has three spaces for each type of order
//DOWN, COMMAND and UP
//The first and last index is not in use since there is no DOWN in floor 1 nor UP in 4
static int orders[12] = {0};

void queue_set_order();

void queue_delete_order();

int queue_get_order();
