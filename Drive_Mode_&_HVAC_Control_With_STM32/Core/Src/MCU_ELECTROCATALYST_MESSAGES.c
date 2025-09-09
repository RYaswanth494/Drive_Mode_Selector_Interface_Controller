/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"uart.h"
#include"can.h"

#include <string.h>

// Decoding functions for each CAN message
// --------------------------------------------------------------------------------

void decode_VECTOR_INDEPENDENT_SIG_MSG_0xC0000000(const uint8_t *data, VECTOR_INDEPENDENT_SIG_MSG_t *msg) {
    int16_t raw_current = (int16_t)((data[1] << 8) | data[0]);
    msg->BATTEERYCURRENT = (float)raw_current * 0.01f - 500.0f;
    uint16_t raw_trq_vcu_map = ((data[1] & 0x07) << 8) | data[0];
    msg->Trq_VCU_MAP = (float)raw_trq_vcu_map * 0.1f - 100.0f;
    uint8_t raw_trq_comp = data[1];
    msg->Trq_comp = (float)raw_trq_comp * 0.1f - 10.0f;
    msg->uSD_CardStatus = data[1];
    msg->CAN_ChargerConnectionStatus = data[1];
    msg->Cell_1_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_2_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_3_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_4_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_5_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_6_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_7_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_8_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_9_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_10_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_11_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_12_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_13_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_14_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_15_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->Cell_16_BalanceStatus = (data[1] >> 0) & 0x01;
    msg->BMS_CycleCount = (data[1] << 8) | data[0];
    msg->FullChargeStatus = data[1];
    msg->CellBalancingTimeoutStatus = data[1];
    msg->SleepStatus = data[1];
    msg->HandBreakStatus = data[1];
    msg->ChargerPinDetection = data[1];
    msg->SOC = data[1];
}

void decode_MCU_Stat_Two_0x98A96000(const uint8_t *data, MCU_Stat_Two_t *msg) {
    int16_t raw_rpm = (data[1] << 8) | data[0];
    msg->MCU_Motor_RPM = raw_rpm - 16384;
    msg->MCU_Cap_Voltage = (data[3] << 8) | data[2];
    uint32_t raw_odo = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
    msg->MCU_Odometer_Val = (float)raw_odo * 0.1f;
}

void decode_MCU_Fault_Code_0xB3(const uint8_t *data, MCU_Fault_Code_t *msg) {
    msg->MCU_SensorSupply_Fault = (data[0] >> 0) & 0x01;
    msg->MCU_DC_CurrSensor_Fault = (data[0] >> 1) & 0x01;
    msg->MCU_Foc_Fault = (data[0] >> 2) & 0x01;
    msg->MCU_PH_Sensor_Fault = (data[0] >> 4) & 0x03;
    msg->MCU_OverCurrent_Fault = (data[0] >> 7) & 0x07;
    msg->MCU_temp_warning = (data[1] >> 0) & 0x01;
    msg->MCU_temp_sensor_fault = (data[1] >> 1) & 0x01;
    msg->VCU_LowSOC_Fault = (data[1] >> 2) & 0x01;
    msg->VCU_AuxVoltage_cutoff = (data[1] >> 3) & 0x01;
    msg->VCU_AuxVoltage_Warning = (data[1] >> 4) & 0x01;
    msg->MCU_Encoder_Fault = (data[1] >> 5) & 0x01;
    msg->MCU_FET_OPEN_FAULT = (data[1] >> 6) & 0x01;
    msg->MCU_WheelLock_Fault = (data[1] >> 7) & 0x01;
    msg->Motor_temp_warning = (data[2] >> 0) & 0x01;
    msg->Motor_temp_sensor_fault = (data[2] >> 1) & 0x01;
    msg->VCU_Brake_STG_Fault = (data[2] >> 2) & 0x01;
    msg->VCU_Brake_STB_Fault = (data[2] >> 3) & 0x01;
    msg->VCU_Throttle2_STG_Fault = (data[2] >> 4) & 0x01;
    msg->VCU_Throttle2_STB_Fault = (data[2] >> 5) & 0x01;
    msg->VCU_Throttle1_STB_Fault = (data[2] >> 6) & 0x01;
    msg->VCU_Throttle1_STG_Fault = (data[2] >> 7) & 0x01;
    msg->MCU_temp_cutoff = (data[3] >> 6) & 0x01;
    msg->motor_temp_cutoff_fault = (data[3] >> 7) & 0x01;
}

