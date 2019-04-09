#ifndef _TIMER__
#define _TIMER__

#include <time.c>

void timer_init(); //Initialize by resetting the timer

void timer_reset(); //Sets a new timestamp

int timer_expired(); //Checks if it has been more than 3 secounds since timstamp



#endif