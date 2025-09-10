/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATLYST.h"
#include"can.h"
#include"uart.h"
/* Per-message print helpers. Add or remove fields to match your structs. */
static void print_MCU_Stat_Two(const MCU_Stat_Two_t *m) {
    // Print precise formatting you like:
    uart_printf("MCU_Stat_Two: Odom=%.1f VCU=%.0f RPM=%.0f\r\n",
                m->MCU_Odometer_Val, m->MCU_VCU_State, m->MCU_Motor_RPM);
}

static void print_MCU_Fault_Code(const MCU_Fault_Code_t *m) {
    uart_printf("MCU_Fault_Code:");
    for (int i = 0; i < 24; ++i) {
        // field names in struct follow MCU_Fault_Code_0 ... _23
        double val = *((double*)m + i); // assumes fields are contiguous doubles
        uart_printf(" %u", (unsigned)val);
    }
    uart_printf("\r\n");
}

static void print_HearthBeat(const HearthBeat_t *m) {
    uart_printf("HeartBeat:");
    uart_printf(" s0=%.0f s1=%.0f s2=%.0f s3=%.0f s4=%.0f s5=%.0f s6=%.0f s7=%.0f s8=%.0f s9=%.0f s10=%.0f s11=%.0f\r\n",
                m->HeartBeat_Stat0, m->HeartBeat_Stat1, m->HeartBeat_Stat2, m->HeartBeat_Stat3,
                m->HeartBeat_Stat4, m->HeartBeat_Stat5, m->HeartBeat_Stat6, m->HeartBeat_Stat7,
                m->HeartBeat_Stat8, m->HeartBeat_Stat9, m->HeartBeat_Stat10, m->HeartBeat_Stat11);
}

static void print_CANFRAME3(const CANFRAME3_t *m) {
    uart_printf("CANFRAME3: s0=%.0f s1=%.0f s2=%.0f s3=%.0f s4=%.0f\r\n",
                m->CANFRAME3_sig0, m->CANFRAME3_sig1, m->CANFRAME3_sig2, m->CANFRAME3_sig3, m->CANFRAME3_sig4);
}
static void print_MCU_POWER_One(const MCU_POWER_One_t  *m) {
    uart_printf("POWER_ONE: s0=%.0f s1=%.0f s2=%.0f s3=%.0f s4=%.0f\r\n",
                m->power_sig0, m->power_sig1, m->power_sig2, m->power_sig3, m->power_sig4);
}
DBC_AllMessages_t msg;
void matel_mcu_process_can_frame(can_frame_t *frame){
	switch(frame->id){
	case Matel_MCU_POWER_CAN_STD_ID_A1:decode_MCU_POWER_One(frame->data,&msg.MCU_POWER_One);
	print_MCU_POWER_One(&msg.MCU_POWER_One);
		                                  break;
	case Matel_MCU_FAULT_one_CAN_STD_ID_AE:decode_MCU_FAULT_One(frame->data, &msg.MCU_FAULT_One);
		                                  break;
	case Matel_MCU_FAULT_two_CAN_STD_ID_AF:decode_MCU_FAULT_two(frame->data,&msg.MCU_FAULT_two);
		                                  break;
	case Matel_MCU_Fault_Code_CAN_STD_ID_B3:decode_MCU_Fault_Code(frame->data, &msg.MCU_Fault_Code);
    print_MCU_Fault_Code(&msg.MCU_Fault_Code);
		                                   break;
	case Matel_CANFRAME3_CAN_STD_ID_3AA:decode_CANFRAME3(frame->data, &msg.CANFRAME3);

		                                   break;
	case Matel_MCU_Stat_One_CAN_EXTD_ID_0x98A92000:decode_MCU_Stat_One(frame->data, &msg.MCU_Stat_One);
		                                  break;
	case Matel_MCU_Stat_Two_CAN_EXTD_ID_0x98A96000:decode_MCU_Stat_Two(frame->data, &msg.MCU_Stat_Two);
    print_MCU_Stat_Two(&msg.MCU_Stat_Two);
		                                       break;
	case Matel_MCU_HearthBeat_CAN_EXTD_ID_0x98F40117:decode_HearthBeat(frame->data, &msg.HearthBeat);
	print_HearthBeat(&msg.HearthBeat);
	break;
	case Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000:
		                                       break;
	}

}
