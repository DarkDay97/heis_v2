/**
 * @file
 * @brief A library containing functions regarding the queue module
 */

//These functions dictate the logic behind the queue-system

#ifndef QUEUE_H__
#define QUEUE_H__

#include "elev.h"

/**
 * @brief Sets the value in the array "orders" to the corresponding order to 1.
 */
void queue_take_order();

/**
 * @brief Checks if there are any orders to given floor.
 * 
 * @param[in] floor The floor where the function looks for orders. Must be an integer in range 0-3.
 * 
 * @return 1 if there is an order at floor @p floor. 0 otherwise.
 */
int queue_get_order(int floor);

/**
 * @brief Sets the value in the array "orders" to the corresponding order to 0.
 * 
 * @param[in] floor The floor where the function deletes the corresponding orders. Must be an integer in range 0-3.
 */
void queue_delete_order(int floor);

/**
 * @brief Sets all values in the array "orders" to 0.
 */
void queue_delete_all_orders();

/**
 * @brief Calculates the elevator's next direction.
 * 
 * @param[in] dir The direction of the elevator's movement. Must have the following values: DIRN_DOWN = -1, DIRN_STOP = 0 or DIRN_UP = 1.
 * 
 * @param[in] pos Previous detected floor by sensor. Must be in range 0-3.
 * 
 * @param[in] pos_between Keeps track of which floors the elevator is located between at any given time. Must be in range 1-3.
 * 
 * @return DIRN_DOWN = -1 when moving down. DIRN_STOP = 0 when stationary. DIRN_UP = 1 when moving up.
 */
elev_motor_direction_t queue_calculate_direction(elev_motor_direction_t dir, int pos, int pos_between);

/**
 * @brief Checks if the elevator should stop at the given floor.
 * 
 * @param[in] motor_dir The direction of the elevator's movement. Must have the following values: DIRN_DOWN = -1, DIRN_STOP = 0 or DIRN_UP = 1.
 * 
 * @param[in] floor The floor where the function checks if it should stop. Must be an integer in range 0-3.
 * 
 * @return 1 if elevator should stop. 0 otherwise.
 */
int queue_should_stop_at_floor(elev_motor_direction_t motor_dir, int floor);

/**
 * @brief Checks if there are any orders.
 * 
 * @return 1 if there are any orders. 0 otherwise.
 */
int queue_have_orders();

#endif