/*
 * system_clock_init.h
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_SYSTEM_CLOCK_INIT_H_
#define INC_SYSTEM_CLOCK_INIT_H_
#include"main.h"
STATUS system_clock_init_to_72MHZ(void);
uint32_t get_SYSCLK_freq(void);
uint32_t get_AHB_freq(void) ;
uint32_t get_APB1_freq(void);
uint32_t get_APB2_freq(void) ;

#endif /* INC_SYSTEM_CLOCK_INIT_H_ */
