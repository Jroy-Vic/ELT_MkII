/*
 * DMA.h
 *
 *  Created on: Nov 18, 2024
 *      Author: vicer
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "main.h"

/* Macros */
#define DMA1_CH6_NVIC_PRIORITY 0x0
#define DMA1_CH6_NVIC (((uint32_t) DMA1_Channel1_IRQn) >> 5UL)

/* Global Variables */
extern char inputArr[ARR_SIZE];
extern uint8_t Array_Flag;

/* Initialize Peripheral-to-Memory Data Transfer */
void DMA_init(void);

/* Enable DMA Channels */
void DMA_enable(void);


#endif /* INC_DMA_H_ */
