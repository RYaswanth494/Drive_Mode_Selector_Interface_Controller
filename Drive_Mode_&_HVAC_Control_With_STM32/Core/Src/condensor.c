/*
 * condensor.c
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"

#define PB15_CONDENSOR   15

void Condensor_Relay_Pins_Init(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRH&=~((0xFF<<28));
    GPIOB->CRH|=((0x3<<28));
    GPIOB->ODR|=((1<<PB15_CONDENSOR));
}
