/*
 * poten.c
 *
 *  Created on: Dec 6, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "poten.h"

/* Define macros for the pin numbers connected to external components */
#define POTEN 3

// Initialization of GPIO Port B for the potentiometer
void GPIOB_Initialization(void)
{
	// Enable GPIO Port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// GPIO Mode to 'Input': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOB->MODER &= ~(0x0UL<<(2U*POTEN));
	// GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1)
	GPIOB->OTYPER |= 0UL<<(POTEN);
	// Configure GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR &= ~(0x3UL<<(2U*POTEN));
	GPIOB->PUPDR |= 0UL<<(2U*POTEN);
}
