/*
 * can.h
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */
#include"main.h"
#ifndef INC_CAN_H_
#define INC_CAN_H_

/**
  * @brief CAN Frame structure for sending and receiving data
  */
typedef struct {
    uint32_t id;        /**< CAN identifier (11-bit for standard, 29-bit for extended) */
    uint8_t  ide;       /**< 0 for standard identifier, 1 for extended identifier */
    uint8_t  rtr;       /**< 0 for data frame, 1 for remote frame */
    uint8_t  dlc;       /**< Data length code (0-8) */
    uint8_t  data[8];   /**< Data bytes */
} can_frame_t;


STATUS can_init(uint32_t Baud_Rate);
void send_can(CAN_FRAME can_frame);
void can_set_filter(uint32_t *std_ids,uint32_t *ext_ids );


#endif /* INC_CAN_H_ */
