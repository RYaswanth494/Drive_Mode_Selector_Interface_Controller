/*
 * uart.h
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include"main.h"

STATUS uart_init(uint32_t Baud_Rate);
void uart_print(uint8_t data);
void uart_printf(const char *format, ...);
void uart_print_str(const char *str);
void uart_send_double(double value, uint8_t decimal_places);
#endif /* INC_UART_H_ */
