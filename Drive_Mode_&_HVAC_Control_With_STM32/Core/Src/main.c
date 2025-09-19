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
void can_ids_filter_configure(){
    uint32_t standard_rx_ids[] = {
    		JBD_BMS_ID0,
			JBD_BMS_ID1,
			JBD_BMS_ID7,
			JBD_BMS_ID8,
			JBD_BMS_ID9,
			JBD_BMS_ID10,
			JBD_BMS_ID11,
			JBD_BMS_ID12,
    		Matel_MCU_POWER_CAN_STD_ID_A1,
			Matel_MCU_FAULT_one_CAN_STD_ID_AE,
			Matel_MCU_FAULT_two_CAN_STD_ID_AF,
			Matel_MCU_Fault_Code_CAN_STD_ID_B3,
    };

    uint32_t extended_rx_ids[] = {
    		Matel_MCU_Stat_One_CAN_EXTD_ID_18265040,
			Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040,
			Matel_VECTOR__INDEPENDENT_SIG_MSG_0xC0000000
    };
    configure_can_filters(standard_rx_ids,sizeof(standard_rx_ids)/sizeof(standard_rx_ids[0]),extended_rx_ids,sizeof(extended_rx_ids)/sizeof(extended_rx_ids[0]));
    uart_printf("[INFO] Configuring CAN filters for CT4 MCU messages...\r\n");
}
void send_full_frame_over_uart( can_frame_t *f) {
    uart_send((uint8_t*)f, sizeof(can_frame_t));
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
	can_ids_filter_configure();
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
    Register_task(50,process_switch_status);
    Register_task(2,Request_Msgs_to_BMS);
    Register_task(20,process_can_messages);


    while(1){

    Run_all_tasks();

    }
}


void Error_Handler(void)
{
}
