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
#ifndef CAN_MESSAGES_H
#define CAN_MESSAGES_H

#include <stdint.h>

// Typedef structures for each CAN message
// --------------------------------------------------------------------------------

typedef struct {
    float Trq_VCU_MAP;
    float Trq_comp;
    uint8_t uSD_CardStatus;
    uint8_t CAN_ChargerConnectionStatus;
    uint8_t Cell_1_BalanceStatus;
    uint8_t Cell_2_BalanceStatus;
    uint8_t Cell_3_BalanceStatus;
    uint8_t Cell_4_BalanceStatus;
    uint8_t Cell_5_BalanceStatus;
    uint8_t Cell_6_BalanceStatus;
    uint8_t Cell_7_BalanceStatus;
    uint8_t Cell_8_BalanceStatus;
    uint8_t Cell_9_BalanceStatus;
    uint8_t Cell_10_BalanceStatus;
    uint8_t Cell_11_BalanceStatus;
    uint8_t Cell_12_BalanceStatus;
    uint8_t Cell_13_BalanceStatus;
    uint8_t Cell_14_BalanceStatus;
    uint8_t Cell_15_BalanceStatus;
    uint8_t Cell_16_BalanceStatus;
    uint16_t BMS_CycleCount;
    uint8_t FullChargeStatus;
    uint8_t CellBalancingTimeoutStatus;
    uint8_t SleepStatus;
    uint8_t HandBreakStatus;
    uint8_t ChargerPinDetection;
    uint8_t SOC;
    float BATTEERYCURRENT;
} VECTOR_INDEPENDENT_SIG_MSG_t;

typedef struct {
    float MCU_Odometer_Val;
    uint16_t MCU_Cap_Voltage;
    int16_t MCU_Motor_RPM;
} MCU_Stat_Two_t;

typedef struct {
    uint8_t MCU_temp_cutoff;
    uint8_t motor_temp_cutoff_fault;
    uint8_t Motor_temp_warning;
    uint8_t Motor_temp_sensor_fault;
    uint8_t VCU_Brake_STG_Fault;
    uint8_t VCU_Brake_STB_Fault;
    uint8_t VCU_Throttle2_STG_Fault;
    uint8_t VCU_Throttle2_STB_Fault;
    uint8_t VCU_Throttle1_STB_Fault;
    uint8_t VCU_Throttle1_STG_Fault;
    uint8_t MCU_temp_warning;
    uint8_t MCU_temp_sensor_fault;
    uint8_t VCU_LowSOC_Fault;
    uint8_t VCU_AuxVoltage_cutoff;
    uint8_t VCU_AuxVoltage_Warning;
    uint8_t MCU_Encoder_Fault;
    uint8_t MCU_FET_OPEN_FAULT;
    uint8_t MCU_WheelLock_Fault;
    uint8_t MCU_SensorSupply_Fault;
    uint8_t MCU_OverCurrent_Fault;
    uint8_t MCU_DC_CurrSensor_Fault;
    uint8_t MCU_Foc_Fault;
    uint8_t MCU_PH_Sensor_Fault;
} MCU_Fault_Code_t;

typedef struct {
    float Current;
    float BatteryVoltage;
    int8_t AvrgCellTemp;
    float AvrgCellVolt;
    uint8_t SOC;
    uint8_t DisFET;
    uint8_t ChgFET;
    uint8_t BattLow;
    uint8_t BattHigh;
    uint8_t CellHigh;
    uint8_t IGN_S;
    uint8_t CellLow;
} HearthBeat_t;

typedef struct {
    float BATTERYCURRENT;
    uint16_t MAXCHARGECURRENT;
    uint8_t BATTERYSTATE;
    uint16_t MAXDISCHARGECURRENT;
    float BATTERYVOLTAGE;
} CANFRAME3_t;

