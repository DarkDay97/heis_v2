/**
 * @file
 * @brief The implementation file of the timer module
 */

#include "timer.h"

//Variabler

/**
 * @brief How long the delay timer is given in seconds
 */
const int TIME_LIMIT = 3;

/**
 * @brief The clock used to see if TIME_LIMIT seconds have passed
 * 
 */
static clock_t clock_var;

//Public-funksjoner

void timer_reset(){
    clock_var = clock();
}

int timer_expired(){
    clock_t clock_now = clock();
    return ((clock_now - clock_var) >= TIME_LIMIT * CLOCKS_PER_SEC);
}