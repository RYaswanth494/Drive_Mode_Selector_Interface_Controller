/*
 * process.c
 *
 *  Created on: Sep 6, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"
#include"can.h"
#include"MCU_ELECTROCATLYST.h"
#include"process.h"
uint8_t crc8(uint8_t data) {
    uint8_t crc = 0x00; // initial value
    for (uint8_t i = 0; i < 8; i++) {
        if ((crc ^ data) & 0x80)
            crc = (crc << 1) ^ 0x07; // polynomial 0x07
        else
            crc <<= 1;
        data <<= 1;
    }
    return crc;
}
void Drive_mode_state(){
	if(I2C_RX_FLAG){
		I2C_RX_FLAG=0;
		if(crc8(data[0])==data[1]){
	    GPIOB->ODR &= ~((1<<PB0_REVERSE) | (1<<PB1_NEUTRAL) | (1<<PB3_DRIVE) | (1<<PB5_SPORT));
	    switch (data[0]) {
	        case 'R': GPIOB->ODR |= (1<<PB0_REVERSE); break;
	        case 'N': GPIOB->ODR |= (1<<PB1_NEUTRAL); break;
	        case 'D': GPIOB->ODR |= (1<<PB3_DRIVE);   break;
	        case 'S': GPIOB->ODR |= (1<<PB5_SPORT);   break;
	    }
		}
	}
}
void process_can_messages(void)
{
   can_frame_t rx_frame;

   if (CAN_MessagePending(0))
   {
	   can_rx(&rx_frame,0);
       matel_mcu_process_can_frame(&rx_frame);
//       send_can(rx_frame);
   }
   if (CAN_MessagePending(1))
   {
	   can_rx(&rx_frame,1);
       matel_mcu_process_can_frame(&rx_frame);
//       send_can(rx_frame);
   }
}
