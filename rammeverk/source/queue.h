//These functions dictate the logic behind the queue-system

//2019, Audun Birkenes og Jonas Rogde Jørgensen

#ifndef QUEUE_H__
#define QUEUE_H__

#include "elev.h"

//Legge til bestillinger
void queue_take_order();

//Fjerne bestillingene på den gitte etasjen. Kalles når heisen stopper i en etasje
void queue_delete_order(int floor);

//Kjører queue_delete_order(int floor) i en løkke over alle etasjer
void queue_delete_all_orders();

//Hente bestilling fra lista med orders
//Tanken er at den skal returnere retningen til den neste bestillingen som skal behandles
//Returnerer enten -1, 0, 1 ettersom den skal nedover, bli stående, eller oppover
elev_motor_direction_t queue_get_order(elev_motor_direction_t dir, int pos);

//Sjekker om heisen skal stoppe i etasjen gitt i 0-3 med retningen motor_dir
int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor);

//Undersøker om det finnes noen bestillinger
int queue_have_orders();

#endif