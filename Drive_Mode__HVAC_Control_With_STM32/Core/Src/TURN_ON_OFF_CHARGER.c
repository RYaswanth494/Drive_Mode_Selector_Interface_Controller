/*
 * TURN_ON_OFF_CHARGER.c
 *
 *  Created on: Oct 10, 2025
 *      Author: MadaYaswanth
 */


#include"TURN_ON_OFF_CHARGER_RELAY.h"
#include"main.h"
void Charger_pin_init(){
RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
GPIOC->CRH &= ~(0xF << 20);      // Clear CNF13[1:0] and MODE13[1:0] (bits 20–23)
GPIOC->CRH |=  (0x1 << 20);      // MODE13 = 01 (Output 10 MHz), CNF13 = 00

}
void charge_relay_on(){
	GPIOC->ODR&=~(1<<13);
}
void charge_relay_off(){
	GPIOC->ODR|=(1<<13);
}
