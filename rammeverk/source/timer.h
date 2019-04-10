/**
 * @file
 * @brief A library containing functions regarding the timer module
 */

#ifndef _TIMER__
#define _TIMER__

#include <time.h>

/**
 * @brief Sets a new timestamp.
 */
void timer_reset();

/**
 * @brief Checks if it has been more than TIME_LIMIT secounds since last timstamp.
 * 
 * @return 1 if the time limit has surpassed. 0 otherwise.
 */
int timer_expired();



#endif