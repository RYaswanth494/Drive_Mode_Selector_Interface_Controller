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
#include"can.h"
#define UART_PRINTF_BUFFER_SIZE 100
extern uint32_t get_APB1_freq(void) ;
STATUS uart_init(uint32_t Baud_Rate){
	 // Enable clocks
	    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
	    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // USART1
	    RCC->AHBENR |= RCC_AHBENR_DMA1EN; // DMA1

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
	    // 4. Configure DMA1 Channel4 for USART1_TX
	       DMA1_Channel4->CCR = 0;            // Disable channel
	       DMA1_Channel4->CPAR = (uint32_t)&USART1->DR; // Peripheral address
	       DMA1_Channel4->CCR = DMA_CCR_DIR | DMA_CCR_MINC; // memory-to-periph, memory increment
	    // Verify configuration
	    if (!(USART1->CR1 & USART_CR1_UE)) {
	        return RY_NOT_OK;
	    }
	    return RY_OK;
}
STATUS uart3_Init(uint32_t baudrate) {
    // 1. Enable clocks for GPIOB and USART3
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   // Enable GPIOB clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3 clock

    // 2. Configure PB10 as Alternate Function Push-Pull (TX), 50 MHz
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    GPIOB->CRH |= (0x03 << GPIO_CRH_MODE10_Pos); // Output mode, max speed 50 MHz
    GPIOB->CRH |= (0x02 << GPIO_CRH_CNF10_Pos);  // AF push-pull

    // 3. Configure PB11 as Input Floating (RX)
    GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
    GPIOB->CRH |= (0x01 << GPIO_CRH_CNF11_Pos);  // Floating input

    // 4. USART configuration
    USART3->CR1 = 0;  // Reset control register

    // Baud rate = Fclk / (16 * USARTDIV)
    // Example: PCLK1 = 36 MHz, baud = 115200
    // USARTDIV = 36MHz / (16 * 115200) ≈ 19.53
    // DIV_Mantissa = 19, DIV_Fraction = 0.53*16 ≈ 8
    uint32_t pclk = 36000000; // APB1 clock is 36 MHz (default with 72 MHz SYSCLK)
    uint32_t usartdiv = (pclk + (baudrate/2)) / baudrate; // scaled
    USART3->BRR = usartdiv;

    // Enable USART, TX, RX
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    return RY_OK;
}
void uart_print(uint8_t data)
{
    // Wait until TXE (Transmit Data Register Empty)
    while (!(USART3->SR & USART_SR_TXE)) {  }
    USART3->DR = data;
    while (!(USART3->SR & USART_SR_TC)) {  }
}
void uart_print_str(const char *str)
{
    while (*str) {
    	uart_print(*str++);
    }
}
void uart_send1(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        while (!(USART1->SR & (1U << 7))) { /* busy wait */ } // TXE bit
        USART1->DR = (uint8_t)buf[i];
    }
    while (!(USART1->SR & (1U << 6))) { /* wait TC */ } // TC bit
}
void uart_send(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        // Wait until TXE (transmit data register empty)
        while (!(USART3->SR & (1U << 7))) { /* busy wait */ } // TXE bit
        // Write data (DR is 8-bit)
        USART3->DR = (uint8_t)buf[i];
    }
    // Optionally wait for TC (transmission complete) if you need to ensure fully shifted out:
    while (!(USART3->SR & (1U << 6))) { /* wait TC */ } // TC bit
}
void send_id_data_only_over_uart(const can_frame_t *f) {
    uint8_t buf[13];
    while (!(USART1->SR & (1U << 7))) { /* busy wait */ } // TXE bit
    USART1->DR = (uint8_t)0XAA;
    buf[3] = (uint8_t)(f->id & 0xFF);
    buf[2] = (uint8_t)((f->id >> 8) & 0xFF);
    buf[1] = (uint8_t)((f->id >> 16) & 0xFF);
    buf[0] = (uint8_t)((f->id >> 24) & 0xFF);
    buf[4] = f->dlc;
    /* copy 8 data bytes (if dlc<8 you still send 8 bytes - zeros ok) */
    for (int i = 0; i < 8; ++i) buf[5 + i] = f->data[i];
    uart_send1(buf, sizeof(buf)/sizeof(buf[0])); // 13 bytes
}
void uart_printf(const char *format, ...)
{
    uint8_t buf[500];
    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    if (n > 0) {
        if ((size_t)n > sizeof(buf)) n = sizeof(buf);
        uart_send(buf, (size_t)n);
    }
}

