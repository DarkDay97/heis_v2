/**
 * @file
 * @brief A library containing functions regarding the FSM (Finite-state machine) module
 */

#ifndef _FSM__
#define _FSM__

/**
 * @brief Updates current state of the elevator and executes operations accordingly.
 */

void FSM_state_machine();

/**
 * @brief Initializes hardware and leads the elevator to a defined state and position.
 * 
 * @return 0 if the elevator hardware is unable to initialize, 1 otherwise.
 */

int FSM_system_init();

#endif