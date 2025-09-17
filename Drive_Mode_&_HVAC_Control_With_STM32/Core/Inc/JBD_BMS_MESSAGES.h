/*
 * JBD_BMS_MESSAGES.h
 *
 *  Created on: Sep 17, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_JBD_BMS_MESSAGES_H_
#define INC_JBD_BMS_MESSAGES_H_

#include"main.h"
#include<stdbool.h>
#define JBD_BMS_ID0 0x100
#define JBD_BMS_ID1 0x101
#define JBD_BMS_ID2 0x102
#define JBD_BMS_ID3 0x103
#define JBD_BMS_ID4 0x104
#define JBD_BMS_ID5 0x105
#define JBD_BMS_ID6 0x106
#define JBD_BMS_ID7 0x107
#define JBD_BMS_ID8 0x108
#define JBD_BMS_ID9 0x109
#define JBD_BMS_ID10 0x10A
#define JBD_BMS_ID11 0x10B
#define JBD_BMS_ID12 0x10C
#define JBD_BMS_ID13 0x10D
#define JBD_BMS_ID14 0x10E
#define JBD_BMS_ID15 0x10F
#define JBD_BMS_ID16 0x110

typedef struct{
	float Total_Voltage;
	float Charge_and_Discharge_Current;
	float Remaining_Capacity;
	uint16_t CRC_Check;
}CAN_BMS_0x100_t;

typedef struct{
	float Full_Capacity;
	uint16_t  No_Cycles;
	uint16_t RSOC;
	uint16_t CRC_Check;
}CAN_BMS_0x101_t;

typedef struct{
	bool Equlization_Low_byte[33];
	uint16_t Protection_Status;
	uint16_t CRC_Check;
}CAN_BMS_0x102_t;

typedef struct{
	uint16_t FET_Control_Status;
	short int Production_Date;
	uint16_t Software_Version;
	uint16_t CRC_Check;
}CAN_BMS_0x103_t;

typedef struct{
	uint8_t Number_Of_Battery_Strings;
	uint8_t NTC_Number_Probes;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x104_t;

typedef struct{
	float NTC1;
	float NTC2;
	float NTC3;
	float CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x105_t;

typedef struct{
	float NTC4;
	float NTC5;
	float NTC6;
	float CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x106_t;

typedef struct{
	uint16_t CELL1;
	uint16_t  CELL2;
	uint16_t CELL3;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x107_t;

typedef struct{
	uint16_t CELL4;
	uint16_t  CELL5;
	uint16_t CELL6;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x108_t;

typedef struct{
	uint16_t CELL7;
	short int CELL8;
	uint16_t CELL6;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x109_t;

typedef struct{
	uint16_t CELL10;
	uint16_t  CELL11;
	uint16_t CELL12;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10A_t;

typedef struct{
	uint16_t CELL13;
	uint16_t  CELL14;
	uint16_t CELL15;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10B_t;

typedef struct{
	uint16_t CELL16;
	uint16_t  CELL17;
	uint16_t CELL18;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10C_t;

typedef struct{
	uint16_t CELL19;
	uint16_t  CELL20;
	uint16_t CELL21;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10D_t;

typedef struct{
	uint16_t CELL22;
	uint16_t  CELL23;
	uint16_t CELL24;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10E_t;

typedef struct{
	uint16_t CELL25;
	uint16_t  CELL26;
	uint16_t CELL27;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x10F_t;

typedef struct{
	uint16_t CELL28;
	uint16_t  CELL29;
	uint16_t CELL30;
	uint16_t CRC_Check;
	 uint8_t rx_or_not:1;
}CAN_BMS_0x110_t;
typedef struct {
    uint16_t cell_voltage[33]; // Array to store all 33 cell voltages
    uint16_t crc_check;
} CAN_BMS_Cell_Voltages_t;

void decode_CAN_0x100(const uint8_t *data, CAN_BMS_0x100_t *msg);
void decode_CAN_0x102(const uint8_t *data, CAN_BMS_0x102_t *msg);
void decode_CAN_0x103(const uint8_t *data, CAN_BMS_0x103_t *msg);
void decode_CAN_0x104(const uint8_t *data, CAN_BMS_0x104_t *msg);
void decode_CAN_0x105(const uint8_t *data, CAN_BMS_0x105_t *msg);
void decode_CAN_0x106(const uint8_t *data, CAN_BMS_0x106_t *msg);
void decode_CAN_0x107(const uint8_t *data, CAN_BMS_0x107_t *msg);
void decode_CAN_0x108(const uint8_t *data, CAN_BMS_0x108_t *msg);
void decode_CAN_0x109(const uint8_t *data, CAN_BMS_0x109_t *msg);
void decode_CAN_0x10A(const uint8_t *data, CAN_BMS_0x10A_t *msg);
void decode_CAN_0x10B(const uint8_t *data, CAN_BMS_0x10B_t *msg);
void decode_CAN_0x10C(const uint8_t *data, CAN_BMS_0x10C_t *msg);
void decode_CAN_0x10D(const uint8_t *data, CAN_BMS_0x10D_t *msg);
void decode_CAN_0x10E(const uint8_t *data, CAN_BMS_0x10E_t *msg);
void decode_CAN_Voltage(const uint8_t *data, uint16_t msg_id, CAN_BMS_Cell_Voltages_t *msg);
void decode_CAN_0x10F(const uint8_t *data, CAN_BMS_0x10F_t *msg);
void decode_CAN_0x110(const uint8_t *data, CAN_BMS_0x110_t *msg);

#endif /* INC_JBD_BMS_MESSAGES_H_ */
