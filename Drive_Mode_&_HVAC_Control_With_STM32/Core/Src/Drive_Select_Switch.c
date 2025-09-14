/*
 * Drive_Select_Switch.c
 *
 *  Created on: Sep 12, 2025
 *      Author: MadaYaswanth
 */


#include<stdint.h>
#include"main.h"
#include"Drive_Selector_Switch.h"
extern uint32_t milli_sec_tick;
uint32_t LastDebounceTime = 0;  // The Last Time The Output Pin Was Toggled
uint32_t DebounceDelay = 50;    // The Ddebounce Time; increase it if the output still flickers
switch_state_t BtnState=SW_STATE_NEUTRAL;               // The Current Reading From The Input Pin
switch_state_t LastBtnState = SW_STATE_NEUTRAL;       // The previous reading from The Input Pin
void Switch_Pins_int(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL&=~(0xf<<28);//PB7 as input//FORWARD
    GPIOB->CRL|=(0b10<<30);
    GPIOB->ODR|=(1<<7);
    GPIOB->CRL&=~(0XF<<24);// PB6 REVERSE
    GPIOB->CRL|=(0b10<<26);
    GPIOB->ODR|=(1<<6);
}
switch_state_t read_switch_raw(void)
{
    uint8_t fwd =!(GPIOB->IDR>>7&1);
    uint8_t rev =!(GPIOB->IDR>>6&1);

    if (fwd && !rev) return SW_STATE_FORWARD;
    if (!fwd && rev) return SW_STATE_REVERSE;
    if (!fwd && !rev) return SW_STATE_NEUTRAL;
}
// Debounce state machine (call periodically from main loop)
switch_state_t Switch_update(void)
{
    uint8_t TempBtnReading;
    TempBtnReading =read_switch_raw();// R_BIT(GPIOC->IDR,13); // read the state of the switch into a local variable:
    if(TempBtnReading != LastBtnState) // If the switch changed, due to noise or pressing:
    {
        LastDebounceTime = milli_sec_tick; // reset the debouncing timer
    }
    if((milli_sec_tick - LastDebounceTime) > DebounceDelay)
    {
        if(TempBtnReading != BtnState)
        {
            BtnState = TempBtnReading;
        }
    }
    LastBtnState = TempBtnReading;
    return BtnState;
}

