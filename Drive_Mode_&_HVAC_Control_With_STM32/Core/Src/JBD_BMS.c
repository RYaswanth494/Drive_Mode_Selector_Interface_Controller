/*
 * JBD_BMS.c
 *
 *  Created on: Sep 17, 2025
 *      Author: MadaYaswanth
 */


#include"JBD_BMS.h"
#include"can.h"
JBD_BMS_MSG_t Bms_Frames;


void Process_Jbd_Bms_Messages(can_frame_t *frame){
 switch(frame->id){
 case 0x100:decode_CAN_0x100(frame->data,&Bms_Frames.Id_0x100);
	 	 	 break;
 case 0x101:decode_CAN_0x101(frame->data,&Bms_Frames.Id_0x101);
	 	 	 break;
 case 0x102:decode_CAN_0x102(frame->data,&Bms_Frames.Id_0x102);
	 	 	 break;
 case 0x103:decode_CAN_0x103(frame->data,&Bms_Frames.Id_0x103);
	 	 	 break;
 case 0x104:decode_CAN_0x104(frame->data,&Bms_Frames.Id_0x104);
	 	 	 break;
 case 0x105:decode_CAN_0x105(frame->data,&Bms_Frames.Id_0x105);
	 	 	 break;
 case 0x106:decode_CAN_0x106(frame->data,&Bms_Frames.Id_0x106);
	 	 	 break;
 case 0x107:decode_CAN_0x107(frame->data,&Bms_Frames.Id_0x107);
	 	 	 break;
 case 0x108:decode_CAN_0x108(frame->data,&Bms_Frames.Id_0x108);
	 	 	 break;
 case 0x109:decode_CAN_0x109(frame->data,&Bms_Frames.Id_0x109);
	 	 	 break;
 case 0x10A:decode_CAN_0x10A(frame->data,&Bms_Frames.Id_0x10A);
	 	 	 break;
 case 0X10B:decode_CAN_0x10B(frame->data,&Bms_Frames.Id_0x10B);
	 	 	 break;
 case 0X10C:decode_CAN_0x10C(frame->data,&Bms_Frames.Id_0x10C);
	 	 	 break;
 default :break;
 }
}

