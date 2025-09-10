/*
 * MCU_ELECTROCATALYST.c
 *
 *  Created on: Sep 9, 2025
 *      Author: MadaYaswanth
 */


#include"MCU_ELECTROCATALYST_MESSAGES.h"
#include"uart.h"
#include"can.h"
#include<stdint.h>
#include <string.h>
#include<stdbool.h>
// Decoding functions for each CAN message
// Helper: extract Motorola-format (big-endian) up to 64 bits
// Sign-extend a value of 'len' bits
static int64_t sign_extend_u64(uint64_t v, int len) {

if (len <= 0 || len > 64) return (int64_t)v;
uint64_t sign = 1ULL << (len - 1);
if (v & sign) {
uint64_t mask = (~0ULL) << len;
return (int64_t)(v | mask);
}
return (int64_t)v;
}
static uint64_t extract_motorola_u64(const uint8_t *data, int start, int len) {
if (len <= 0 || len > 64) return 0;
uint64_t value = 0;
int cur_byte = start / 8;
int cur_bit = start % 8; // 7 = MSB of byte
for (int i = 0; i < len; i++) {
uint8_t bit = 0;
if (cur_byte >= 0 && cur_byte < 8) bit = (data[cur_byte] >> cur_bit) & 1U;
value = (value << 1) | (uint64_t)bit;
cur_bit--;
if (cur_bit < 0) { cur_byte++; cur_bit = 7; }
}
return value;
}


// Helper: extract Intel-format (little-endian) up to 64 bits
static uint64_t extract_little_endian_u64(const uint8_t *data, int start, int len) {
if (len <= 0 || len > 64) return 0;
uint64_t value = 0;
for (int i = 0; i < len; i++) {
int bit_index = start + i;
int byte_index = bit_index / 8;
int bit_in_byte = bit_index % 8;
uint8_t bit = 0;
if (byte_index >= 0 && byte_index < 8) bit = (data[byte_index] >> bit_in_byte) & 1U;
value |= ((uint64_t)bit << i);
}
return value;
}



// Implementations of decode functions follow (using extract helpers)


void decode_MCU_Stat_Two(const uint8_t *data, MCU_Stat_Two_t *out) {
if (!out) return;
// MCU_Odometer_Val: little-endian start=32 len=32 factor=0.1 offset=0.0
uint64_t raw0 = extract_little_endian_u64(data, 32, 32);
out->MCU_Odometer_Val = (double)raw0 * 0.1 + 0.0;
// MCU_VCU_State: little-endian start=24 len=8
uint64_t raw1 = extract_little_endian_u64(data, 24, 8);
out->MCU_VCU_State = (double)raw1;
// MCU_Motor_RPM: little-endian start=0 len=16 offset=-16384
uint64_t raw2 = extract_little_endian_u64(data, 0, 16);
out->MCU_Motor_RPM = (double)raw2 + -16384.0;
}


void decode_MCU_Fault_Code(const uint8_t *data, MCU_Fault_Code_t *out) {
if (!out) return;
// sequential 8-bit little-endian signals
out->MCU_Fault_Code_0 = (double)extract_little_endian_u64(data, 0, 8);
out->MCU_Fault_Code_1 = (double)extract_little_endian_u64(data, 8, 8);
out->MCU_Fault_Code_2 = (double)extract_little_endian_u64(data, 16, 8);
out->MCU_Fault_Code_3 = (double)extract_little_endian_u64(data, 24, 8);
out->MCU_Fault_Code_4 = (double)extract_little_endian_u64(data, 32, 8);
out->MCU_Fault_Code_5 = (double)extract_little_endian_u64(data, 40, 8);
out->MCU_Fault_Code_6 = (double)extract_little_endian_u64(data, 48, 8);
out->MCU_Fault_Code_7 = (double)extract_little_endian_u64(data, 56, 8);
out->MCU_Fault_Code_8 = (double)extract_little_endian_u64(data, 64, 8);
out->MCU_Fault_Code_9 = (double)extract_little_endian_u64(data, 72, 8);
out->MCU_Fault_Code_10 = (double)extract_little_endian_u64(data, 80, 8);
out->MCU_Fault_Code_11 = (double)extract_little_endian_u64(data, 88, 8);
out->MCU_Fault_Code_12 = (double)extract_little_endian_u64(data, 96, 8);
out->MCU_Fault_Code_13 = (double)extract_little_endian_u64(data, 104, 8);
out->MCU_Fault_Code_14 = (double)extract_little_endian_u64(data, 112, 8);
out->MCU_Fault_Code_15 = (double)extract_little_endian_u64(data, 120, 8);
out->MCU_Fault_Code_16 = (double)extract_little_endian_u64(data, 128, 8);
out->MCU_Fault_Code_17 = (double)extract_little_endian_u64(data, 136, 8);
out->MCU_Fault_Code_18 = (double)extract_little_endian_u64(data, 144, 8);
out->MCU_Fault_Code_19 = (double)extract_little_endian_u64(data, 152, 8);
out->MCU_Fault_Code_20 = (double)extract_little_endian_u64(data, 160, 8);
out->MCU_Fault_Code_21 = (double)extract_little_endian_u64(data, 168, 8);
out->MCU_Fault_Code_22 = (double)extract_little_endian_u64(data, 176, 8);
out->MCU_Fault_Code_23 = (double)extract_little_endian_u64(data, 184, 8);
}


