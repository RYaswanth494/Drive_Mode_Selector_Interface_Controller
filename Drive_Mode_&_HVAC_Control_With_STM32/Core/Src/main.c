#include"main.h"
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
CAN_FRAME frame;
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
void CAN_Filter_Config(void)
{
    // Enter filter init mode
    CAN1->FMR |= CAN_FMR_FINIT;

    // Select filter bank 0
    uint8_t filter_bank = 0;

    // Deactivate filter
    CAN1->FA1R &= ~(1 << filter_bank);

    // Identifier for 0x3AA (standard ID)
    uint32_t filter_id = (0x3AA << 5);  // shift 5 bits for StdID

    // Mask mode: accept only exact ID
    uint32_t filter_mask = 0x7FF << 5;  // all 11 bits must match

    // Set filter scale (32-bit)
    CAN1->FS1R |= (1 << filter_bank);

    // Set filter mode (mask mode)
    CAN1->FM1R &= ~(1 << filter_bank);

    // Set ID and mask
    CAN1->sFilterRegister[filter_bank].FR1 = filter_id;
    CAN1->sFilterRegister[filter_bank].FR2 = filter_mask;

    // Assign filter to FIFO 0
    CAN1->FFA1R &= ~(1 << filter_bank);

    // Activate filter
    CAN1->FA1R |= (1 << filter_bank);

    // Exit filter init mode
    CAN1->FMR &= ~CAN_FMR_FINIT;
}
void can_ids_configure(){
    /* Standard IDs array - terminated with 0x0000 */
    uint16_t standard_rx_ids[] = {
    		Matel_MCU_POWER_CAN_STD_ID_A1,
			Matel_MCU_FAULT_CAN_STD_ID_AE,
			Matel_MCU_FAULT_two_CAN_STD_ID_AF,
			Matel_MCU_Fault_Code_CAN_STD_ID_B3,
			Matel_CANFRAME3_CAN_STD_ID_3AA,
        0x0000  /* End-of-list sentinel */
    };

    /* Extended IDs array - terminated with 0x00000000 */
    uint32_t extended_rx_ids[] = {
    	Matel_MCU_Stat_One_CAN_EXTD_ID_18265040,
		Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040,
		Matel_HearthBeat_CAN_EXTD_ID_18963257,
        0x00000000  /* End-of-list sentinel */
    };

    uart_printf("[INFO] Configuring CAN filters for CT4 MCU messages...\r\n");
}
int main(){
	HAL_Init();
	system_clock_init_to_72MHZ();
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
    uart_printf("CAN initialization is ok ,baud_baudrate in %d kbps:\r\n", 500);
    uart_printf("=========================================================\r\n");
    CAN_Filter_Config();
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
    MX_I2C1_Init();
    Register_task(50,Drive_mode_state);
    while(1){
    	Run_all_tasks();
    }
}


void Error_Handler(void)
{
}
