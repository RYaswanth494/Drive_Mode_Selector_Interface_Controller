/*
 * timer.c
 *
 *  Created on: Oct 6, 2025
 *      Author: MadaYaswanth
 */


/*
 * timer.c
 *
 *  Created on: May 24, 2025
 *      Author: pariv
 */

#include "timer.h"
#include "main.h"
#include <stdint.h>



volatile uint32_t ms_tick = 0;
volatile uint32_t us_tick = 0;
uint32_t timer_diff = 0;
//
void Init_timer(void){
	 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   // Enable TIM2 clock

	    TIM2->PSC = 71;                       // 72 MHz / (71+1) = 1 MHz → 1 tick = 1 µs
	    TIM2->ARR = 1 - 1;            // Interrupt every ‘period_us’ µs
	    TIM2->EGR |= TIM_EGR_UG;              // Update registers immediately

	    TIM2->DIER |= TIM_DIER_UIE;           // Enable update interrupt
	    TIM2->CR1  |= TIM_CR1_CEN;            // Start counter

	    NVIC_EnableIRQ(TIM2_IRQn);            // Enable TIM2 interrupt in NVIC
	    NVIC_SetPriority(TIM2_IRQn, 1);

}

//void timer_handler(void)
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)            // Check update interrupt flag
    {
        TIM2->SR &= ~TIM_SR_UIF;          // Clear it
        us_tick++;
        // ---- Your microsecond code here ----
        // Example: toggle a pin or increment counter
    }
}