void decode_HearthBeat(const uint8_t *data, HearthBeat_t *out) {
if (!out) return;
out->HeartBeat_Stat0 = (double)extract_little_endian_u64(data, 0, 1);
out->HeartBeat_Stat1 = (double)extract_little_endian_u64(data, 1, 1);
out->HeartBeat_Stat2 = (double)extract_little_endian_u64(data, 2, 1);
out->HeartBeat_Stat3 = (double)extract_little_endian_u64(data, 3, 1);
out->HeartBeat_Stat4 = (double)extract_little_endian_u64(data, 4, 1);
out->HeartBeat_Stat5 = (double)extract_little_endian_u64(data, 5, 1);
out->HeartBeat_Stat6 = (double)extract_little_endian_u64(data, 6, 1);
out->HeartBeat_Stat7 = (double)extract_little_endian_u64(data, 7, 1);
out->HeartBeat_Stat8 = (double)extract_little_endian_u64(data, 8, 1);
out->HeartBeat_Stat9 = (double)extract_little_endian_u64(data, 9, 1);
out->HeartBeat_Stat10 = (double)extract_little_endian_u64(data, 10, 1);
out->HeartBeat_Stat11 = (double)extract_little_endian_u64(data, 11, 1);
}


void decode_CANFRAME3(const uint8_t *data, CANFRAME3_t *out) {
if (!out) return;
out->CANFRAME3_sig0 = (double)extract_motorola_u64(data, 7, 17)*0.01-500;
uart_send_double(out->CANFRAME3_sig0,3);
uart_print_str("\n\r");
out->CANFRAME3_sig1 = (double)extract_motorola_u64(data, 35, 9);
uart_send_double(out->CANFRAME3_sig1,3);
uart_print_str("\n\r");
out->CANFRAME3_sig2 = (double)extract_motorola_u64(data, 41, 8);
uart_send_double(out->CANFRAME3_sig2,3);
uart_print_str("\n\r");
out->CANFRAME3_sig3 = (double)extract_motorola_u64(data, 28, 9);
uart_send_double(out->CANFRAME3_sig3,3);
uart_print_str("\n\r");

out->CANFRAME3_sig4 = (double)extract_motorola_u64(data, 22, 10)*0.1;
uart_send_double(out->CANFRAME3_sig4,3);
uart_print_str("\n\r");

uart_print_str("done \n\r");

}


void decode_MCU_Stat_One(const uint8_t *data, MCU_Stat_One_t *out) {
if (!out) return;
out->MCU_Stat_One_sig0 = (double)extract_motorola_u64(data, 7, 2);
out->MCU_Stat_One_sig1 = (double)extract_motorola_u64(data, 5, 2);
out->MCU_Stat_One_sig2 = (double)extract_motorola_u64(data, 3, 2);
out->MCU_Stat_One_sig3 = (double)extract_motorola_u64(data, 1, 2);
out->MCU_Stat_One_sig4 = (double)extract_motorola_u64(data, 56, 2);
out->MCU_Stat_One_sig5 = (double)extract_motorola_u64(data, 54, 2);
out->MCU_Stat_One_sig6 = (double)extract_motorola_u64(data, 48, 8);
}


