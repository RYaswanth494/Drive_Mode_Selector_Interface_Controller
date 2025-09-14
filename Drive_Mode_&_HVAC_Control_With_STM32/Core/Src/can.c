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
#define CAN_MAX_FILTER_BANKS 14

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
    return RY_OK;//0x001e0003
}
void Gpio_init(){
    RCC->APB1ENR|=RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
    GPIOB->CRH &= ~(0xF << 4); // Clear bits for PB9
    GPIOB->CRH &= ~(0xF << 0); // Clear bits for PB8
    GPIOB->CRH|=(1<<2);
    GPIOB->CRH|=(0Xb0);
    AFIO->MAPR|=AFIO_MAPR_CAN_REMAP_REMAP2;
}
STATUS can_init(uint32_t Baud_Rate){
	Gpio_init();
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
	CAN1->FMR |= CAN_FMR_FINIT;       // Enter filter init mode

	CAN1->FA1R = 0;                   // Disable all filters
	CAN1->FM1R = 0;                   // Identifier Mask mode
	CAN1->FS1R = 1;                   // 32-bit scale
	CAN1->FFA1R = 0;                  // Assign to FIFO0

	CAN1->sFilterRegister[0].FR1 = 0x00000000;  // ID = 0
	CAN1->sFilterRegister[0].FR2 = 0x00000000;  // Mask = 0

	CAN1->FA1R |= 1;                  // Enable filter 0

	CAN1->FMR &= ~CAN_FMR_FINIT;
	return RY_OK;
}
void send_can(can_frame_t frame){
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
	if(frame.ide==0){
		CAN1->sTxMailBox[mail_box].TIR |= (frame.id<<21);

	}else{
		CAN1->sTxMailBox[mail_box].TIR |= (frame.id<<3)|(1<<2);

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
void configure_can_filters(const uint32_t *std_ids, uint8_t std_cnt,const uint32_t *ext_ids, uint8_t ext_cnt) {

    uint8_t bank = 0;

    // Enter filter init mode
    CAN1->FMR |= CAN_FMR_FINIT;

    // --- Standard IDs: use banks from 0 upwards ---
    for (uint8_t i = 0; i < std_cnt; i += 2) {
        if (bank >= CAN_MAX_FILTER_BANKS) break;

        // Deactivate bank while configuring
        CAN1->FA1R &= ~(1u << bank);

        // Identifier list mode (exact IDs)
        CAN1->FM1R |= (1u << bank);

        // 32-bit scale
        CAN1->FS1R |= (1u << bank);

        // Assign to FIFO0
        CAN1->FFA1R &= ~(1u << bank);

        // Prepare two 32-bit slots (FR1 and FR2)
        uint32_t fr1 = 0, fr2 = 0;

        // first ID into FR1
        uint32_t id1 = (std_ids[i] & 0x7FFu) << 21; // std ID in bits [31:21]
        fr1 = id1;

        // second ID into FR2 (if exists)
        if ((i + 1) < std_cnt) {
            uint32_t id2 = (std_ids[i + 1] & 0x7FFu) << 21;
            fr2 = id2;
        }

        CAN1->sFilterRegister[bank].FR1 = fr1;
        CAN1->sFilterRegister[bank].FR2 = fr2;

        // Activate bank
        CAN1->FA1R |= (1u << bank);
        bank++;
    }

    // --- Extended IDs: continue with remaining banks ---
    for (uint8_t j = 0; j < ext_cnt; j += 2) {
        if (bank >= CAN_MAX_FILTER_BANKS) break;

        // Deactivate bank while configuring
        CAN1->FA1R &= ~(1u << bank);

        // Identifier list mode
        CAN1->FM1R |= (1u << bank);

        // 32-bit scale
        CAN1->FS1R |= (1u << bank);

        // Assign to FIFO1 for extended IDs
        CAN1->FFA1R |= (1u << bank);

        uint32_t fr1 = 0, fr2 = 0;

        // FR1: ext id1 encoded as (ID << 3) + IDE bit in bit2
        uint32_t e1 = ((ext_ids[j] & 0x1FFFFFFFu) << 3) | (1u << 2);
        fr1 = e1;

        if ((j + 1) < ext_cnt) {
            uint32_t e2 = ((ext_ids[j + 1] & 0x1FFFFFFFu) << 3) | (1u << 2);
            fr2 = e2;
        }

        CAN1->sFilterRegister[bank].FR1 = fr1;
        CAN1->sFilterRegister[bank].FR2 = fr2;

        // Activate bank
        CAN1->FA1R |= (1u << bank);
        bank++;
    }

    // Leave filter init mode
    CAN1->FMR &= ~CAN_FMR_FINIT;
}
//void send_full_frame_over_uart( can_frame_t *f) {
//    uart_send((uint8_t*)f, sizeof(can_frame_t));
//}
/**
 * @brief Checks and receives a CAN message from a specified FIFO.
 * This function reads a CAN message from either FIFO0 or FIFO1 and populates
 * the provided CanRxMsg structure.
 * @param fifo_number The FIFO to check (0 for FIFO0, 1 for FIFO1).
 * @param rx_message A pointer to the CanRxMsg structure to store the received data.
 * @return 1 if a message was successfully received, 0 otherwise.
 */
uint8_t CAN_MessagePending(uint8_t fifo)
{
    if (fifo == 0)
        return (CAN1->RF0R & CAN_RF0R_FMP0_Msk) ? 1 : 0;
    else
        return (CAN1->RF1R & CAN_RF1R_FMP1_Msk) ? 1 : 0;
}

uint8_t can_rx(can_frame_t *RxMessage,uint8_t fifo_number){
	  RxMessage->ide = (uint8_t)0x04 & CAN1->sFIFOMailBox[fifo_number].RIR;
	  if (RxMessage->ide == 0)
	  {
	    RxMessage->id = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[fifo_number].RIR >> 21);
	  }
	  else
	  {
	    RxMessage->id = (uint32_t)0x1FFFFFFF & (CAN1->sFIFOMailBox[fifo_number].RIR >> 3);
	  }

	  RxMessage->rtr = (uint8_t)0x02 & CAN1->sFIFOMailBox[fifo_number].RIR;
	  /* Get the DLC */
	  RxMessage->dlc = (uint8_t)0x0F & CAN1->sFIFOMailBox[fifo_number].RDTR;
	  /* Get the data field */
	  for(uint8_t i=0;i< RxMessage->dlc;i++){
		  if(i<4){
			  RxMessage->data[i] = (CAN1->sFIFOMailBox[fifo_number].RDLR>>(i*8));
		  }else{
			  RxMessage->data[i] = (CAN1->sFIFOMailBox[fifo_number].RDHR>>(((i-4)*8)));

		  }
	  }
//	  send_full_frame_over_uart(&RxMessage);
	  if (fifo_number == 0)
	  {
		  CAN1->RF0R |= CAN_RF0R_RFOM0;
	  }
	  else if (fifo_number == 1)
	  {
		  CAN1->RF1R |= CAN_RF1R_RFOM1;
	  }
	  return 1;
}
uint32_t can_recv_bulk(can_frame_t *frames, uint32_t max_frames)
{
    uint32_t frames_received = 0;

    if ( frames == NULL || max_frames == 0)
        return 0;

    while (frames_received < max_frames)
    {
        if ((CAN1->RF0R & CAN_RF0R_FMP0) > 0)
        {
            if (receive_can_data(0,&frames[frames_received]))
                frames_received++;
        }
        else if ((CAN1->RF1R & CAN_RF1R_FMP1)>0)
        {
            if (receive_can_data(1,&frames[frames_received]))
                frames_received++;
        }
        else
        {
            break; // No messages in either FIFO
        }
    }

    return frames_received;
}
