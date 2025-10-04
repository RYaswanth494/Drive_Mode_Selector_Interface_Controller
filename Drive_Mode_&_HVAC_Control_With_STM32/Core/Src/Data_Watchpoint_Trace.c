/*
 * Data_Watchpoint_Trace.c
 *
 *  Created on: Sep 18, 2025
 *      Author: MadaYaswanth
 *

           ┌───────────────────────┐
           │   Cortex-M3 CPU Core  │
           │                       │
           │  ┌───────────────┐    │
           │  │  FPB (Flash   │<───┤-- Breakpoints / Flash patch
           │  │  Patch &      │    │
           │  │  Breakpoint)  │    │
           │  └───────────────┘    │
           │                       │
           │  ┌───────────────┐    │
           │  │  DWT (Data    │<───┤-- Cycle counter / Watchpoints
           │  │  Watchpoint & │    │
           │  │  Trace)       │    │
           │  └───────────────┘    │
           │                       │
           │  ┌───────────────┐    │
           │  │  ITM           │<──┤-- Software trace (printf, events)
           │  │ (Instrumentation│  │
           │  │  Trace Macrocell│  │
           │  └───────────────┘    │
           │                       │
           │  ┌───────────────┐    │
           │  │ CoreDebug /   │<── ┤-- Halting, status, vector catch
           │  │ DHCSR/DEMCR   │    │
           │  └───────────────┘    │
           └───────────────────────┘
                     │
                     ▼
          ┌─────────────────────┐
          │ Debug Interfaces    │
          │                     │
          │  SWD (2 pins)       │-- Control + read/write
          │  SWO (1 pin)        │-- Trace output (ITM/DWT events)
          │  Optional JTAG      │-- Full debug control
          └─────────────────────┘
*/
#include"Data_Watchpoint_Trace.h"
#include"main.h"
#include<stdbool.h>
#define CPU_CLOCK_HZ 72000000UL // Change according to your system clock

static uint32_t start_cycle = 0;
static uint32_t end_cycle = 0;

// Check if DWT is supported
bool DWT_IsSupported(void) {
    // Cortex-M0/M0+ may not support DWT->CYCCNT
    return ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) != 0);
}

// Initialize DWT
void DWT_Init(void) {
    // Enable trace
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Reset the counter
    DWT->CYCCNT = 0;

    // Enable cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    // Reset start/end
    start_cycle = 0;
    end_cycle = 0;
}

// Reset counter
void DWT_Reset(void) {
    DWT->CYCCNT = 0;
    start_cycle = 0;
    end_cycle = 0;
}

// Start measurement
void DWT_Start(void) {
    start_cycle = DWT->CYCCNT;
}

// Stop measurement
void DWT_Stop(void) {
    end_cycle = DWT->CYCCNT;
}

// Get elapsed cycles
uint32_t DWT_GetCycles(void) {
    return (end_cycle - start_cycle);
}

// Get elapsed time in microseconds
float DWT_GetUs(void) {
    return ((float)(end_cycle - start_cycle)) / (CPU_CLOCK_HZ / 1000000.0f);
}

// Get elapsed time in milliseconds
float DWT_GetMs(void) {
    return ((float)(end_cycle - start_cycle)) / (CPU_CLOCK_HZ / 1000.0f);
}



