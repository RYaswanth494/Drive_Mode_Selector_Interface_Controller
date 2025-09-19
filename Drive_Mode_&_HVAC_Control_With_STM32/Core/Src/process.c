/*
 * process.c
 *
 *  Created on: Sep 6, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"
#include"can.h"
#include"uart.h"
#include"MCU_ELECTROCATLYST.h"
#include"process.h"
#include"JBD_BMS.h"
#include"Drive_Selector_Switch.h"
switch_state_t last_state=0,cur_state;
uint16_t id[]={0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107,0x108,0x109,0x10a,0x10b,0x10c};
uint8_t id_size=sizeof(id)/sizeof(id[0]);
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
   while(CAN_MessagePending(0)){
	   can_rx(&rx_frame,0);
	   Process_Jbd_Bms_Messages(&rx_frame);
	   matel_mcu_process_can_frame(&rx_frame);
	   send_id_data_only_over_uart(&rx_frame);
   }
   while(CAN_MessagePending(1)){
	   can_rx(&rx_frame,1);
	   Process_Jbd_Bms_Messages(&rx_frame);
	   matel_mcu_process_can_frame(&rx_frame);
	   send_id_data_only_over_uart(&rx_frame);
   }

}
void process_switch_status(){
		cur_state=Switch_update();
		if(cur_state!=last_state){
			last_state=cur_state;
			GPIOB->ODR |= ((1<<PB0_REVERSE) | (1<<PB1_NEUTRAL) | (1<<PB3_DRIVE));
			switch (cur_state) {
				case 2: GPIOB->ODR &=~ (1<<PB0_REVERSE); break;
				case 0: GPIOB->ODR &=~ (1<<PB1_NEUTRAL); break;
				case 1: GPIOB->ODR &=~ (1<<PB3_DRIVE);   break;
			}
		}
}
void Request_Msgs_to_BMS(){
	   can_frame_t rx_frame;
	   static uint8_t i=0;
	   rx_frame.id=id[i];
	   rx_frame.ide=0;
	   rx_frame.dlc=8;
	   send_can_remote_frame(rx_frame);
	   i++;
	   if(i>=id_size){
		   i=0;
	   }
//	   for( i=0;i<id_size;i++){
////		   uint32_t start=HAL_GetTick();
//		   rx_frame.id=id[i];
//		   rx_frame.ide=0;
//		   rx_frame.dlc=8;
//		   send_can_remote_frame(rx_frame);
////		   uart_printf("%f \n\r",start-HAL_GetTick());
//	   }

}
