/**
 * @file
 * @brief A library containing the functions regarding the lights module
 */

#ifndef _LIGHTS__
#define _LIGHTS__

#include "elev.h"

/**
 * @brief Updates the ordering lights, on or off, according to the array "lights".
 */
void lights_update_ordering_lights();

/**
 * @brief Sets the values in the array "lights" for the ordering lights to the corresponding floor to 0.
 * 
 * @param[in] floor The floor where the ordering lights will be turned off. Must be an integer in range 0-3.
 */
void lights_reset_ordering_lights_array(int floor);

/**
 * @brief Sets all the values in the array "lights" to 0.
 */
void lights_reset_all_ordering_lights_array();

/**
 * @brief Gets signal from ordering button, and sets the corresponding element in the "lights" array to 1.
 */
void lights_set_ordering_lights_array();

#endif