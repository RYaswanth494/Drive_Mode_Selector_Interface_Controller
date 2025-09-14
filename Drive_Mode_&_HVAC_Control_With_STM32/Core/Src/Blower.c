/*
 * Blower.c
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */


#include"extern_file_declarations.h"

#define PB12_LOW_SPEED_BLOWER    12
#define PB13_MEDIUM_SPEED_BLOWER 13
#define PB14_HIGH_SPEED_BLOWER   14

void Blower_Relay_Pins_Init(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRH&=~((0xF<<16)|(0XF<<20)|(0XF<<24));
    GPIOB->CRH|=((0x3<<16)|(0X3<<20)|(0X3<<24));
    GPIOB->ODR|=((1<<PB12_LOW_SPEED_BLOWER)|(1<<PB13_MEDIUM_SPEED_BLOWER)|(1<<PB14_HIGH_SPEED_BLOWER));
}
