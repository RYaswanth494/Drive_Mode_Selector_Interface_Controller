/*
 * can.c
 *
 *  Created on: Sep 2, 2025
 *      Author: MadaYaswanth
 */

#include"main.h"
#include"can.h"
#include"uart.h"
#include"system_clock_init.h"


typedef struct {
    uint16_t prescaler;
    uint32_t tseg1;
    uint32_t tseg2;
    uint32_t sjw;
    float sample_point;  // in percentage
} CAN_BitTiming;

STATUS set_baudrate(uint32_t Baud_Rate){
    CAN_BitTiming table[100]; // store results
    int count = 0;
    uint32_t clk = get_APB1_freq();      // 36 MHz
    uint32_t baud = Baud_Rate;      // 500 kbps
    if (clk == 0 || Baud_Rate == 0) {
        return RY_NOT_OK;
    }
    uint32_t target = clk / baud; // should be 16
    for (uint32_t prescaler = 1; prescaler <= 1024; prescaler++) {
        for (uint32_t tseg1 = 1; tseg1 <= 16; tseg1++) {
            for (uint32_t tseg2 = 1; tseg2 <= 8; tseg2++) {
            	uint32_t tq = 1 + tseg1 + tseg2;
                if (prescaler * tq == target) {
                    float sp = ((1.0f + tseg1) / tq) * 100.0f;
                    if (sp >= 70.0f && sp <= 80.0f) {
                        for (int sjw = 1; sjw <= tseg2; sjw++) {
                            table[count].prescaler = prescaler;
                            table[count].tseg1 = tseg1;
                            table[count].tseg2 = tseg2;
                            table[count].sjw = sjw;
                            table[count].sample_point = sp;
                            count++;
                        }
                    }
                }
            }
        }
    }
    CAN1->BTR|=((table[0].prescaler-1)<<0)|((table[0].sjw-1)<<24)|((table[0].tseg1-1)<<16)|((table[0].tseg2-1)<<20);
    return RY_OK;
}
STATUS can_init(uint32_t Baud_Rate){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_REMAP2;
    GPIOB->CRH &= ~(0xF << 0);
    GPIOB->CRH |=  (0x4 << 0);
    GPIOB->CRH &= ~(0xF << 4);
    GPIOB->CRH |=  (0xB << 4);
	CAN1->MCR &= ~CAN_MCR_AWUM_Msk;
	CAN1->MCR &= ~CAN_MCR_SLEEP_Msk;
	CAN1->MCR |= CAN_MCR_INRQ_Msk;
	while (!(CAN1->MSR & CAN_MSR_INAK_Msk));
	if(set_baudrate(Baud_Rate)==RY_NOT_OK)
	{
		RY_NOT_OK;
	}
	CAN1->MCR &= ~(1<<0);  // CLEAR INQR BIT TO LEAVE INTILAIZE MODE=
	while (CAN1->MSR & CAN_MSR_INAK_Msk);  // Wait for Normal Mode
	return RY_OK;
}
void send_can(CAN_FRAME frame){
	uint32_t tme_mask = CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2);
	int mail_box = -1;
    if (tme_mask & CAN_TSR_TME0) {
        mail_box = 0;
    } else if (tme_mask & CAN_TSR_TME1) {
        mail_box = 1;
    } else if (tme_mask & CAN_TSR_TME2) {
        mail_box = 2;
    }
	CAN1->sTxMailBox[mail_box].TIR = 0x00000000;
	switch(frame.id_type)
	{
		case 0:
			CAN1->sTxMailBox[mail_box].TIR |= (frame.id<<21);
			break;
		case 1:
			CAN1->sTxMailBox[mail_box].TIR |= (frame.id<<3)|(1<<2);
			break;
	}
	CAN1->sTxMailBox[mail_box].TDTR &= ~(0xF<<0);
	CAN1->sTxMailBox[mail_box].TDTR |= (frame.dlc<<0);
	CAN1->sTxMailBox[mail_box].TDLR = 0x00000000;
	CAN1->sTxMailBox[mail_box].TDHR = 0x00000000;
	 for(uint8_t i=0;i<frame.dlc;i++){
		 if(i<4){
			 CAN1->sTxMailBox[mail_box].TDLR|=(frame.data[i]<<(i*8));
		 }else{
			 CAN1->sTxMailBox[mail_box].TDHR|=(frame.data[i]<<((i-4)*8));
		 }
	 }
	CAN1->sTxMailBox[mail_box].TIR |= (1<<0);
	}
void can_set_filter(uint32_t *std_ids,uint32_t *ext_ids ){

}
