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
void configure_can_filters(const uint16_t *std_ids, uint8_t std_count,const uint32_t *ext_ids, uint8_t ext_count) {

	 // Check if the total number of filters exceeds the available banks
	    if ((std_count + ext_count) > CAN_FILTER_BANKS) {
	        // In a real application, you would handle this error more gracefully
	        return;
	    }

	    // 1. Enter filter initialization mode
	    // Set the FINIT bit (Filter Initialization Mode)
	    CAN1->FMR |= CAN_FMR_FINIT;

	    uint8_t filter_bank_index = 0;

	    // 2. Configure filters for Standard IDs and assign to FIFO0
	    for (uint8_t i = 0; i < std_count; i++) {
	        // Set filter mode to Mask mode
	        CAN1->FM1R |= (1 << filter_bank_index);

	        // Set filter scale to 16-bit
	        CAN1->FS1R &= ~(1 << filter_bank_index);

	        // Assign the filter to FIFO0 (clear the bit in FFA1R)
	        CAN1->FFA1R &= ~(1 << filter_bank_index);

	        // Configure the filter ID and mask registers
	        // Standard ID is shifted left by 5 bits to align with the hardware format
	        CAN1->sFilterRegister[filter_bank_index].FR1 = (std_ids[i] << 5);
	        // The mask is set to accept only this specific ID
	        CAN1->sFilterRegister[filter_bank_index].FR2 = (0x7FF << 5);

	        // Enable the filter bank
	        CAN1->FA1R |= (1 << filter_bank_index);

	        filter_bank_index++;
	    }

	    // 3. Configure filters for Extended IDs and assign to FIFO1
	    for (uint8_t i = 0; i < ext_count; i++) {
	        // Set filter mode to Mask mode
	        CAN1->FM1R |= (1 << filter_bank_index);

	        // Set filter scale to 32-bit
	        CAN1->FS1R |= (1 << filter_bank_index);

	        // Assign the filter to FIFO1 (set the bit in FFA1R)
	        CAN1->FFA1R |= (1 << filter_bank_index);

	        // The 29-bit ID is split into two 16-bit registers
	        // FR1: Contains the most significant 16 bits of the ID
	        // FR2: Contains the least significant 16 bits of the ID
	        CAN1->sFilterRegister[filter_bank_index].FR1 = (uint16_t)(ext_ids[i] >> 16);
	        CAN1->sFilterRegister[filter_bank_index].FR2 = (uint16_t)ext_ids[i];

	        // Enable the filter bank
	        CAN1->FA1R |= (1 << filter_bank_index);

	        filter_bank_index++;
	    }

	    // 4. Exit filter initialization mode to activate the filters
	    CAN1->FMR &= ~CAN_FMR_FINIT;
}
/**
 * @brief Checks and receives a CAN message from a specified FIFO.
 * This function reads a CAN message from either FIFO0 or FIFO1 and populates
 * the provided CanRxMsg structure.
 * @param fifo_number The FIFO to check (0 for FIFO0, 1 for FIFO1).
 * @param rx_message A pointer to the CanRxMsg structure to store the received data.
 * @return 1 if a message was successfully received, 0 otherwise.
 */
uint8_t receive_can_data(uint8_t fifo_number, can_frame_t* rx_message) {

    // Check if the requested FIFO has a pending message
    if (fifo_number == 0) {
        // Check FMP0 bit (FIFO Message Pending)
        if ((CAN1->RF0R & CAN_RF0R_FMP0) == 0) {
            return 0; // No message pending in FIFO0
        }
    } else if (fifo_number == 1) {
        // Check FMP1 bit (FIFO Message Pending)
        if ((CAN1->RF1R & CAN_RF1R_FMP1) == 0) {
            return 0; // No message pending in FIFO1
        }
    } else {
        return 0; // Invalid FIFO number
    }

    // Read message from the selected FIFO
    volatile CAN_FIFOMailBox_TypeDef* rx_mailbox;
    if (fifo_number == 0) {
        rx_mailbox = &CAN1->sFIFOMailBox[0];
    } else {
        rx_mailbox = &CAN1->sFIFOMailBox[1];
    }

    // Read the ID and ID type
    if ((rx_mailbox->RIR & CAN_RI0R_IDE) == 0) {
        // Standard ID (11-bit)
        rx_message->ide = 0;
        rx_message->id = (rx_mailbox->RIR >> 21) & 0x7FF;
    } else {
        // Extended ID (29-bit)
        rx_message->ide = 1;
        rx_message->id = (rx_mailbox->RIR >> 3);
    }

    // Check for Remote Transmission Request (RTR)
    rx_message->rtr = (rx_mailbox->RIR & CAN_RI0R_RTR) >> 1;

    // Read Data Length Code (DLC)
    rx_message->dlc = (rx_mailbox->RDTR & CAN_RDT0R_DLC) >> 0;

    // Read the 8 bytes of data
    // Use bit-wise operations to get the correct data
    rx_message->data[0] = (uint8_t)rx_mailbox->RDLR;
    rx_message->data[1] = (uint8_t)(rx_mailbox->RDLR >> 8);
    rx_message->data[2] = (uint8_t)(rx_mailbox->RDLR >> 16);
    rx_message->data[3] = (uint8_t)(rx_mailbox->RDLR >> 24);
    rx_message->data[4] = (uint8_t)rx_mailbox->RDHR;
    rx_message->data[5] = (uint8_t)(rx_mailbox->RDHR >> 8);
    rx_message->data[6] = (uint8_t)(rx_mailbox->RDHR >> 16);
    rx_message->data[7] = (uint8_t)(rx_mailbox->RDHR >> 24);

    // Release the FIFO mailbox
    if (fifo_number == 0) {
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    } else {
        CAN1->RF1R |= CAN_RF1R_RFOM1;
    }

    return 1; // Message received successfully
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