typedef struct {
    int8_t MCU_Motor_Temp;
    int8_t MCU_PCB_Temp;
    float MCU_RMS_Current;
    uint8_t MCU_Drive_Mode;
    uint8_t MCU_Speed_Kmph;
    uint8_t MCU_Brake_perc;
    uint8_t MCU_Throttle_perc;
} MCU_Stat_One_t;

typedef struct {
    uint8_t VCU_BFNRFault_St_B;
    uint8_t VCU_DCDCEnableFault_St_B;
    uint8_t VCU_ThrottleMismatch_St_B;
    uint8_t VCU_LowSOC_Fault_St_B;
    uint8_t VCU_LowSOC_Warning_St_B;
    uint8_t VCU_AuxBatteryUV_Fault_St_B;
    uint8_t VCU_AuxVoltageUV_Warning_St_B;
    uint8_t VCU_OverSpeed_Fault_St_B;
    uint8_t VCU_OverSpeed_Warning_St_B;
    uint8_t VCU_Brake_STB_St_B;
    uint8_t VCU_Brake_STG_St_B;
    uint8_t VCU_Throttle2_STB_St_B;
    uint8_t VCU_Throttle2_STG_St_B;
    uint8_t VCU_Throttle1_STB_St_B;
    uint8_t VCU_Throttle1_STG_St_B;
} MCU_FAULT_two_t;

typedef struct {
    uint8_t HW_ASC_st_b;
    uint8_t MCU_FET_OPEN_FAULT;
    uint8_t MCU_DCDCCONV_Fault_St_B;
    uint8_t MCU_BatteryUV_St_B;
    uint8_t MCU_VCUFault_St_B;
    uint8_t MCU_PhaseISensorFault_enum;
    uint8_t MCU_SpeedFault_St_B;
    uint8_t MCU_SensorSupplyFault_enum;
    uint8_t MCU_PhFault_enum;
    uint8_t MCU_PhaseSensorFault_enum;
    uint8_t MCU_OverCurrentFault_enum;
    uint8_t MCU_FOCFault_St_B;
    uint8_t MCU_EncoderFault_enum;
    uint8_t MCU_DC_ISensorFault_St_B;
    uint8_t MCU_ChargerFault_St_B;
    uint8_t MCU_BatOVFault_St_B;
} MCU_FAULT_One_t;

typedef struct {
    float MCU_FinalRequestedTrq_Nm;
    float MCU_DCCapVoltage_Act_V;
    float MCU_EstTorqueVal_Nm;
    int16_t MCU_MotorActSpeed_RPM;
    float MCU_DCInstCurrent_Act_A;
} MCU_POWER_One_t;

#endif
// CAN_MESSAGES_H
// Function prototypes for each CAN message decoder
// --------------------------------------------------------------------------------

void decode_VECTOR_INDEPENDENT_SIG_MSG_0xC0000000(const uint8_t *data, VECTOR_INDEPENDENT_SIG_MSG_t *msg);
void decode_MCU_Stat_Two_0x98A96000(const uint8_t *data, MCU_Stat_Two_t *msg);
void decode_MCU_Fault_Code_0xB3(const uint8_t *data, MCU_Fault_Code_t *msg);
void decode_HearthBeat_0x98F40117(const uint8_t *data, HearthBeat_t *msg);
void decode_CANFRAME3_0x3AA(const uint8_t *data, CANFRAME3_t *msg);
void decode_MCU_Stat_One_0x98A92000(const uint8_t *data, MCU_Stat_One_t *msg);
void decode_MCU_FAULT_two_0xAF(const uint8_t *data, MCU_FAULT_two_t *msg);
void decode_MCU_FAULT_One_0xAE(const uint8_t *data, MCU_FAULT_One_t *msg);
void decode_MCU_POWER_One_0XA1(const uint8_t *data, MCU_POWER_One_t *msg);
#endif /* INC_MCU_ELECTROCATALYST_MESSAGES_H_ */
