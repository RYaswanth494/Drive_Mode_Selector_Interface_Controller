#include"main.h"
#include"process.h"
#include"system_clock_init.h"
#include"uart.h"
#include"can.h"
#include"sys_tick.h"
#include"task_scheduler.h"
#include"Blower.h"
#include"Condensor.h"
#include"I2C.h"
#include"Motor_Control_uint_pins.h"
#include"process.h"
#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"Drive_Selector_Switch.h"
#include"JBD_BMS.h"
#include"JBD_BMS_MESSAGES.h"
extern void matel_mcu_process_can_frame(can_frame_t *);
I2C_HandleTypeDef hi2c1;
void clock_print_status(){
    uart_printf("\r\n=====================================\r\n");
    uart_printf("EVA DRIVE_MODE_SELECTOR & HVAC \r\n");
    uart_printf("=====================================\r\n");
    uart_printf("System Clock: %lu MHz\r\n", get_SYSCLK_freq() / 1000000);
    uart_printf("APB1 Clock: %lu MHz\r\n", get_APB1_freq() / 1000000);
    uart_printf("APB2 Clock: %lu MHz\r\n", get_APB2_freq() / 1000000);
    uart_printf("=====================================\r\n");
}
\
void can_ids_filter_configure(){
    uint32_t standard_rx_ids[] = {
    		Matel_MCU_POWER_CAN_STD_ID_A1,
			Matel_MCU_FAULT_one_CAN_STD_ID_AE,
			Matel_MCU_FAULT_two_CAN_STD_ID_AF,
			Matel_MCU_Fault_Code_CAN_STD_ID_B3,
    };

    uint32_t extended_rx_ids[] = {
    		Matel_MCU_Stat_One_CAN_EXTD_ID_18265040,
			Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040,
			Matel_MCU_HearthBeat_CAN_EXTD_ID_18963257,
			Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000
    };
    configure_can_filters(standard_rx_ids,sizeof(standard_rx_ids)/sizeof(standard_rx_ids[0]),extended_rx_ids,sizeof(extended_rx_ids)/sizeof(extended_rx_ids[0]));
    uart_printf("[INFO] Configuring CAN filters for CT4 MCU messages...\r\n");
}
void send_full_frame_over_uart( can_frame_t *f) {
    uart_send((uint8_t*)f, sizeof(can_frame_t));
}
void send_id_data_only_over_uart(const can_frame_t *f) {
    uint8_t buf[13];
    /* send id always as 4 bytes (LE) so ESP32 can read little-endian uint32_t */
    buf[3] = (uint8_t)(f->id & 0xFF);
    buf[2] = (uint8_t)((f->id >> 8) & 0xFF);
    buf[1] = (uint8_t)((f->id >> 16) & 0xFF);
    buf[0] = (uint8_t)((f->id >> 24) & 0xFF);
    buf[4] = f->dlc;
    /* copy 8 data bytes (if dlc<8 you still send 8 bytes - zeros ok) */
    for (int i = 0; i < 8; ++i) buf[5 + i] = f->data[i];
    uart_send1(buf, sizeof(buf)); // 13 bytes
}
#define PB0_REVERSE 0
#define PB1_NEUTRAL 1
#define PB3_DRIVE 3
int main(){
	HAL_Init();
	system_clock_init_to_72MHZ();
	if(uart3_Init(115200)==RY_NOT_OK){
	  Error_Handler();
	}
    uart_printf("UART3 initialization is ok ,baud_baudrate in kbps %d:\r\n", 115200);
    uart_printf("=========================================================\r\n");
	if(uart_init(115200)==RY_NOT_OK){
	  Error_Handler();
	}
	clock_print_status();
    uart_printf("UART1 initialization is ok ,baud_baudrate in kbps %d:\r\n", 115200);
    uart_printf("=========================================================\r\n");
    systick_init(1000);
    uart_printf("systick clock initialized for milli second ");
    uart_printf("=========================================================\r\n");
	if(can_init(500000)==RY_NOT_OK){
      uart_printf("CAN initialization is NOT oK\r\n");
	  uart_printf("======================================================\r\n");
	  Error_Handler();
	}
//	can_ids_filter_configure();
    uart_printf("CAN initialization is ok ,baud_baudrate in %d kbps:\r\n", 500);
    uart_printf("=========================================================\r\n");
    Init_tasks();
    uart_printf("Task schedular is initialized\r\n");
    uart_printf("=========================================================\r\n");
    Blower_Relay_Pins_Init();
    uart_printf("Blower pins are initialized PIN12=%d PIN13=%d PIN14=%d\r\n",(GPIOB->IDR>>12&1),(GPIOB->IDR>>13&1),(GPIOB->IDR>>14&1));
    uart_printf("=========================================================\r\n");
    Condensor_Relay_Pins_Init();
    uart_printf("Condensor pins are initialized PIN15=%d \r\n",(GPIOB->IDR>>15&1));
    uart_printf("=========================================================\r\n");
    Motor_control_pins_init();
    uart_printf("Motor_control pins are initialized PIN0=%d PIN1=%d  PIN3=%d PIN5=%d\r\n",(GPIOB->IDR>>0&1),(GPIOB->IDR>>1&1),(GPIOB->IDR>>3&1),(GPIOB->IDR>>5&1));
    uart_printf("=========================================================\r\n");
    Switch_Pins_int();
//    MX_I2C1_Init();
//    Register_task(50,Drive_mode_state);
//    Register_task(20,process_can_messages);
    //    Register_task(30,process_can_messages);
//    switch_state_t last_state,cur_state;
//     last_state=Switch_update();
    uint16_t id[]={0x100,0x101};//,0x102,0x103,0x104,0x105,0x106,0x107,0x108,0x109,0x10a,0x10b,0x10c,0x10d,0x10e,0x10f,0x110};
    uint8_t index=0,id_size=sizeof(id)/sizeof(id[0]);
    while(1){
    	    	   can_frame_t rx_frame;
    	    	   rx_frame.id=id[index];
    	    	   rx_frame.ide=0;
    	    	   rx_frame.dlc=8;
    	    	   rx_frame.data[0]=0;
    	    	   send_can_remote_frame(rx_frame);
    	    	   if (CAN_MessagePending(0))
    	    	   {
    	    		   can_rx(&rx_frame,0);
    	    		   Process_Jbd_Bms_Messages(&rx_frame);
    	    		   uart_printf("\n");
//    	    		   send_id_data_only_over_uart(&rx_frame);
    	    	   }
    	    	   if (CAN_MessagePending(1))
    	    	   {
    	    		   uart_printf("\n");
    	    		   can_rx(&rx_frame,1);
    	    		   Process_Jbd_Bms_Messages(&rx_frame);
//    	    		   send_id_data_only_over_uart(&rx_frame);
					 }
					index++;
					if(index>=2){
					index=0;
					}
//    	Run_all_tasks();
//    	   can_frame_t rx_frame;
//    	   if (CAN_MessagePending(0))
//    	   {
//    		   can_rx(&rx_frame,0);
//    		   send_id_data_only_over_uart(&rx_frame);
//    	       matel_mcu_process_can_frame(&rx_frame);
//    	   }
//    	   if (CAN_MessagePending(1))
//    	   {
//    		   can_rx(&rx_frame,1);
//    		   send_id_data_only_over_uart(&rx_frame);
//    	       matel_mcu_process_can_frame(&rx_frame);
//    	   }
//    	cur_state=Switch_update();
//    	if(cur_state!=last_state){
//    		last_state=cur_state;
//    	    GPIOB->ODR |= ((1<<PB0_REVERSE) | (1<<PB1_NEUTRAL) | (1<<PB3_DRIVE));
//    	    switch (cur_state) {
//    	        case 2: GPIOB->ODR &=~ (1<<PB0_REVERSE); break;
//    	        case 0: GPIOB->ODR &=~ (1<<PB1_NEUTRAL); break;
//    	        case 1: GPIOB->ODR &=~ (1<<PB3_DRIVE);   break;
//    	    }
//    		uart_printf(" button=%d :\n",cur_state);
//    	}
    }
}


void Error_Handler(void)
{
}
