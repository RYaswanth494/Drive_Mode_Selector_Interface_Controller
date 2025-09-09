/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATLYST.h"
#include"can.h"
#include"uart.h"
Matel_MCU_Messages_t Mcu_frame;
void matel_mcu_process_can_frame(can_frame_t *frame){
	switch(frame->id){
	case Matel_MCU_POWER_CAN_STD_ID_A1:
		                                  break;
	case Matel_MCU_FAULT_one_CAN_STD_ID_AE:
		                                  break;
	case Matel_MCU_FAULT_two_CAN_STD_ID_AF:
		                                  break;
	case Matel_MCU_Fault_Code_CAN_STD_ID_B3:
		                                   break;
	case Matel_CANFRAME3_CAN_STD_ID_3AA:decode_CANFRAME3_0x3AA(frame->data,&Mcu_frame.canframe3);
    uart_printf("\n\r Battery_current=%d Battery_voltage=%d Battery_Dischrg_cur=%d Battery_chrg_cur=%d battery_state=%d\n\r",&Mcu_frame.canframe3.BATTERYCURRENT,&Mcu_frame.canframe3.BATTERYVOLTAGE,&Mcu_frame.canframe3.BATTERYVOLTAGE,&Mcu_frame.canframe3.MAXDISCHARGECURRENT,&Mcu_frame.canframe3.BATTERYSTATE);
		                                   break;
	case Matel_MCU_Stat_One_CAN_EXTD_ID_0x98A92000:decode_MCU_Stat_One_0x98A92000(frame->data,&Mcu_frame.mcu_stat_one);
		                                  break;
	case Matel_MCU_Stat_Two_CAN_EXTD_ID_0x98A96000:
		                                       break;
	case Matel_MCU_HearthBeat_CAN_EXTD_ID_0x98F40117:
		                                       break;
	case Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000:
		                                       break;
	}

}
