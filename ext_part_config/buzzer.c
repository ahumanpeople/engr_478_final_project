/*
 * buzzer.c
 *
 *  Created on: Dec 6, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "buzzer.h"

/* Define macros for the pin numbers connected to external components */
#define BUZZER 2

// Initialization of GPIO Port D for the buzzer
void GPIOD_Initialization(void)
{
	// Enable GPIO Port D
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOD->MODER &= ~(0x3UL<<(2U*BUZZER));
	GPIOD->MODER |= (0x1UL << (2U*BUZZER));
	// GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1)
	GPIOD->OTYPER |= 0UL<<(BUZZER);
	// GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOD->PUPDR &= ~(0x3UL<<(2U*BUZZER));
	GPIOD->PUPDR |= 0UL<<(2*BUZZER);
	// GPIO Speed Register to 'High Speed': Low Speed (00), Medium Speed (01), High Speed (10), Very High Speed (11)
	GPIOD->OSPEEDR |= (3UL<<(2*BUZZER));
	// GPIO Set/Reset Register: Bit Set to '1'; remove interrupt interference with GPIO outputs
	GPIOD->BSRR = 1UL<<(BUZZER);
}
