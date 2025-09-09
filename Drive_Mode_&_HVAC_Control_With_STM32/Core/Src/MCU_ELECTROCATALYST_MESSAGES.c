/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"uart.h"
#include"can.h"

void Decode_Matel_MCU_0X3AA(const uint8_t *data, Matel_MCU_0x3AA_t *msg){
	msg->BMS_CURRENT=(data[0]<<16)|(data[1]<<8)|(data[2]<<0);
	msg->BATTERY_VOLTAGE=(data[2]<<8 &0x3ff)|(data[3]);
}
void mcu_can_messages(){
	Decode_Matel_MCU_0X3AA();
}


/*

Byte0: b7  b6  b5  b4  b3  b2  b1  b0   (bits 7..0)
Byte1: b15 b14 b13 b12 b11 b10 b9  b8   (bits 15..8)
Byte2: b23 b22 b21 b20 b19 b18 b17 b16  (bits 23..16)
Byte3: b31 b30 b29 b28 b27 b26 b25 b24  (bits 31..24)
Byte4: b39 b38 b37 b36 b35 b34 b33 b32  (bits 39..32)
Byte5: b47 b46 b45 b44 b43 b42 b41 b40  (bits 47..40)
Byte6: b55 b54 b53 b52 b51 b50 b49 b48  (bits 55..48)
Byte7: b63 b62 b61 b60 b59 b58 b57 b56  (bits 63..56)


 * */
