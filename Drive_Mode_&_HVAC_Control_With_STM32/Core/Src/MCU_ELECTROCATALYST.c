/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATLYST.h"
#include"can.h"
#include"uart.h"

DBC_AllMessages_t msg;
void matel_mcu_process_can_frame(can_frame_t *frame){
	switch(frame->id){
	case Matel_MCU_POWER_CAN_STD_ID_A1:decode_MCU_POWER_One(frame->data,&msg.MCU_POWER_One);
	                              msg.MCU_POWER_One.rx_or_not=1;
		                                break;
	case Matel_MCU_FAULT_one_CAN_STD_ID_AE:decode_MCU_FAULT_One(frame->data, &msg.MCU_FAULT_One);
                                  msg.MCU_FAULT_One.rx_or_not=1;
		                                  break;
	case Matel_MCU_FAULT_two_CAN_STD_ID_AF:decode_MCU_FAULT_two(frame->data,&msg.MCU_FAULT_two);
                                   msg.MCU_FAULT_two.rx_or_not=1;
		                                  break;
	case Matel_MCU_Fault_Code_CAN_STD_ID_B3:decode_MCU_Fault_Code(frame->data, &msg.MCU_Fault_Code);
                                    msg.MCU_Fault_Code.rx_or_not=1;
		                                   break;
	case Matel_MCU_Stat_One_CAN_EXTD_ID_18265040:decode_MCU_Stat_One(frame->data, &msg.MCU_Stat_One);
                                     msg.MCU_Stat_One.rx_or_not=1;
		                                  break;
	case Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040:decode_MCU_Stat_Two(frame->data, &msg.MCU_Stat_Two);
                                 msg.MCU_Stat_Two.rx_or_not=1;
		                                       break;

	}

}
