/*
 * Motor_control_Pins.c
 *
 *  Created on: Sep 5, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"
#define PB0_REVERSE 0
#define PB1_NEUTRAL 1
#define PB3_DRIVE 3
#define PB4_SPORT 5

void Motor_control_pins_init(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL&=~((0xFF<<0)|(0XFF<<4)|(0XFF<<12)|(0xff<<20));
    GPIOB->CRL|=((0x3<<0)|(0X3<<4)|(0X3<<12)|(0x3<<20));
    GPIOB->ODR&=~((1<<PB0_REVERSE)|(1<<PB1_NEUTRAL)|(1<<PB3_DRIVE)|(1<<PB4_SPORT));
//    GPIOB->ODR|=((1<<PB0_REVERSE)|(1<<PB1_NEUTRAL)|(1<<PB3_DRIVE)|(1<<PB4_SPORT));
}
