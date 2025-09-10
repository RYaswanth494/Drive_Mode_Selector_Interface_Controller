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
/*
 * Master (M5Dial)                Slave (STM32)                   Output (GPIO)
      |                              |                               |
      |--[Rotate/Select Mode]------->|                               |
      |                              |                               |
      |                              |--[Wait for debounce 50 ms]-->|  Ignore noise
      |                              |                               |
      |                              |--[Prepare 4-byte data + CRC32]|
      |                              |                               |
      |--[I2C Send 4 bytes + CRC32]->|                               |
      |                              |                               |
      |                              |--[Receive I2C]--------------->|  ~180 µs
      |                              |--[Verify CRC32]-------------->|  ~5 µs
      |                              |--[Parse Command]-------------->|  ~1 µs
      |                              |--[Update GPIO ODR]------------>|  Output changes
      |                              |                               |
 * */
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

   while (can_recv_bulk( &rx_frame, 1) > 0)
   {
       matel_mcu_process_can_frame(&rx_frame);
       send_can(rx_frame);
   }
}
