/*
 * Laser.c
 *
 *  Created on: Nov 22, 2024
 *      Author: vicer
 */

#include "main.h"
#include "Laser.h"
#include "TIM.h"
#include "Servo.h"


/* Initialize Laser Peripheral */
void LASER_init() {
	/* Using PC3 as Laser Output *
	 * AHB2ENR: GPIOC Clock Enable
	 * MODER = Output (01)
	 * OTYPER = PP (0)
	 * OSPEEDR = Low Speed (00)
	 * PUPD = PD (10)
	 */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODE3);
	GPIOC->MODER |= (GPIO_MODER_MODE3_0);
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT3);
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED3);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD3);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD3);
}


/* Activate Laser */
void LASER_activate() {
	/* Turn Laser on when Servo is Active */
	if ((TIM2->CCR1 > DEF_SERVO_CCRX) || (TIM2->CCR2 > DEF_SERVO_CCRY)) {
		GPIOC->ODR |= GPIO_ODR_OD3;
	} else {
		GPIOC->ODR &= ~GPIO_ODR_OD3;
	}
}
