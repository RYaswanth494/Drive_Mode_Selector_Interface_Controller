/*
 * JBD_BMS.c
 *
 *  Created on: Sep 17, 2025
 *      Author: MadaYaswanth
 */


#include"JBD_BMS.h"
#include"can.h"
#include"uart.h"
JBD_BMS_MSG_t Bms_Frames;
void jump_to_bootloader(void) {
	FLASH_WRITE_FLAG(BL_UPDATE_REQUIRED);
    // 1. Read the MSP and Reset Handler from application vector table
    uint32_t boot_stack = *(volatile uint32_t*)(BOOT_ADDRESS);
    uint32_t boot_reset = *(volatile uint32_t*)(BOOT_ADDRESS + 4);
    // 2. Set the Vector Table base
    SCB->VTOR = BOOT_ADDRESS;
    // 3. Set the Main Stack Pointer
    __set_MSP(boot_stack);
    // 4. Jump to the application's Reset Handler
    void (*boot_entry)(void) = (void*)boot_reset;
    boot_entry();
}
void Process_Jbd_Bms_Messages(can_frame_t *frame){
 switch(frame->id){
 case 0x100:decode_CAN_0x100(frame->data,&Bms_Frames.Id_0x100);
 Bms_Frames.Id_0x100.rx_or_not=1;
	 	 	 break;
 case 0x101:decode_CAN_0x101(frame->data,&Bms_Frames.Id_0x101);
 Bms_Frames.Id_0x101.rx_or_not=1;
	 	 	 break;
 case 0x102:decode_CAN_0x102(frame->data,&Bms_Frames.Id_0x102);
 Bms_Frames.Id_0x102.rx_or_not=1;
	 	 	 break;
 case 0x103:decode_CAN_0x103(frame->data,&Bms_Frames.Id_0x103);
 Bms_Frames.Id_0x103.rx_or_not=1;
	 	 	 break;
 case 0x104:decode_CAN_0x104(frame->data,&Bms_Frames.Id_0x104);
 Bms_Frames.Id_0x104.rx_or_not=1;
	 	 	 break;
 case 0x105:decode_CAN_0x105(frame->data,&Bms_Frames.Id_0x105);
 Bms_Frames.Id_0x105.rx_or_not=1;
	 	 	 break;
 case 0x106:decode_CAN_0x106(frame->data,&Bms_Frames.Id_0x106);
 Bms_Frames.Id_0x106.rx_or_not=1;
	 	 	 break;
 case 0x107:decode_CAN_0x107(frame->data,&Bms_Frames.Id_0x107);
 Bms_Frames.Id_0x107.rx_or_not=1;
	 	 	 break;
 case 0x108:decode_CAN_0x108(frame->data,&Bms_Frames.Id_0x108);
 Bms_Frames.Id_0x108.rx_or_not=1;
	 	 	 break;
 case 0x109:decode_CAN_0x109(frame->data,&Bms_Frames.Id_0x109);
 Bms_Frames.Id_0x109.rx_or_not=1;
	 	 	 break;
 case 0x10A:decode_CAN_0x10A(frame->data,&Bms_Frames.Id_0x10A);
 Bms_Frames.Id_0x10A.rx_or_not=1;
	 	 	 break;
 case 0X10B:decode_CAN_0x10B(frame->data,&Bms_Frames.Id_0x10B);
 Bms_Frames.Id_0x10B.rx_or_not=1;
	 	 	 break;
 case 0X10C:decode_CAN_0x10C(frame->data,&Bms_Frames.Id_0x10C);
 Bms_Frames.Id_0x10C.rx_or_not=1;
	 	 	 break;
 case 0x1234:jump_to_bootloader();

 default :break;
 }
}

