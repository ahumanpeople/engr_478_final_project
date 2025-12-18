/*
 * button.c
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "button.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define SET_BUTTON	10	// PC13 presented conflict with on-board button
#define PED_BUTTON	12


// Initialization of GPIO Port C for push buttons
void configure_EXTI_Buttons(void)
{
	// Enable GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// Configure SYSCFG module to link EXTI12 to PC13
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(3U<<(2U*SET_BUTTON));
	GPIOC->MODER &= ~(3U<<(2U*PED_BUTTON));
	// Configure GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOD->PUPDR &= ~(0x3UL<<(2U*SET_BUTTON));
	GPIOD->PUPDR &= ~(0x3UL<<(2U*PED_BUTTON));
	GPIOC->PUPDR |= 0UL<<(2U*SET_BUTTON);
	GPIOC->PUPDR |= 0UL<<(2U*PED_BUTTON);
	// Clear the relevant bits in SYSCFG's EXTICR4 register, and set EXTI sources for PC14 and PC15 and onboard button
	uint32_t temp_set_button = (SET_BUTTON%4U)*4U;
	uint32_t temp_ped_button = (PED_BUTTON%4U)*4U;
	SYSCFG->EXTICR[3] &= ~(0xFU<<temp_set_button);
	SYSCFG->EXTICR[3] &= ~(0xFU<<temp_ped_button);
	SYSCFG->EXTICR[3] |= (0x2U<<temp_set_button); // 0x2U = Port C
	SYSCFG->EXTICR[3] |= (0x2U<<temp_ped_button);
	// Enable interrupt trigger for both falling and rising edges
	EXTI->RTSR1 &= ~(1U << SET_BUTTON);	// Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	EXTI->RTSR1 &= ~(1U << PED_BUTTON);	// Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
	EXTI->FTSR1 |= (1U << SET_BUTTON);
	EXTI->FTSR1 |= (1U << PED_BUTTON);
	// Enable (unmask) the EXTI13 interrupt
	EXTI->IMR1 |= (1U << SET_BUTTON);
	EXTI->IMR1 |= (1U << PED_BUTTON);
	// Cleared by writing a 1 to this bit
	EXTI->PR1 = (1U << SET_BUTTON);
	EXTI->PR1 = (1U << PED_BUTTON);
	// Enabling & prioritizing EXTI15_10 interrupt
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Interrupt handler for pedestrian button
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR1 & (1U << PED_BUTTON)) {
		EXTI->PR1 = (1U << PED_BUTTON);   // clear pending register
		// Software debounce (adds delay to ignore bounce)
		for (volatile uint32_t i = 0; i < 20000; ++i) { __NOP(); }
		if ((GPIOC->IDR & (1 << PED_BUTTON)) == 0) {
			toggle_LED(YELLOW_LED);
			// GPIOA->BRR |= 1 << YELLOW_LED;
		}
	}
	else if (EXTI->PR1 & (1 << SET_BUTTON)) {
		EXTI->PR1 = (1 << SET_BUTTON);   // clear pending register
		// Example: you *could* toggle a debug LED here if needed.
		if ((GPIOC->IDR & (1 << SET_BUTTON)) == 0) {
			toggle_LED(RED_LED);
			// GPIOA->BRR |= 1 << RED_LED;
		}
	}
}
