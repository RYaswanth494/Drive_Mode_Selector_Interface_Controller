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
#include"timer.h"
#include"Motor_Control_uint_pins.h"
#include"process.h"
#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"Drive_Selector_Switch.h"
#include"JBD_BMS.h"
#include"JBD_BMS_MESSAGES.h"
#include"TURN_ON_OFF_CHARGER_RELAY.h"
extern volatile uint32_t ms_tick ;
extern volatile uint32_t us_tick ;
extern void matel_mcu_process_can_frame(can_frame_t *);
I2C_HandleTypeDef hi2c1;
void clock_print_status(){
    uart_printf("\r\n=====================================\r\n");
    uart_printf("     APP                          \r\n");
    uart_printf("=====================================\r\n");
    uart_printf("System Clock: %lu MHz\r\n", get_SYSCLK_freq() / 1000000);
    uart_printf("APB1 Clock: %lu MHz\r\n", get_APB1_freq() / 1000000);
    uart_printf("APB2 Clock: %lu MHz\r\n", get_APB2_freq() / 1000000);
    uart_printf("=====================================\r\n");
}
#define MAX_FILTERS 14
// Helper: configure one filter bank with up to 2 IDs
static void CAN_ConfigFilterBank(int bank, uint32_t id1, uint32_t id2, uint8_t fifo, uint8_t isExtended)
{
    CAN1->FA1R &= ~(1 << bank);     // deactivate

    CAN1->FM1R |= (1 << bank);      // list mode
    CAN1->FS1R |= (1 << bank);      // 32-bit scale

    if (isExtended) {
        // Extended: shift << 3, set IDE bit
        uint32_t eid1 = (id1 << 3) | (1 << 2);
        uint32_t eid2 = (id2 << 3) | (1 << 2);
        CAN1->sFilterRegister[bank].FR1 = eid1;
        CAN1->sFilterRegister[bank].FR2 = eid2;
    } else {
        // Standard: shift << 21
        CAN1->sFilterRegister[bank].FR1 = (id1 << 21);
        CAN1->sFilterRegister[bank].FR2 = (id2 << 21);
    }

    if (fifo == 0)
        CAN1->FFA1R &= ~(1 << bank);   // FIFO0
    else
        CAN1->FFA1R |= (1 << bank);    // FIFO1

    CAN1->FA1R |= (1 << bank);         // activate
}

// Main filter loader
void CAN_LoadFilters(uint32_t *mcu_std, int mcu_std_count,
                     uint32_t *mcu_ext, int mcu_ext_count,
                     uint32_t *bms_std, int bms_std_count)
{
    int filter_bank = 0;
    CAN1->FMR |= CAN_FMR_FINIT; // Enter filter init
    // --- MCU Std IDs → FIFO0 ---
    for (int i = 0; i < mcu_std_count; i += 2) {
        uint32_t id1 = mcu_std[i];
        uint32_t id2 = (i+1 < mcu_std_count) ? mcu_std[i+1] : mcu_std[i]; // duplicate if odd count
        CAN_ConfigFilterBank(filter_bank++, id1, id2, 0, 0);
    }

    // --- MCU Ext IDs → FIFO0 ---
    for (int i = 0; i < mcu_ext_count; i += 2) {
        uint32_t id1 = mcu_ext[i];
        uint32_t id2 = (i+1 < mcu_ext_count) ? mcu_ext[i+1] : mcu_ext[i];
        CAN_ConfigFilterBank(filter_bank++, id1, id2, 0, 1);
    }

    // --- BMS Std IDs → FIFO1 ---
    for (int i = 0; i < bms_std_count; i += 2) {
        uint32_t id1 = bms_std[i];
        uint32_t id2 = (i+1 < bms_std_count) ? bms_std[i+1] : bms_std[i];
        CAN_ConfigFilterBank(filter_bank++, id1, id2, 1, 0);
    }

    CAN1->FMR &= ~CAN_FMR_FINIT; // Exit filter init
}

// --- User config wrapper ---
void can_ids_filter_configure(void)
{

    uint32_t mcu_standard_rx_ids[] = {
        Matel_MCU_POWER_CAN_STD_ID_A1,
        Matel_MCU_FAULT_one_CAN_STD_ID_AE,
        Matel_MCU_FAULT_two_CAN_STD_ID_AF,
        Matel_MCU_Fault_Code_CAN_STD_ID_B3,
//		0x1234
    };
    uint32_t bms_standard_ids[] = {
        JBD_BMS_ID0,
        JBD_BMS_ID1,
        JBD_BMS_ID2,
        JBD_BMS_ID4,
        JBD_BMS_ID5,
        JBD_BMS_ID6,
        JBD_BMS_ID7,
        JBD_BMS_ID8,
        JBD_BMS_ID9,
        JBD_BMS_ID10,
        JBD_BMS_ID11,
        JBD_BMS_ID12,
    };

    uint32_t mcu_extended_rx_ids[] = {
        Matel_MCU_Stat_One_CAN_EXTD_ID_18265040,
        Matel_MCU_Stat_Two_CAN_EXTD_ID_18275040,
		0x1234
    };

    CAN_LoadFilters(mcu_standard_rx_ids,
                    sizeof(mcu_standard_rx_ids)/sizeof(mcu_standard_rx_ids[0]),
                    mcu_extended_rx_ids,
                    sizeof(mcu_extended_rx_ids)/sizeof(mcu_extended_rx_ids[0]),
                    bms_standard_ids,
                    sizeof(bms_standard_ids)/sizeof(bms_standard_ids[0]));
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
	 FLASH_WRITE_FLAG(BL_UPDATE_DONE);
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
    Charger_pin_init();
//    Init_timer();
//    MX_I2C1_Init();
//    Register_task(50,Drive_mode_state); //dial
    Register_task(50,process_switch_status);
    Register_task(2,Request_Msgs_to_BMS);
    Register_task(0,process_can_messages);
    Register_task(100,print_mcu_data);
    Register_task(100,print_bms_data);
    Register_task(50,charger_relay_on_off);

//    DWT_Init();

    while(1){
//uint32_t start=HAL_GetTick();
//    uart_printf("time= %.3f\n\r",(float)HAL_GetTick());
    Run_all_tasks();
//    uart_printf("time= %.3f\n\r",(float)(HAL_GetTick()-start));
    }
}


void Error_Handler(void)
{
}
