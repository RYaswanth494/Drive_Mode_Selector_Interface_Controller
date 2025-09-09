/*
 * extern_file_declarations.h
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_EXTERN_FILE_DECLARATIONS_H_
#define INC_EXTERN_FILE_DECLARATIONS_H_
#include"main.h"

extern STATUS can_init(uint32_t Baud_Rate);
extern void send_can(CAN_FRAME can_frame);
extern void can_set_filter(uint32_t *std_ids,uint32_t *ext_ids );
extern void systick_init(uint32_t ticks_per_sec);
extern STATUS system_clock_init_to_72MHZ(void);
extern uint32_t get_SYSCLK_freq(void);
extern uint32_t get_AHB_freq(void) ;
extern uint32_t get_APB1_freq(void);
extern uint32_t get_APB2_freq(void) ;
extern STATUS uart_init(uint32_t Baud_Rate);
extern void uart_print(uint8_t data);
extern void uart_printf(const char *format, ...);
extern void uart_print_str(const char *str);
extern uint32_t milli_sec_tick;
extern void Error_Handler(void);
typedef void (*TaskFunction)(void);
typedef struct{
	uint32_t period_ms;
	uint32_t last_time_ms;
	TaskFunction task_fn;
	uint8_t enable;
	uint8_t one_time_execute;
}task_timer;
extern void Init_tasks(void);
extern int Register_task(uint32_t period_ms, TaskFunction task_fn);
extern int Schedule_task_once(uint32_t time_period, TaskFunction task_fn);
extern void Enable_task(int task_id);
extern void Disable_task(int task_id);
extern void Remove_task(int task_id);
extern void Run_all_tasks(void);


extern void Blower_Relay_Pins_Init();
extern void Condensor_Relay_Pins_Init();
extern void Motor_control_pins_init();
extern I2C_HandleTypeDef hi2c1;

#define PB0_REVERSE 0
#define PB1_NEUTRAL 1
#define PB3_DRIVE 3
#define PB5_SPORT 5
extern void Drive_mode_state();
extern uint8_t I2C_RX_FLAG,data[2];

#endif /* INC_EXTERN_FILE_DECLARATIONS_H_ */
