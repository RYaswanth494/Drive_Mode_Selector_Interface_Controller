/*
 * can.h
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */
#include"main.h"
#ifndef INC_CAN_H_
#define INC_CAN_H_
STATUS can_init(uint32_t Baud_Rate);
void send_can(CAN_FRAME can_frame);
#endif /* INC_CAN_H_ */
