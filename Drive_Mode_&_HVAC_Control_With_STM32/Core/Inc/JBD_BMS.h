/*
 * JBD_BMS.h
 *
 *  Created on: Sep 17, 2025
 *      Author: MadaYaswanth
 */

#ifndef JBD_BMS_H_
#define JBD_BMS_H_

#include"JBD_BMS_MESSAGES.h"
#include"can.h"
typedef struct{
	CAN_BMS_0x100_t Id_0x100;
	CAN_BMS_0x101_t Id_0x101;
	CAN_BMS_0x102_t Id_0x102;
	CAN_BMS_0x103_t Id_0x103;
	CAN_BMS_0x104_t Id_0x104;
	CAN_BMS_0x105_t Id_0x105;
	CAN_BMS_0x106_t Id_0x106;
	CAN_BMS_0x107_t Id_0x107;
	CAN_BMS_0x108_t Id_0x108;
	CAN_BMS_0x109_t Id_0x109;
	CAN_BMS_0x10A_t Id_0x10A;
	CAN_BMS_0x10B_t Id_0x10B;
	CAN_BMS_0x10C_t Id_0x10C;
	CAN_BMS_0x10D_t Id_0x10D;
	CAN_BMS_0x10E_t Id_0x10E;
	CAN_BMS_0x10F_t Id_0x10F;
	CAN_BMS_0x110_t Id_0x110;
}JBD_BMS_MSG_t;
void Process_Jbd_Bms_Messages(can_frame_t *);
#endif /* JBD_BMS_H_ */
