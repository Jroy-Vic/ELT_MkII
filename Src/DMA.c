/*
 * DMA.c
 *
 *  Created on: Nov 22, 2024
 *      Author: vicer
 */

#include "main.h"
#include "UART.h"
#include "DMA.h"


/* Initialize DMA Peripheral-to-Memory Data Transfer */
/* USART2 to Memory Transfer
 * USART2_RX: Channel 6, DMA1
 * -------------------------
 * Memory to Memory Transfer
 */
void DMA_init() {
	/* Initialize Clock for DMA1 */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	/* Disable DMA1 Channel 6 Configure */
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;

	/* Set Peripheral Register Address (USART2_RX) in DMA1_CPAR6 */
	DMA1_Channel6->CPAR = (uint32_t) &(USART2->RDR);

	/* Set Memory Address (inBuff) in DMA1_CMAR1 */
	DMA1_Channel6->CMAR = (uint32_t) inputArr;

	/* Configure the Number of Data to Transfer in DMA_CNDTR6 */
	DMA1_Channel6->CNDTR = ARR_SIZE;

	/* Configure Channel 6 for USART2_Rx (C6S = 4'b0010) */
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C6S;
	DMA1_CSELR->CSELR |= (0x2 << DMA_CSELR_C6S_Pos);

	/* Configure DMA1_CCR6
	 * Data Transfer Direction: MEM2MEM - Disabled (0), DIR - Read from Peripheral (0)
	 * Priority Level: PL - Very High (11)
	 * Circular Mode: CIRC - Enabled (1)
	 * Peripheral and Memory Incremented Mode: MINC - Enabled (1), PINC - Disabled (0)
	 * Peripheral and Memory Data Size: MSIZE - 8 Bits (00), PSIZE - 8 Bits (00)
	 * Interrupt Enable: TCIE - Enabled (1)
	 * */
	DMA1_Channel6->CCR &= ~(DMA_CCR_MEM2MEM | DMA_CCR_DIR);
	DMA1_Channel6->CCR |= DMA_CCR_PL;
	DMA1_Channel6->CCR |= DMA_CCR_CIRC;
	DMA1_Channel6->CCR |= DMA_CCR_MINC;
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
	DMA1_Channel6->CCR &= ~(DMA_CCR_MSIZE | DMA_CCR_PSIZE);
	DMA1_Channel6->CCR |= DMA_CCR_TCIE;

	/* Enable NVIC for Interrupts */
	NVIC_SetPriority(DMA1_Channel6_IRQn, DMA1_CH6_NVIC_PRIORITY);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	__enable_irq();

	/* Clear Any Pending Interrupts Before Enabling DMA Channel */
	DMA1->IFCR |= DMA_IFCR_CGIF6;
	DMA1->IFCR |= DMA_IFCR_CHTIF6;
	DMA1->IFCR |= DMA_IFCR_CTCIF6;
}


/* Enable DMA Channels */
void DMA_enable() {
	/* Activate Channel 6 */
	DMA1_Channel6->CCR |= DMA_CCR_EN;
}


/* Handle DMA Interrupts */
void DMA1_Channel6_IRQHandler() {
	/* Set Array Flag when Transfer is Complete */
	if (DMA1->ISR & DMA_ISR_TCIF6) {
		/* Clear Interrupt Flag */
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
	}
}

