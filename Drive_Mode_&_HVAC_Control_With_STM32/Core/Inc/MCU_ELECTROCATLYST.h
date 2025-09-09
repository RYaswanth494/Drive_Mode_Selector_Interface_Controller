/*
 * MCU_ELECTROCATLYST.h
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_MCU_ELECTROCATLYST_H_
#define INC_MCU_ELECTROCATLYST_H_

#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"can.h"
typedef struct{
    VECTOR_INDEPENDENT_SIG_MSG_t vector_independent_sig_msg;
    MCU_Stat_Two_t mcu_stat_two;
    MCU_Fault_Code_t mcu_fault_code;
    HearthBeat_t hearthbeat;
    CANFRAME3_t canframe3;
    MCU_Stat_One_t mcu_stat_one;
    MCU_FAULT_two_t mcu_fault_two;
    MCU_FAULT_One_t mcu_fault_one;
    MCU_POWER_One_t mcu_power_one;
}Matel_MCU_Messages_t;
void matel_mcu_process_can_frame(can_frame_t *frame);
#endif /* INC_MCU_ELECTROCATLYST_H_ */
