/*
 * uart.h
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include"main.h"
void uart_send(const uint8_t *buf, size_t len) ;
void uart_send1(const uint8_t *buf, size_t len) ;
STATUS uart_init(uint32_t Baud_Rate);
void uart_print(uint8_t data);
void uart_printf(const char *format, ...);
void uart_print_str(const char *str);
void uart_send_double(double value, uint8_t decimal_places);
STATUS uart3_Init(uint32_t baudrate) ;
#endif /* INC_UART_H_ */
