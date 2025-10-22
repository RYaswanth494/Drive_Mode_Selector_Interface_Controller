/*
 * process.c
 *
 *  Created on: Sep 6, 2025
 *      Author: MadaYaswanth
 */

#include"extern_file_declarations.h"
#include"can.h"
#include"uart.h"
#include"MCU_ELECTROCATLYST.h"
#include"process.h"
#include"JBD_BMS.h"
#include"Drive_Selector_Switch.h"
#include"TURN_ON_OFF_CHARGER_RELAY.h"
extern DBC_AllMessages_t msg;
extern JBD_BMS_MSG_t Bms_Frames;
extern volatile uint8_t bus_ok;
extern volatile uint32_t mcu_connected;
extern volatile uint32_t bms_connected;
extern volatile uint32_t mcu_timer;
extern volatile uint32_t bms_timer;
switch_state_t last_state=0,cur_state;
uint16_t id[]={0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107,0x108,0x109,0x10a,0x10b,0x10c};
uint8_t id_size=sizeof(id)/sizeof(id[0]);
extern float cells_array[16];
extern bool ar[6];
uint8_t crc8(uint8_t data) {
    uint8_t crc = 0x00; // initial value
    for (uint8_t i = 0; i < 8; i++) {
        if ((crc ^ data) & 0x80)
            crc = (crc << 1) ^ 0x07; // polynomial 0x07
        else
            crc <<= 1;
        data <<= 1;
    }
    return crc;
}
float find_smallest_float_in_single_pass(float arr[], int size) {
	if(size==16){
		float smallest = arr[0];
		for (int i = 1; i < size; i++) {
			if (arr[i] < smallest) {
				smallest = arr[i];
			}
		}
		return (float)((int)(smallest*100.0))/100.0;
	}
	return -1.0;
}
float find_largest_element(float arr[], int size) {
	if(size==16){
		float largest = arr[0];
	    for (int i = 1; i < size; i++) {
	         if (arr[i] > largest) {
	            largest = arr[i];
	        }
	    }
	    return (float)((int)(largest*100.0))/100.0;
	}
	return -1.0;
}
void Drive_mode_state(){
	if(I2C_RX_FLAG){
		I2C_RX_FLAG=0;
		if(crc8(data[0])==data[1]){
	    GPIOB->ODR &= ~((1<<PB0_REVERSE) | (1<<PB1_NEUTRAL) | (1<<PB3_DRIVE) | (1<<PB5_SPORT));
	    switch (data[0]) {
	        case 'R': GPIOB->ODR |= (1<<PB0_REVERSE); break;
	        case 'N': GPIOB->ODR |= (1<<PB1_NEUTRAL); break;
	        case 'D': GPIOB->ODR |= (1<<PB3_DRIVE);   break;
	        case 'S': GPIOB->ODR |= (1<<PB5_SPORT);   break;
	    }
		}
	}
}
void process_can_messages(void)
{
   can_frame_t rx_frame;
   if(CAN_MessagePending(0)){
	   mcu_timer = 0;
       mcu_connected = 1;
	   can_rx(&rx_frame,0);
	   send_id_data_only_over_uart(&rx_frame);
//	   Process_Jbd_Bms_Messages(&rx_frame);
	   matel_mcu_process_can_frame(&rx_frame);
   }
   if(CAN_MessagePending(1)){
	   bms_timer = 0;
       bms_connected = 1;
	   can_rx(&rx_frame,1);
	   send_id_data_only_over_uart(&rx_frame);
	   Process_Jbd_Bms_Messages(&rx_frame);
//	   matel_mcu_process_can_frame(&rx_frame);
   }
   check_can_bus();
}
void process_switch_status(){
		cur_state=Switch_update();
		if(cur_state!=last_state){
			last_state=cur_state;
			GPIOB->ODR |= ((1<<PB0_REVERSE) | (1<<PB1_NEUTRAL) | (1<<PB3_DRIVE));
			switch (cur_state) {
				case 2: GPIOB->ODR &=~ (1<<PB0_REVERSE); break;
				case 0: GPIOB->ODR &=~ (1<<PB1_NEUTRAL); break;
				case 1: GPIOB->ODR &=~ (1<<PB3_DRIVE);   break;
			}
		}
		can_frame_t rx_frame;
		rx_frame.id=0x123;
		rx_frame.dlc=8;
		rx_frame.data[0]=cur_state;
		rx_frame.data[1]=bus_ok;
		rx_frame.data[2]=mcu_connected;
		rx_frame.data[3]=bms_connected;
		send_id_data_only_over_uart(&rx_frame);
}
void Request_Msgs_to_BMS(){
	   can_frame_t rx_frame;
	   static uint8_t i=0;
	   rx_frame.id=id[i];
	   rx_frame.ide=0;
	   rx_frame.dlc=8;
	   send_can_remote_frame(rx_frame);
	   i++;
	   if(i>=id_size){
		   i=0;
	   }
}
void charger_relay_on_off(){
	if(ar[0]&&ar[1]&&ar[2]&&ar[3]&&ar[4]&&ar[5]){
		uart_printf("received all\n\r");
		float small=find_smallest_float_in_single_pass(cells_array,16);
		float large=find_largest_element(cells_array,16);
		uart_printf("small =%f  large =%f \n\r",small,large);
		if((small<2.5)||(large>3.5)){
			charge_relay_off();
		}
		else{
			charge_relay_on();
		}
	}
}
void print_mcu_data(){
	if(msg.MCU_POWER_One.rx_or_not){
		uart_printf(
		    "MCU_POWER_One: Sig0=%.2f Sig1=%.2f Sig2=%.2f Sig3=%llu Sig4=%.2f\n\r",
			msg.MCU_POWER_One.power_sig0,
		    msg.MCU_POWER_One.power_sig1,
		    msg.MCU_POWER_One.power_sig2,
		    (unsigned long long)msg.MCU_POWER_One.power_sig3,
		    msg.MCU_POWER_One.power_sig4
		);
		msg.MCU_POWER_One.rx_or_not=0;
	}
	if(msg.MCU_FAULT_One.rx_or_not){
		uart_printf(
		    "MCU_FAULT_One: S0=%d S1=%d S2=%d S3=%d S4=%d S5=%d S6=%d S7=%d "
		    "S8=%d S9=%d S10=%d S11=%d S12=%d S13=%d S14=%d S15=%d\n\r",
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig0,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig1,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig2,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig3,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig4,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig5,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig6,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig7,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig8,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig9,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig10,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig11,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig12,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig13,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig14,
		    msg.MCU_FAULT_One.MCU_FAULT_One_sig15
		);
		msg.MCU_FAULT_One.rx_or_not=0;
	}
	if(msg.MCU_FAULT_two.rx_or_not){
		uart_printf(
		    "MCU_FAULT_two: S0=%d S1=%d S2=%d S3=%d S4=%d S5=%d S6=%d "
		    "S7=%d S8=%d S9=%d S10=%d S11=%d S12=%d S13=%d S14=%d\n\r",
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig0,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig1,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig2,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig3,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig4,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig5,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig6,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig7,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig8,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig9,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig10,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig11,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig12,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig13,
		    msg.MCU_FAULT_two.MCU_FAULT_two_sig14
		);

		msg.MCU_FAULT_two.rx_or_not=0;
	}
	if(msg.MCU_Fault_Code.rx_or_not){
		uart_printf(
		    "MCU_Faults: F0=%d F1=%d F2=%d F3=%d F4=%d F5=%d F6=%d F7=%d "
		    "F8=%d F9=%d F10=%d F11=%d F12=%d F13=%d F14=%d F15=%d "
		    "F16=%d F17=%d F18=%d F19=%d F20=%d F21=%d F22=%d\n\r",
		    msg.MCU_Fault_Code.MCU_Fault_Code_0,
		    msg.MCU_Fault_Code.MCU_Fault_Code_1,
		    msg.MCU_Fault_Code.MCU_Fault_Code_2,
		    msg.MCU_Fault_Code.MCU_Fault_Code_3,
		    msg.MCU_Fault_Code.MCU_Fault_Code_4,
		    msg.MCU_Fault_Code.MCU_Fault_Code_5,
		    msg.MCU_Fault_Code.MCU_Fault_Code_6,
		    msg.MCU_Fault_Code.MCU_Fault_Code_7,
		    msg.MCU_Fault_Code.MCU_Fault_Code_8,
		    msg.MCU_Fault_Code.MCU_Fault_Code_9,
		    msg.MCU_Fault_Code.MCU_Fault_Code_10,
		    msg.MCU_Fault_Code.MCU_Fault_Code_11,
		    msg.MCU_Fault_Code.MCU_Fault_Code_12,
		    msg.MCU_Fault_Code.MCU_Fault_Code_13,
		    msg.MCU_Fault_Code.MCU_Fault_Code_14,
		    msg.MCU_Fault_Code.MCU_Fault_Code_15,
		    msg.MCU_Fault_Code.MCU_Fault_Code_16,
		    msg.MCU_Fault_Code.MCU_Fault_Code_17,
		    msg.MCU_Fault_Code.MCU_Fault_Code_18,
		    msg.MCU_Fault_Code.MCU_Fault_Code_19,
		    msg.MCU_Fault_Code.MCU_Fault_Code_20,
		    msg.MCU_Fault_Code.MCU_Fault_Code_21,
		    msg.MCU_Fault_Code.MCU_Fault_Code_22
		);
		msg.MCU_Fault_Code.rx_or_not=0;
	}
	if(msg.MCU_Stat_One.rx_or_not){
		msg.MCU_Stat_One.rx_or_not=0;
		uart_printf(
		    "MCU_Stat_One: sig0=%d sig1=%d sig2=%.1f sig3=%d sig4=%d sig5=%d sig6=%d\n\r",
		    msg.MCU_Stat_One.MCU_Stat_One_sig0,
		    msg.MCU_Stat_One.MCU_Stat_One_sig1,
		    msg.MCU_Stat_One.MCU_Stat_One_sig2,
		    msg.MCU_Stat_One.MCU_Stat_One_sig3,
		    msg.MCU_Stat_One.MCU_Stat_One_sig4,
		    msg.MCU_Stat_One.MCU_Stat_One_sig5,
		    msg.MCU_Stat_One.MCU_Stat_One_sig6
		);

	}
	if(msg.MCU_Stat_Two.rx_or_not){
	uart_printf("Odometer=%.1f km voltage=%.0f Motor_RPM=%.0f\n\r",msg.MCU_Stat_Two.MCU_Odometer_Val,msg.MCU_Stat_Two.MCU_VCU_State,msg.MCU_Stat_Two.MCU_Motor_RPM);
	msg.MCU_Stat_Two.rx_or_not=0;
	}
}
void print_bms_data(){
	if(Bms_Frames.Id_0x100.rx_or_not){
		Bms_Frames.Id_0x100.rx_or_not=0;
		uart_printf("0x100 -> Total_Voltage=%.2f V, Current=%.2f A, Remaining_Capacity=%.2f Ah, CRC=0x%d\n",
		Bms_Frames.Id_0x100.Total_Voltage, Bms_Frames.Id_0x100.Charge_and_Discharge_Current, Bms_Frames.Id_0x100.Remaining_Capacity, Bms_Frames.Id_0x100.CRC_Check);
	}
	if(Bms_Frames.Id_0x101.rx_or_not){
	   Bms_Frames.Id_0x101.rx_or_not=0;
	   uart_printf("0x101 -> Full_Capacity=%.2f Ah, No_Cycles=%d, RSOC=%d%%, CRC=0x%d\n",
			   Bms_Frames.Id_0x101.Full_Capacity, Bms_Frames.Id_0x101.No_Cycles, Bms_Frames.Id_0x101.RSOC, Bms_Frames.Id_0x101.CRC_Check);
	}
	if(Bms_Frames.Id_0x102.rx_or_not){
		Bms_Frames.Id_0x102.rx_or_not=0;
		uart_printf(
		        "0x102 -> EQ1=%d EQ2=%d EQ3=%d EQ4=%d EQ5=%d EQ6=%d EQ7=%d EQ8=%d "
		        "EQ9=%d EQ10=%d EQ11=%d EQ12=%d EQ13=%d EQ14=%d EQ15=%d EQ16=%d | "
		        "Prot=0x%X, CRC=0x%X\n",
		        Bms_Frames.Id_0x102.Equlization_Low_byte[0], Bms_Frames.Id_0x102.Equlization_Low_byte[1],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[2], Bms_Frames.Id_0x102.Equlization_Low_byte[3],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[4], Bms_Frames.Id_0x102.Equlization_Low_byte[5],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[6], Bms_Frames.Id_0x102.Equlization_Low_byte[7],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[8], Bms_Frames.Id_0x102.Equlization_Low_byte[9],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[10], Bms_Frames.Id_0x102.Equlization_Low_byte[11],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[12], Bms_Frames.Id_0x102.Equlization_Low_byte[13],
		        Bms_Frames.Id_0x102.Equlization_Low_byte[14], Bms_Frames.Id_0x102.Equlization_Low_byte[15],
		        Bms_Frames.Id_0x102.Protection_Status,
		        Bms_Frames.Id_0x102.CRC_Check
		    );
	}
	if(Bms_Frames.Id_0x103.rx_or_not){
		Bms_Frames.Id_0x103.rx_or_not=0;
		uart_printf("0x103 -> FET_Status=0x%d, Prod_Date=0x%d, SW_Version=0x%d, CRC=0x%d\n",
				Bms_Frames.Id_0x103.FET_Control_Status,Bms_Frames.Id_0x103.Production_Date, Bms_Frames.Id_0x103.Software_Version, Bms_Frames.Id_0x103.CRC_Check);
	}
	if(Bms_Frames.Id_0x104.rx_or_not){
		Bms_Frames.Id_0x104.rx_or_not=0;
		uart_printf("0x104 -> Battery_Strings=%d, NTC_Probes=%d, CRC=0x%X\n",
				Bms_Frames.Id_0x104.Number_Of_Battery_Strings, Bms_Frames.Id_0x104.NTC_Number_Probes,Bms_Frames.Id_0x104.CRC_Check);
	}
	if(Bms_Frames.Id_0x105.rx_or_not){
		Bms_Frames.Id_0x105.rx_or_not=0;
		uart_printf("0x105 -> NTC1=%.2f°C, NTC2=%.2f°C, NTC3=%.2f°C, CRC=0x%X\n",
		Bms_Frames.Id_0x105.NTC1,Bms_Frames.Id_0x105.NTC2, Bms_Frames.Id_0x105.NTC3, Bms_Frames.Id_0x105.CRC_Check);
	}
	if(Bms_Frames.Id_0x106.rx_or_not){
		Bms_Frames.Id_0x106.rx_or_not=0;
		uart_printf("0x106 -> NTC4=%.2f°C, NTC5=%.2f°C,  CRC=0x%X\n",
				Bms_Frames.Id_0x106.NTC4, Bms_Frames.Id_0x106.NTC5,  Bms_Frames.Id_0x106.CRC_Check);
	}
	if(Bms_Frames.Id_0x107.rx_or_not){
		Bms_Frames.Id_0x107.rx_or_not=0;
		uart_printf("0x107 -> CELL1=%.3fV, CELL2=%.3fV, CELL3=%.3fV, CRC=0x%X\n",
				Bms_Frames.Id_0x107.CELL1, Bms_Frames.Id_0x107.CELL2, Bms_Frames.Id_0x107.CELL3, Bms_Frames.Id_0x107.CRC_Check);
	}
	if(Bms_Frames.Id_0x108.rx_or_not){
		Bms_Frames.Id_0x108.rx_or_not=0;
		 uart_printf("0x108 -> CELL4=%.3fV, CELL5=%.3fV, CELL6=%.3fV, CRC=0x%X\n",
				 Bms_Frames.Id_0x108.CELL4, Bms_Frames.Id_0x108.CELL5, Bms_Frames.Id_0x108.CELL6, Bms_Frames.Id_0x108.CRC_Check);
	}
	if(Bms_Frames.Id_0x109.rx_or_not){
		Bms_Frames.Id_0x109.rx_or_not=0;
		uart_printf("0x109 -> CELL7=%.3fV, CELL8=%.3fV, CELL9=%.3fV, CRC=0x%X\n",
				Bms_Frames.Id_0x109.CELL7, Bms_Frames.Id_0x109.CELL8, Bms_Frames.Id_0x109.CELL9, Bms_Frames.Id_0x109.CRC_Check);
	}
	if(Bms_Frames.Id_0x10A.rx_or_not){
		Bms_Frames.Id_0x10A.rx_or_not=0;
		uart_printf("0x10A -> CELL10=%.3fV, CELL11=%.3fV, CELL12=%.3fV, CRC=0x%X\n",
				Bms_Frames.Id_0x10A.CELL10, Bms_Frames.Id_0x10A.CELL11, Bms_Frames.Id_0x10A.CELL12, Bms_Frames.Id_0x10A.CRC_Check);
	}
	if(Bms_Frames.Id_0x10B.rx_or_not){
		Bms_Frames.Id_0x10B.rx_or_not=0;
		uart_printf("0x10B -> CELL13=%.3fV, CELL14=%.3fV, CELL15=%.3fV, CRC=0x%X\n",
				Bms_Frames.Id_0x10B.CELL13, Bms_Frames.Id_0x10B.CELL14, Bms_Frames.Id_0x10B.CELL15, Bms_Frames.Id_0x10B.CRC_Check);
	}
	if(Bms_Frames.Id_0x10C.rx_or_not){
		Bms_Frames.Id_0x10C.rx_or_not=0;
		uart_printf("0x10C -> CELL16=%.3fV,\n",
		Bms_Frames.Id_0x10C.CELL16);
	}
}
