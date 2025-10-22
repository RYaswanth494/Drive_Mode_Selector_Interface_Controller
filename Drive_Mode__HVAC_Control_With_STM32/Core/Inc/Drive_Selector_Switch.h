/*
 * Drive_Selector_Switch.h
 *
 *  Created on: Sep 12, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_DRIVE_SELECTOR_SWITCH_H_
#define INC_DRIVE_SELECTOR_SWITCH_H_
// Switch states
typedef enum {
    SW_STATE_NEUTRAL = 0,
    SW_STATE_FORWARD = 1,
    SW_STATE_REVERSE = 2
} switch_state_t;
void Switch_Pins_int();
switch_state_t Switch_update(void);
#endif /* INC_DRIVE_SELECTOR_SWITCH_H_ */
