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

#define CAN_FILTER_BANKS 14
void check_can_bus(void);
STATUS can_init(uint32_t Baud_Rate);
void send_can_data_frame(can_frame_t can_frame);
void send_can_remote_frame(can_frame_t can_frame);
void configure_can_filters(const uint32_t *std_ids, uint8_t std_count,const uint32_t *ext_ids, uint8_t ext_count);
uint8_t receive_can_data(uint8_t fifo_number, can_frame_t* rx_message) ;
uint32_t can_recv_bulk(can_frame_t *frames, uint32_t max_frames);
uint8_t CAN_MessagePending(uint8_t fifo);
uint8_t can_rx(can_frame_t *RxMessage,uint8_t fifo_number);
#endif /* INC_CAN_H_ */
