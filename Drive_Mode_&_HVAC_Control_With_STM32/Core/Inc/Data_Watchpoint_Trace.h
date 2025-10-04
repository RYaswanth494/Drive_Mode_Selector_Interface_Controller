/*
 * Data_Wtchpoint_Trace.h
 *
 *  Created on: Sep 18, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_DATA_WATCHPOINT_TRACE_H_
#define INC_DATA_WATCHPOINT_TRACE_H_

#include <stdint.h>
#include <stdbool.h>

// Initialize DWT
void DWT_Init(void);

// Start measurement
void DWT_Start(void);

// Stop measurement
void DWT_Stop(void);

// Reset cycle counter
void DWT_Reset(void);

// Get elapsed cycles
uint32_t DWT_GetCycles(void);

// Get elapsed time in microseconds
float DWT_GetUs(void);

// Get elapsed time in milliseconds
float DWT_GetMs(void);

// Check if DWT is supported
bool DWT_IsSupported(void);

#endif /* INC_DATA_WATCHPOINT_TRACE_H_ */
