/*
 * UART.c
 *
 *  Created on: Nov 22, 2024
 *      Author: vicer
 */

#include "main.h"
#include "UART.h"
#include <stdlib.h>

/* Initialize and Configure USART2 */
void UART_init() {
	/* USART2 is being used since it is already
	 *  connected to the USB and Hardware Debugger */
	/* USART2 is AF7 for PA2-PA4:
	 * PA2 = USART2_TX
	 * PA3 = USART2_RX
	 * PA4 = USART2_CK */

	/* GPIO Configuration */
	/* Set Clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	/* MODE = Alt. Func (10) */
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3 |
					GPIO_MODER_MODE4);
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1 |
					GPIO_MODER_MODE4_1);
	/* AFL = AF7 (0111) */
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3 |
					GPIO_AFRL_AFSEL4);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_3 | GPIO_AFRL_AFSEL3_3 |
					GPIO_AFRL_AFSEL4_3);
	/* OTYPER = PP (0) */
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3 |
					GPIO_OTYPER_OT4);
	/* OSPEEDR = Very High Speed (11) */
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3 |
					GPIO_OSPEEDR_OSPEED4);
	/* PUPDR = No PUPD (00) */
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3 |
					GPIO_PUPDR_PUPD4);

	/* USART2 Configuration */
	/* Disable USART2 to configure (Clear UE Bit) */
	USART2->CR1 &= ~USART_CR1_UE;
	/* Set Word Length to 8 Bits (M0, M1 = 00) */
	USART2->CR1 &= ~USART_CR1_M1;
	USART2->CR1 &= ~USART_CR1_M0;
	/* Set Oversampling by 8 to account for Baud Rate
	 * Using 8 to save power */
	USART2->CR1 |= USART_CR1_OVER8;
	/* Configure STOP Bits (1 Stop Bit) */
	USART2->CR2 &= ~USART_CR2_STOP;
	/* Enabled USART2 RX DMA Request */
	USART2->CR3 |= USART_CR3_DMAR;

//	/* Enable Interrupts */
//	/* Enable Rx Not Empty Flag */
//	USART2->CR1 |= USART_CR1_RXNEIE;
//	/* Enable NVIC */
//	NVIC->IP[USART2_IRQn] = UART2_PRIORITY;
//	NVIC->ISER[UART2_NVIC] = (1<< (USART2_IRQn & 0x1F));

	/* Set Baud Rate */
	/* When OVER8 = 1: BRR[15:4] = USARTDIV[15:4]
	 * 				   BRR[3] must be kept cleared
	 * 				   BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right
	 */
	USART2->BRR = ((BAUD_RATE & 0xFFF0) | (BAUD_RATE & ~0x000F) |
					((BAUD_RATE & 0x000F) >> 1));

	/* Enable USART2 */
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_UE);
}
