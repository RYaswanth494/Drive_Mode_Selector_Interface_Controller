/*
 * sys_tick.c
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"

void systick_init(uint32_t ticks_per_sec){
    // Reload value = (SystemCoreClock / ticks_per_sec) - 1
    SysTick->LOAD = (get_SYSCLK_freq() / ticks_per_sec) - 1;

    SysTick->VAL = 0;             // Clear current value
    SysTick->CTRL = (1 << 2) |    // CLKSOURCE = processor clock (AHB)
                    (1 << 1) |    // TICKINT = enable SysTick interrupt
                    (1 << 0);     // ENABLE counter
}
