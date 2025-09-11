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
void uart_send_double(double value, uint8_t decimal_places)
{
    char buf[32]; // Larger buffer for double
    int i = 0;

    // Handle negative
    if (value < 0) {
        buf[i++] = '-';
        value = -value;
    }

    // Integer part
    long long int_part = (long long)value;
    double frac = value - (double)int_part;

    // Convert integer part to string
    char int_buf[20];
    int j = 0;
    do {
        int_buf[j++] = '0' + (int_part % 10);
        int_part /= 10;
    } while (int_part > 0);

    // Reverse digits into buf
    for (int k = j - 1; k >= 0; k--) {
        buf[i++] = int_buf[k];
    }

    buf[i++] = '.'; // decimal point

    // Fractional part
    for (int k = 0; k < decimal_places; k++) {
        frac *= 10;
        int digit = (int)frac;
        buf[i++] = '0' + digit;
        frac -= digit;
    }

    // Send string over UART
    for (int k = 0; k < i; k++) {
    	uart_print(buf[k]);
    }
}
void uart_send(const char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        // Wait until TXE (transmit data register empty)
        while (!(USART1->SR & (1U << 7))) { /* busy wait */ } // TXE bit
        // Write data (DR is 8-bit)
        USART1->DR = (uint8_t)buf[i];
    }
    // Optionally wait for TC (transmission complete) if you need to ensure fully shifted out:
    while (!(USART1->SR & (1U << 6))) { /* wait TC */ } // TC bit
}
void uart_printf(const char *format, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    if (n > 0) {
        // truncate if too long
        if ((size_t)n > sizeof(buf)) n = sizeof(buf);
        uart_send(buf, (size_t)n);
    }
}

