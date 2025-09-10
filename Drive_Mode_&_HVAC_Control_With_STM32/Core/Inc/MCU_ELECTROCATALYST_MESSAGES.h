/*
 * MCU_ELECTROCATALYST_MESSAGES.h
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_MCU_ELECTROCATALYST_MESSAGES_H_
#define INC_MCU_ELECTROCATALYST_MESSAGES_H_

#include<stdint.h>
#include<stdbool.h>
/*****************************************************************
 *                       CAN ID'S  RELATED MCU                   *
 ****************************************************************/
/*---------------------------STD_ID's---------------------------*/
#define Matel_MCU_POWER_CAN_STD_ID_A1                0xA1  //MCU to CONTROLLER( decimal 161)
#define Matel_MCU_FAULT_one_CAN_STD_ID_AE            0xAE  //MCU to CONTROLLER(decimal 174)
#define Matel_MCU_FAULT_two_CAN_STD_ID_AF            0xAF  //MCU to CONTROLLER(decimal175)
#define Matel_MCU_Fault_Code_CAN_STD_ID_B3           0xB3  //MCU to CONTROLLER(decimal 179)
#define Matel_CANFRAME3_CAN_STD_ID_3AA               0x3AA  //MCU to CONTROLLER(decimal 938)
/*---------------------------EXTD_ID's---------------------------*/
#define Matel_MCU_Stat_One_CAN_EXTD_ID_0x98A92000       0x98A92000 //MCU to CONTROLLER(decimal 2552647744)
#define Matel_MCU_Stat_Two_CAN_EXTD_ID_0x98A96000       0x98A96000 //MCU to CONTROLLER(decimal 2552713280)
#define Matel_MCU_HearthBeat_CAN_EXTD_ID_0x98F40117     0x98F40117 //MCU to CONTROLLER(decimal 2559980119)
#define Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000    0xC0000000
/**********************************************************************
 *
 * ********************************************************************/


#include <stdint.h>

// Typedef structures for each CAN message
#include <stdint.h>

// Message: MCU_Stat_Two
typedef struct {
double MCU_Odometer_Val; // scaled
double MCU_VCU_State;
double MCU_Motor_RPM;
} MCU_Stat_Two_t;


// Message: MCU_Fault_Code
typedef struct {
double MCU_Fault_Code_0;
double MCU_Fault_Code_1;
double MCU_Fault_Code_2;
double MCU_Fault_Code_3;
double MCU_Fault_Code_4;
double MCU_Fault_Code_5;
double MCU_Fault_Code_6;
double MCU_Fault_Code_7;
double MCU_Fault_Code_8;
double MCU_Fault_Code_9;
double MCU_Fault_Code_10;
double MCU_Fault_Code_11;
double MCU_Fault_Code_12;
double MCU_Fault_Code_13;
double MCU_Fault_Code_14;
double MCU_Fault_Code_15;
double MCU_Fault_Code_16;
double MCU_Fault_Code_17;
double MCU_Fault_Code_18;
double MCU_Fault_Code_19;
double MCU_Fault_Code_20;
double MCU_Fault_Code_21;
double MCU_Fault_Code_22;
double MCU_Fault_Code_23;
} MCU_Fault_Code_t;


// Message: HearthBeat
typedef struct {
double HeartBeat_Stat0;
double HeartBeat_Stat1;
double HeartBeat_Stat2;
double HeartBeat_Stat3;
double HeartBeat_Stat4;
double HeartBeat_Stat5;
double HeartBeat_Stat6;
double HeartBeat_Stat7;
double HeartBeat_Stat8;
double HeartBeat_Stat9;
double HeartBeat_Stat10;
double HeartBeat_Stat11;
} HearthBeat_t;


// Message: CANFRAME3
typedef struct {
double CANFRAME3_sig0;
double CANFRAME3_sig1;
double CANFRAME3_sig2;
double CANFRAME3_sig3;
double CANFRAME3_sig4;
} CANFRAME3_t;
// Message: MCU_Stat_One
typedef struct {
double MCU_Stat_One_sig0;
double MCU_Stat_One_sig1;
double MCU_Stat_One_sig2;
double MCU_Stat_One_sig3;
double MCU_Stat_One_sig4;
double MCU_Stat_One_sig5;
double MCU_Stat_One_sig6;
} MCU_Stat_One_t;
// Message: MCU_FAULT_two
typedef struct {
double MCU_FAULT_two_sig0;
double MCU_FAULT_two_sig1;
double MCU_FAULT_two_sig2;
double MCU_FAULT_two_sig3;
double MCU_FAULT_two_sig4;
double MCU_FAULT_two_sig5;
double MCU_FAULT_two_sig6;
double MCU_FAULT_two_sig7;
double MCU_FAULT_two_sig8;
double MCU_FAULT_two_sig9;
double MCU_FAULT_two_sig10;
double MCU_FAULT_two_sig11;
double MCU_FAULT_two_sig12;
double MCU_FAULT_two_sig13;
double MCU_FAULT_two_sig14;
} MCU_FAULT_two_t;


// Message: MCU_FAULT_One
typedef struct {
double MCU_FAULT_One_sig0;
double MCU_FAULT_One_sig1;
double MCU_FAULT_One_sig2;
double MCU_FAULT_One_sig3;
double MCU_FAULT_One_sig4;
double MCU_FAULT_One_sig5;
double MCU_FAULT_One_sig6;
double MCU_FAULT_One_sig7;
double MCU_FAULT_One_sig8;
double MCU_FAULT_One_sig9;
double MCU_FAULT_One_sig10;
double MCU_FAULT_One_sig11;
double MCU_FAULT_One_sig12;
double MCU_FAULT_One_sig13;
double MCU_FAULT_One_sig14;
double MCU_FAULT_One_sig15;
} MCU_FAULT_One_t;


// Message: MCU_POWER_One
typedef struct {
double power_sig0;
double power_sig1;
double power_sig2;
double power_sig3;
double power_sig4;
} MCU_POWER_One_t;

// Prototypes for decode functions
void decode_MCU_Stat_Two(const uint8_t *data, MCU_Stat_Two_t *out);
void decode_MCU_Fault_Code(const uint8_t *data, MCU_Fault_Code_t *out);
void decode_HearthBeat(const uint8_t *data, HearthBeat_t *out);
void decode_CANFRAME3(const uint8_t *data, CANFRAME3_t *out);
void decode_MCU_Stat_One(const uint8_t *data, MCU_Stat_One_t *out);
void decode_MCU_FAULT_two(const uint8_t *data, MCU_FAULT_two_t *out);
void decode_MCU_FAULT_One(const uint8_t *data, MCU_FAULT_One_t *out);
void decode_MCU_POWER_One(const uint8_t *data, MCU_POWER_One_t *out);


#endif /* INC_MCU_ELECTROCATALYST_MESSAGES_H_ */
