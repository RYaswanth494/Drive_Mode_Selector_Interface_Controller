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
