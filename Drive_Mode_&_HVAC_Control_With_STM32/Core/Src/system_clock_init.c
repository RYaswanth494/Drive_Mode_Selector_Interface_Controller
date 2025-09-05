/*
 * system_clock_init.c
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */
#include"main.h"
STATUS system_clock_init_to_72MHZ(){
	    // 1. Enable HSE (external crystal, e.g., 8MHz)
	    RCC->CR |= RCC_CR_HSEON;       // Turn on HSE
	    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready
	    FLASH->ACR |= FLASH_ACR_LATENCY_2;  // 2 wait states
	    FLASH->ACR |= FLASH_ACR_PRFTBE;     // Enable prefetch
	    // 2. Configure PLL to multiply HSE to 72 MHz
	    //    PLLMUL = 9 (8MHz * 9 = 72MHz)
	    RCC->CFGR |= RCC_CFGR_PLLMULL9; // PLL multiplier = 9
	    // 3. Select HSE as PLL source
	    RCC->CFGR |= RCC_CFGR_PLLSRC;   // PLL source = HSE
	    // 4. Set APB1 Prescaler to 2 (PCLK1 = 72MHz / 2 = 36MHz)
	    RCC->CFGR |= RCC_CFGR_PPRE1_2;  // APB1 prescaler = 2 (36MHz)
	    // 5. Set APB2 Prescaler to 1 (PCLK2 = 72MHz, no division)
	    RCC->CFGR &= ~RCC_CFGR_PPRE2;   // APB2 prescaler = 1 (72MHz)
	    // 6. Enable PLL and wait until ready
	    RCC->CR |= RCC_CR_PLLON;
	    while (!(RCC->CR & RCC_CR_PLLRDY));
	     // 7. Switch SYSCLK to PLL
	    RCC->CFGR |= RCC_CFGR_SW_PLL;
	    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
uint32_t get_SYSCLK_freq(void) {
    uint32_t sysclk_source, pllmul, prediv, pllclk;
    uint32_t HSE_Value = 8000000; // External crystal (adjust if different)
    uint32_t HSI_Value = 8000000; // Internal RC
    sysclk_source = (RCC->CFGR >> 2) & 0x3;  // SWS bits
    switch (sysclk_source) {
        case 0x00: // HSI
            return HSI_Value;
        case 0x01: // HSE
            return HSE_Value;
        case 0x02: // PLL
            // PLL source
            if (RCC->CFGR & (1 << 16)) {
                // HSE as PLL source
                if (RCC->CFGR & (1 << 17))
                    prediv = 2;   // HSE/2
                else
                    prediv = 1;   // HSE
                pllclk = HSE_Value / prediv;
            } else {
                // HSI/2 as PLL source
                pllclk = HSI_Value / 2; // 4 MHz
            }

            // PLL multiplier
            pllmul = ((RCC->CFGR >> 18) & 0xF) + 2;
            // Note: value 0b1101 = x13, 0b1110 = x14, 0b1111 = x16
            if (pllmul == 15) pllmul = 16; // adjust for 0b1111 special case

            return pllclk * pllmul;
        default:
            return HSI_Value; // fallback
    }
}
uint32_t get_AHB_freq(void) {
    uint32_t presc_table[16] = {1,1,1,1,1,1,1,1,2,4,8,16,64,128,256,512};
    uint32_t hpre = (RCC->CFGR >> 4) & 0xF;
    return get_SYSCLK_freq() / presc_table[hpre];
}

uint32_t get_APB1_freq(void) {
    uint32_t presc_table[8] = {1,1,1,1,2,4,8,16};
    uint32_t ppre1 = (RCC->CFGR >> 8) & 0x7;
    return get_AHB_freq() / presc_table[ppre1];
}

uint32_t get_APB2_freq(void) {
    uint32_t presc_table[8] = {1,1,1,1,2,4,8,16};
    uint32_t ppre2 = (RCC->CFGR >> 11) & 0x7;
    return get_AHB_freq() / presc_table[ppre2];
}