void decode_HearthBeat_0x98F40117(const uint8_t *data, HearthBeat_t *msg) {
    uint16_t raw_bat_volt = (data[1] << 8) | data[0];
    msg->BatteryVoltage = (float)raw_bat_volt * 0.01f;
    int16_t raw_current = (data[3] << 8) | data[2];
    msg->Current = (float)raw_current * 0.01f;
    int8_t raw_temp = data[4];
    msg->AvrgCellTemp = raw_temp - 40;
    uint8_t raw_volt = data[5];
    msg->AvrgCellVolt = (float)raw_volt * 0.1f;
    msg->CellLow = (data[7] >> 2) & 0x01;
    msg->IGN_S = (data[7] >> 1) & 0x01;
    msg->CellHigh = (data[7] >> 3) & 0x01;
    msg->BattHigh = (data[7] >> 4) & 0x01;
    msg->BattLow = (data[7] >> 5) & 0x01;
    msg->ChgFET = (data[7] >> 6) & 0x01;
    msg->DisFET = (data[7] >> 7) & 0x01;
    msg->SOC = data[6];
}

void decode_CANFRAME3_0x3AA(const uint8_t *data, CANFRAME3_t *msg) {
    int32_t raw_current = ((data[2] & 0x01) << 16) | (data[1] << 8) | data[0];
    msg->BATTERYCURRENT = (float)raw_current * 0.01f - 500.0f;
    uint16_t raw_voltage = ((data[3] & 0x03) << 8) | data[2];
    msg->BATTERYVOLTAGE = (float)raw_voltage * 0.1f;
    uint16_t raw_discharge = ((data[4] & 0x01) << 8) | data[3];
    msg->MAXDISCHARGECURRENT = raw_discharge;
    uint16_t raw_charge = ((data[5] & 0x01) << 8) | data[4];
    msg->MAXCHARGECURRENT = raw_charge;
    msg->BATTERYSTATE = data[5];
}

void decode_MCU_Stat_One_0x98A92000(const uint8_t *data, MCU_Stat_One_t *msg) {
    msg->MCU_PCB_Temp = data[0] - 40;
    msg->MCU_Motor_Temp = data[1] - 40;
    uint16_t raw_current = (data[3] << 8) | data[2];
    msg->MCU_RMS_Current = (float)raw_current * 0.1f;
    msg->MCU_Throttle_perc = data[4];
    msg->MCU_Brake_perc = data[5];
    msg->MCU_Speed_Kmph = data[6];
    msg->MCU_Drive_Mode = (data[7] >> 5) & 0x07;
}