void decode_MCU_FAULT_two(const uint8_t *data, MCU_FAULT_two_t *out) {
if (!out) return;
out->MCU_FAULT_two_sig0 = (double)extract_little_endian_u64(data, 0, 8);
out->MCU_FAULT_two_sig1 = (double)extract_little_endian_u64(data, 8, 8);
out->MCU_FAULT_two_sig2 = (double)extract_little_endian_u64(data, 16, 8);
out->MCU_FAULT_two_sig3 = (double)extract_little_endian_u64(data, 24, 8);
out->MCU_FAULT_two_sig4 = (double)extract_little_endian_u64(data, 32, 8);
out->MCU_FAULT_two_sig5 = (double)extract_little_endian_u64(data, 40, 8);
out->MCU_FAULT_two_sig6 = (double)extract_little_endian_u64(data, 48, 8);
out->MCU_FAULT_two_sig7 = (double)extract_little_endian_u64(data, 56, 8);
out->MCU_FAULT_two_sig8 = (double)extract_little_endian_u64(data, 64, 8);
out->MCU_FAULT_two_sig9 = (double)extract_little_endian_u64(data, 72, 8);
out->MCU_FAULT_two_sig10 = (double)extract_little_endian_u64(data, 80, 8);
out->MCU_FAULT_two_sig11 = (double)extract_little_endian_u64(data, 88, 8);
out->MCU_FAULT_two_sig12 = (double)extract_little_endian_u64(data, 96, 8);
out->MCU_FAULT_two_sig13 = (double)extract_little_endian_u64(data, 104, 8);
out->MCU_FAULT_two_sig14 = (double)extract_little_endian_u64(data, 112, 8);
}


void decode_MCU_FAULT_One(const uint8_t *data, MCU_FAULT_One_t *out) {
if (!out) return;
out->MCU_FAULT_One_sig0 = (double)extract_little_endian_u64(data, 0, 8);
out->MCU_FAULT_One_sig1 = (double)extract_little_endian_u64(data, 8, 8);
out->MCU_FAULT_One_sig2 = (double)extract_little_endian_u64(data, 16, 8);
out->MCU_FAULT_One_sig3 = (double)extract_little_endian_u64(data, 24, 8);
out->MCU_FAULT_One_sig4 = (double)extract_little_endian_u64(data, 32, 8);
out->MCU_FAULT_One_sig5 = (double)extract_little_endian_u64(data, 40, 8);
out->MCU_FAULT_One_sig6 = (double)extract_little_endian_u64(data, 48, 8);
out->MCU_FAULT_One_sig7 = (double)extract_little_endian_u64(data, 56, 8);
out->MCU_FAULT_One_sig8 = (double)extract_little_endian_u64(data, 64, 8);
out->MCU_FAULT_One_sig9 = (double)extract_little_endian_u64(data, 72, 8);
out->MCU_FAULT_One_sig10 = (double)extract_little_endian_u64(data, 80, 8);
out->MCU_FAULT_One_sig11 = (double)extract_little_endian_u64(data, 88, 8);
out->MCU_FAULT_One_sig12 = (double)extract_little_endian_u64(data, 96, 8);
out->MCU_FAULT_One_sig13 = (double)extract_little_endian_u64(data, 104, 8);
out->MCU_FAULT_One_sig14 = (double)extract_little_endian_u64(data, 112, 8);
out->MCU_FAULT_One_sig15 = (double)extract_little_endian_u64(data, 120, 8);
}


void decode_MCU_POWER_One(const uint8_t *data, MCU_POWER_One_t *out) {
if (!out) return;

out->power_sig0 = (double)extract_little_endian_u64(data, 0, 8);
out->power_sig1 = (double)extract_little_endian_u64(data, 8, 8);
out->power_sig2 = (double)extract_little_endian_u64(data, 16, 8);
out->power_sig3 = (double)extract_little_endian_u64(data, 24, 8);
out->power_sig4 = (double)extract_little_endian_u64(data, 32, 8);
}
void mcu_can_messages(){
}
