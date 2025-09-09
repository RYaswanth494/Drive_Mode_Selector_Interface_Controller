/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATLYST.h"
#include"can.h"
Matel_MCU_Messages_t Mcu_frame;
void mcu_can_messages_process(can_frame_t *frame){
	switch(frame->id){
	case Matel_MCU_POWER_CAN_STD_ID_A1:
		                                  break;
	case Matel_MCU_FAULT_CAN_STD_ID_AE:
		                                  break;
	case Matel_MCU_FAULT_two_CAN_STD_ID_AF:
		                                  break;
	case Matel_MCU_Fault_Code_CAN_STD_ID_B3:
		                                   break;
	case Matel_CANFRAME3_CAN_STD_ID_3AA:Decode_Matel_MCU_0X3AA(frame->data,&Mcu_frame.BMS_data);
		                                  break;
	case Matel_MCU_Stat_One_CAN_EXTD_ID_18265040:
		                                       break;
	case Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040:
		                                       break;
	case Matel_HearthBeat_CAN_EXTD_ID_18963257:
		                                       break;
	}
}
