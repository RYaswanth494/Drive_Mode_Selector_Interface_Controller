/*
 * MCU_ELECTROCATALYST_MESSAGES.h
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_MCU_ELECTROCATALYST_MESSAGES_H_
#define INC_MCU_ELECTROCATALYST_MESSAGES_H_

#include<stdint.h>

/*****************************************************************
 *                       CAN ID'S  RELATED MCU                   *
 ****************************************************************/
/*---------------------------STD_ID's---------------------------*/
#define Matel_MCU_POWER_CAN_STD_ID_A1                0xA1  //MCU to CONTROLLER( decimal 161)
#define Matel_MCU_FAULT_CAN_STD_ID_AE                0xAE  //MCU to CONTROLLER(decimal 174)
#define Matel_MCU_FAULT_two_CAN_STD_ID_AF            0xAF  //MCU to CONTROLLER(decimal175)
#define Matel_MCU_Fault_Code_CAN_STD_ID_B3           0xB3  //MCU to CONTROLLER(decimal 179)
#define Matel_CANFRAME3_CAN_STD_ID_3AA               0x3AA  //MCU to CONTROLLER(decimal 938)
/*---------------------------EXTD_ID's---------------------------*/
#define Matel_MCU_Stat_One_CAN_EXTD_ID_18265040       0X18265040 //MCU to CONTROLLER(decimal 405164096)
#define Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040       0X18275040 //MCU to CONTROLLER(decimal 405229632)
#define Matel_HearthBeat_CAN_EXTD_ID_18963257         0X18963257 //MCU to CONTROLLER(decimal 412496471)
/**********************************************************************
 *
 * ********************************************************************/
typedef struct{
	uint32_t BMS_CURRENT;
	uint32_t MAX_CHARGE_CURRENT;
	uint32_t BATTERY_STATE;
	uint32_t MAX_DISCHARGECURRENT;
	uint32_t BATTERY_VOLTAGE;
}Matel_MCU_0x3AA_t;
// ========== FUNCTION PROTOTYPES ==========
// Decode functions (MCU is receive-only)
void Decode_Matel_MCU_0X3AA(const uint8_t *data, Matel_MCU_0x3AA_t *msg);

#endif /* INC_MCU_ELECTROCATALYST_MESSAGES_H_ */
