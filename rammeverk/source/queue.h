//These functions dictate the logic behind the queue-system

//2019, Audun Birkenes og Jonas Rogde Jørgensen

#ifndef QUEUE_H__
#define QUEUE_H__

//Legge til bestillinger
void queue_set_order();

//Fjerne bestillingene på den gitte etasjen. Kalles når heisen stopper i en etasje
void queue_delete_order(int floor);

//Hente bestilling fra lista med orders
int queue_get_order(elev_motor_direction_t dir, int pos);

//Sjekker om heisen skal stoppe i etasjen gitt i 0-3 med retningen motor_dir
int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor);


#endif