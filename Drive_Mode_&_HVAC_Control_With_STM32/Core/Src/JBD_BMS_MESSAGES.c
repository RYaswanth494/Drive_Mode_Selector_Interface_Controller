/*
 * JBD_BMS_MESSAGES.c
 *
 *  Created on: Sep 17, 2025
 *      Author: MadaYaswanth
 */


#include"JBD_BMS_MESSAGES.h"
#include"uart.h"
void decode_CAN_0x100(const uint8_t *data, CAN_BMS_0x100_t *msg) {
    msg->Total_Voltage = ((data[0] << 8) | data[1]) * 0.01f;   // 10 mV → V
    msg->Charge_and_Discharge_Current =(float)((int16_t)((data[2] << 8) | data[3])) * 0.01f;  // 10 mA → A
    msg->Remaining_Capacity= (float)(((int16_t)(data[4] << 8) | data[5])) * 0.01f; // 10 mAh → Ah
    msg->CRC_Check   = (data[6] << 8) | data[7];
    uart_printf("Total_Voltage=%.2f Vol Charge_and_Discharge_Current=%.2f A Remaining_Capacity=%.2f Ah\n\r",msg->Total_Voltage,msg->Charge_and_Discharge_Current,msg->Remaining_Capacity);
}
void decode_CAN_0x101(const uint8_t *data, CAN_BMS_0x101_t *msg) {
    msg->Full_Capacity = ((data[0] << 8) | data[1]) * 0.01f;  // 10 mAh → Ah
    msg->No_Cycles        = (data[2] << 8) | data[3];
    msg->RSOC          = (uint8_t)(((data[4] << 8) | data[5]) & 0xFF); // RSOC in %
    msg->CRC_Check        = (data[6] << 8) | data[7];
    uart_printf("Full_Capacity=%.2f Ah RSOC=%d %\n\r",msg->Full_Capacity,msg->RSOC);

}
void decode_CAN_Voltage(const uint8_t *data, uint16_t msg_id, CAN_BMS_Cell_Voltages_t *msg) {
    uint8_t start_cell;

    if (msg_id == 0x109) {
        start_cell = 6;
    } else if (msg_id == 0x10A) {
        start_cell = 10;
    } else {
        start_cell = (msg_id - 0x107) * 3 + 1;
    }

    msg->cell_voltage[start_cell - 1] = (data[0] << 8) | data[1];
    msg->cell_voltage[start_cell] = (data[2] << 8) | data[3];
    msg->cell_voltage[start_cell + 1] = (data[4] << 8) | data[5];
    msg->crc_check = (data[6] << 8) | data[7];
}
void decode_CAN_0x103(const uint8_t *data, CAN_BMS_0x103_t *msg) {
    // Decode MOS tube status (Bytes 0-1)
    msg->FET_Control_Status = (data[0] << 8) | data[1];
    // Decode Production date (Bytes 2-3)
    msg->Production_Date = (data[2] << 8) | data[3];
    // Decode Software version (Bytes 4-5)
    msg->Software_Version = (data[4] << 8) | data[5];
    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x102(const uint8_t *data, CAN_BMS_0x102_t *msg) {
    // Decode equalization status for cells 1-16 (BYTE0 - BYTE1)
    uint16_t status_low_byte = (data[0] << 8) | data[1];
    for (int i = 0; i < 16; ++i) {
        msg->Equlization_Low_byte[i] = (status_low_byte >> i) & 1;
    }
    // Decode equalization status for cells 17-33 (BYTE2 - BYTE3)
    uint16_t status_high_byte = (data[2] << 8) | data[3];
    for (int i = 0; i < 17; ++i) {
        msg->Equlization_Low_byte[16 + i] = (status_high_byte >> i) & 1;
    }
    // Decode protection status (BYTE4 - BYTE5)
    msg->Protection_Status = (data[4] << 8) | data[5];
    // Decode CRC_16 check value (BYTE6 - BYTE7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x104(const uint8_t *data, CAN_BMS_0x104_t *msg) {
    // Decode Number of battery strings (BYTE0)
    msg->Number_Of_Battery_Strings = data[0];
    // Decode Number of NTC probes (BYTE1)
    msg->NTC_Number_Probes = data[1];
    // Decode CRC_16 check value (Bytes 2-3)
    // Assuming a 16-bit CRC value from bytes 2 and 3
    msg->CRC_Check = (data[2] << 8) | data[3];
}
void decode_CAN_0x105(const uint8_t *data, CAN_BMS_0x105_t *msg) {
    // Decode temperature value of NTC1 (Bytes 0-1)
    // The raw value is in 0.1K.
    // Convert to Celsius: (Value * 0.1) - 273.15
    uint16_t raw_ntc1_temp = (data[0] << 8) | data[1];
    msg->NTC1 = (raw_ntc1_temp * 0.1f) - 273.15f;
    // Decode temperature value of NTC2 (Bytes 2-3)
    uint16_t raw_ntc2_temp = (data[2] << 8) | data[3];
    msg->NTC2 = (raw_ntc2_temp * 0.1f) - 273.15f;
    // Decode temperature value of NTC3 (Bytes 4-5)
    uint16_t raw_ntc3_temp = (data[4] << 8) | data[5];
    msg->NTC3 = (raw_ntc3_temp * 0.1f) - 273.15f;
    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x106(const uint8_t *data, CAN_BMS_0x106_t *msg) {
    // Decode temperature value of NTC4 (Bytes 0-1)
    // The raw value is in 0.1K.
    // Convert to Celsius: (Value * 0.1) - 273.15
    uint16_t raw_ntc4_temp = (data[0] << 8) | data[1];
    msg->NTC4 = (raw_ntc4_temp * 0.1f) - 273.15f;
    // Decode temperature value of NTC5 (Bytes 2-3)
    uint16_t raw_ntc5_temp = (data[2] << 8) | data[3];
    msg->NTC5 = (raw_ntc5_temp * 0.1f) - 273.15f;
    // Decode temperature value of NTC6 (Bytes 4-5)
    uint16_t raw_ntc6_temp = (data[4] << 8) | data[5];
    msg->NTC6 = (raw_ntc6_temp * 0.1f) - 273.15f;
    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x107(const uint8_t *data, CAN_BMS_0x107_t *msg) {
    // Decode Cell1 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL1 = (float)((data[0] << 8) | data[1])/1000.f;
    // Decode Cell2 voltage (Bytes 2-3)
    msg->CELL2 = (float)((data[2] << 8) | data[3])/1000.0f;
    // Decode Cell3 voltage (Bytes 4-5)
    msg->CELL3 = (float)((data[4] << 8) | data[5])/1000.0f;
    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL1=%.2f v CELL2=%.2f v CELL3=%.2f v \n\r",msg->CELL1,msg->CELL2,msg->CELL3);
}

void decode_CAN_0x108(const uint8_t *data, CAN_BMS_0x108_t *msg) {
    // Decode Cell4 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL4 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell5 voltage (Bytes 2-3)
    msg->CELL5 =(float)((data[2] << 8) | data[3])/1000.0f;
    // Decode Cell6 voltage (Bytes 4-5)
    msg->CELL6 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL4=%.2f v CELL5=%.2f v CELL6=%.2f v \n\r",msg->CELL4,msg->CELL5,msg->CELL6);
}
void decode_CAN_0x109(const uint8_t *data, CAN_BMS_0x109_t *msg) {
    // Decode Cell6 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL7 = (float)((data[0] << 8) | data[1])/1000.f;
    // Decode Cell7 voltage (Bytes 2-3)
    msg->CELL8 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell8 voltage (Bytes 4-5)
    msg->CELL9 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL7=%.2f v CELL8=%.2f v CELL9=%.2f v \n\r",msg->CELL7,msg->CELL8,msg->CELL9);
}
void decode_CAN_0x10A(const uint8_t *data, CAN_BMS_0x10A_t *msg) {
    // Decode Cell10 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL10 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell11 voltage (Bytes 2-3)
    msg->CELL11 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell12 voltage (Bytes 4-5)
    msg->CELL12 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL10=%.2f v CELL11=%.2f v CELL12=%.2f v \n\r",msg->CELL10,msg->CELL11,msg->CELL12);
}
void decode_CAN_0x10B(const uint8_t *data, CAN_BMS_0x10B_t *msg) {
    // Decode Cell13 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL13 =  (float)((data[0] << 8) | data[1])/1000.f;
    // Decode Cell14 voltage (Bytes 2-3)
    msg->CELL14 = (float)((data[2] << 8) | data[3])/1000.0f;
    // Decode Cell15 voltage (Bytes 4-5)
    msg->CELL15 = (float)((data[4] << 8) | data[5])/1000.0f;
    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL13=%.2f v CELL14=%.2f v CELL15=%.2f v \n\r",msg->CELL13,msg->CELL14,msg->CELL15);
}
void decode_CAN_0x10C(const uint8_t *data, CAN_BMS_0x10C_t *msg) {
    // Decode Cell16 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL16 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell17 voltage (Bytes 2-3)
    msg->CELL17 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell3 voltage (Bytes 4-5)
    msg->CELL18 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
    uart_printf("CELL16=%.2f v \n\r",msg->CELL16);
}
void decode_CAN_0x10D(const uint8_t *data, CAN_BMS_0x10D_t *msg) {
    // Decode Cell19 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL19 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell20 voltage (Bytes 2-3)
    msg->CELL20 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell21 voltage (Bytes 4-5)
    msg->CELL21 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x10E(const uint8_t *data, CAN_BMS_0x10E_t *msg) {
    // Decode Cell22 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL22 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell23 voltage (Bytes 2-3)
    msg->CELL23 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell24 voltage (Bytes 4-5)
    msg->CELL24 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x10F(const uint8_t *data, CAN_BMS_0x10F_t *msg) {
    // Decode Cell25 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL25 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell26 voltage (Bytes 2-3)
    msg->CELL26 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell27 voltage (Bytes 4-5)
    msg->CELL27 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
void decode_CAN_0x110(const uint8_t *data, CAN_BMS_0x110_t *msg) {
    // Decode Cell28 voltage (Bytes 0-1)
    // The value is unsigned and in mV. "High byte first" means big-endian.
    msg->CELL28 =  (float)((data[0] << 8) | data[1])/1000.f;

    // Decode Cell29 voltage (Bytes 2-3)
    msg->CELL29 = (float)((data[2] << 8) | data[3])/1000.0f;

    // Decode Cell30 voltage (Bytes 4-5)
    msg->CELL30 = (float)((data[4] << 8) | data[5])/1000.0f;

    // Decode CRC_16 check value (Bytes 6-7)
    msg->CRC_Check = (data[6] << 8) | data[7];
}
