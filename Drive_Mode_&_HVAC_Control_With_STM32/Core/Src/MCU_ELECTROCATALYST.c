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
		                                break;
	case Matel_MCU_FAULT_one_CAN_STD_ID_AE:decode_MCU_FAULT_One(frame->data, &msg.MCU_FAULT_One);
		                                  break;
	case Matel_MCU_FAULT_two_CAN_STD_ID_AF:decode_MCU_FAULT_two(frame->data,&msg.MCU_FAULT_two);
		                                  break;
	case Matel_MCU_Fault_Code_CAN_STD_ID_B3:decode_MCU_Fault_Code(frame->data, &msg.MCU_Fault_Code);
		                                   break;
	case Matel_CANFRAME3_CAN_STD_ID_3AA:decode_CANFRAME3(frame->data, &msg.CANFRAME3);
		                                   break;
	case Matel_MCU_Stat_One_CAN_EXTD_ID_18265040:decode_MCU_Stat_One(frame->data, &msg.MCU_Stat_One);
		                                  break;
	case Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040:decode_MCU_Stat_Two(frame->data, &msg.MCU_Stat_Two);
		                                       break;
	case Matel_MCU_HearthBeat_CAN_EXTD_ID_18963257:decode_HearthBeat(frame->data, &msg.HearthBeat);
	                                             break;
	case Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000:
		                                       break;
	}

}