void decode_MCU_FAULT_two_0xAF(const uint8_t *data, MCU_FAULT_two_t *msg) {
    msg->VCU_OverSpeed_Fault_St_B = (data[0] >> 0) & 0x01;
    msg->VCU_OverSpeed_Warning_St_B = (data[0] >> 1) & 0x01;
    msg->VCU_Brake_STB_St_B = (data[0] >> 2) & 0x01;
    msg->VCU_Brake_STG_St_B = (data[0] >> 3) & 0x01;
    msg->VCU_Throttle2_STB_St_B = (data[0] >> 4) & 0x01;
    msg->VCU_Throttle2_STG_St_B = (data[0] >> 5) & 0x01;
    msg->VCU_Throttle1_STB_St_B = (data[0] >> 6) & 0x01;
    msg->VCU_Throttle1_STG_St_B = (data[0] >> 7) & 0x01;
    msg->VCU_BFNRFault_St_B = (data[1] >> 1) & 0x07;
    msg->VCU_DCDCEnableFault_St_B = (data[1] >> 2) & 0x01;
    msg->VCU_ThrottleMismatch_St_B = (data[1] >> 3) & 0x01;
    msg->VCU_LowSOC_Fault_St_B = (data[1] >> 4) & 0x01;
    msg->VCU_LowSOC_Warning_St_B = (data[1] >> 5) & 0x01;
    msg->VCU_AuxBatteryUV_Fault_St_B = (data[1] >> 6) & 0x01;
    msg->VCU_AuxVoltageUV_Warning_St_B = (data[1] >> 7) & 0x01;
}

void decode_MCU_FAULT_One_0xAE(const uint8_t *data, MCU_FAULT_One_t *msg) {
    msg->MCU_PhaseSensorFault_enum = (data[0] >> 1) & 0x03;
    msg->MCU_VCUFault_St_B = (data[0] >> 2) & 0x01;
    msg->MCU_ChargerFault_St_B = (data[0] >> 3) & 0x01;
    msg->MCU_BatteryUV_St_B = (data[0] >> 4) & 0x01;
    msg->MCU_BatOVFault_St_B = (data[0] >> 5) & 0x01;
    msg->MCU_SpeedFault_St_B = (data[0] >> 6) & 0x01;
    msg->MCU_FOCFault_St_B = (data[0] >> 7) & 0x01;
    msg->MCU_SensorSupplyFault_enum = (data[1] >> 1) & 0x03;
    msg->MCU_PhFault_enum = (data[1] >> 3) & 0x03;
    msg->MCU_EncoderFault_enum = (data[1] >> 7) & 0x0F;
    msg->MCU_PhaseISensorFault_enum = (data[2] >> 2) & 0x3F;
    msg->MCU_OverCurrentFault_enum = (data[2] >> 5) & 0x07;
    msg->MCU_DC_ISensorFault_St_B = (data[2] >> 7) & 0x03;
    msg->HW_ASC_st_b = (data[3] >> 3) & 0x01;
    msg->MCU_FET_OPEN_FAULT = (data[3] >> 4) & 0x01;
    msg->MCU_DCDCCONV_Fault_St_B = (data[4] >> 7) & 0x01;
}

void decode_MCU_POWER_One_0XA1(const uint8_t *data, MCU_POWER_One_t *msg) {
    uint16_t raw_voltage = (data[1] >> 7) & 0x03;
    raw_voltage = (raw_voltage << 8) | data[0];
    msg->MCU_DCCapVoltage_Act_V = (float)raw_voltage * 0.1f;
    int16_t raw_current = (data[2] >> 5) & 0x07;
    raw_current = (raw_current << 8) | data[1];
    raw_current = (raw_current << 5) >> 5;
    msg->MCU_DCInstCurrent_Act_A = (float)raw_current * 0.1f - 409.6f;
    int16_t raw_torque = (data[3] >> 6) & 0x03;
    raw_torque = (raw_torque << 8) | data[2];
    raw_torque = (raw_torque << 6) >> 6;
    msg->MCU_EstTorqueVal_Nm = (float)raw_torque * 0.1f - 65.0f;
    uint16_t raw_req_torque = (data[4] >> 6) & 0x03;
    raw_req_torque = (raw_req_torque << 8) | data[3];
    msg->MCU_FinalRequestedTrq_Nm = (float)raw_req_torque * 0.1f - 37.3f;
    int16_t raw_speed = (data[6] >> 1) & 0x3F;
    raw_speed = (raw_speed << 8) | data[5];
    raw_speed = (raw_speed << 1) >> 1;
    msg->MCU_MotorActSpeed_RPM = raw_speed - 16384;
}
void mcu_can_messages(){
}
