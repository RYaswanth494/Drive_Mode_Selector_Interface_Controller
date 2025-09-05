/*
 * uart.c
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */

#include"main.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include"extern_file_declarations.h"
#define UART_PRINTF_BUFFER_SIZE 100
extern uint32_t get_APB1_freq(void) ;
STATUS uart_init(uint32_t Baud_Rate){
	 // Enable clocks
	    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
	    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // USART1
	    // PA9 = TX (AF push-pull), PA10 = RX (input floating)
	    GPIOA->CRH &= ~((0xF << (4 * 1)) | (0xF << (4 * 2))); // clear CNF/MODE for PA9, PA10
	    GPIOA->CRH |=  (0xB << (4 * 1)); // PA9 = AF PP, 50 MHz
	    GPIOA->CRH |=  (0x4 << (4 * 2)); // PA10 = input floating
	    // Reset USART1
	    USART1->CR1 = 0;
	    // Calculate baudrate divisor
	    uint32_t pclk2 = get_APB2_freq();
	    if (pclk2 == 0 || Baud_Rate == 0) {
	        return RY_NOT_OK;
	    }
	    uint32_t usartdiv = (pclk2 + (Baud_Rate/2)) / Baud_Rate; // nearest
	    USART1->BRR = usartdiv;
	    // Enable USART, TX, RX
	    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	    // Verify configuration
	    if (!(USART1->CR1 & USART_CR1_UE)) {
	        return RY_NOT_OK;
	    }
	    return RY_OK;
}
/**
 * @brief  Send a single byte over UART1
 * @param  data: byte to send
 * @return UART_Status_t
 */
void uart_print(uint8_t data)
{
    // Wait until TXE (Transmit Data Register Empty)
    while (!(USART1->SR & USART_SR_TXE)) {  }
    USART1->DR = data;
    while (!(USART1->SR & USART_SR_TC)) {  }
}
void uart_print_str(const char *str)
{
    while (*str) {
    	uart_print(*str++);
    }
}
void uart_printf(const char *format, ...)
{
    char buffer[UART_PRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    uart_print_str(buffer);
}

