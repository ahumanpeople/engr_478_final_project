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
#define SET_BUTTON	14	// PC13 presented conflict with on-board button
#define PED_BUTTON	15

// Initialization of GPIO Port C for push buttons
void configure_EXTI_Buttons(void)
{
	// Enable GPIO Port C and SYSCFG clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(3U<<(2U*SET_BUTTON));
	GPIOC->MODER &= ~(3U<<(2U*PED_BUTTON));
	// Configure GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOD->PUPDR &= ~(0x3UL<<(2U*SET_BUTTON));
	GPIOD->PUPDR &= ~(0x3UL<<(2U*PED_BUTTON));
	GPIOC->PUPDR |= 0UL<<(2U*SET_BUTTON);
	GPIOC->PUPDR |= 0UL<<(2U*PED_BUTTON);
	// Configure SYSCFG module to link EXTI14 to PC14
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Clear the relevant bits in SYSCFG's EXTICR4 register, and set EXTI sources for PC14 and PC15
	uint32_t temp_set_button = (SET_BUTTON%4U)*4U;
	uint32_t temp_ped_button = (PED_BUTTON%4U)*4U;
	SYSCFG->EXTICR[14/4] &= ~((0xFU<<temp_set_button) | (0xFU<<temp_ped_button));
	SYSCFG->EXTICR[14/4] |= ((0x2U<<temp_set_button) | (0x2U<<temp_ped_button)); // 0x2U = Port C
	// Enable interrupt trigger for both rising (button release) and falling (button press) edges.
	EXTI->RTSR1 &= ~((1U << SET_BUTTON) | (1U << PED_BUTTON));	// Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	EXTI->FTSR1 |= ((1U << SET_BUTTON) | (1U << PED_BUTTON)); 	// Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
	// Enable (unmask) the EXTI2 and EXTI3 interrupt by setting its corresponding bit in the EXTI's IMR.
	EXTI->IMR1 |= (1U << SET_BUTTON) | (1U << PED_BUTTON);	// Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)
	// Cleared by writing a 1 to this bit
	EXTI->PR1 = ((1U << SET_BUTTON) | (1U << PED_BUTTON));
	// Enabling & setting priority for NVIC EXTI15_10 interrupt
	NVIC_SetPriority(EXTI15_10_IRQn,2);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void configure_extra_button(void)
{
	// Enable GPIO Port C and SYSCFG clock
	// Configure SYSCFG module to link EXTI12 to PC13
	// RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(3U<<(13*2));
	GPIOD->PUPDR &= ~(0x3UL<<(2U*13));
	GPIOC->PUPDR |= 0UL<<(2U*13);
	// Linking EXTI13 to PC13
	uint32_t idx = (13U%4U)*4U;
	SYSCFG->EXTICR[13/4] &= ~(0xFU<<idx);
	SYSCFG->EXTICR[13/4] |= (0x2U<<idx); // 0x2 = Port C
	// Enable interrupt trigger for both falling and rising edges
	EXTI->RTSR1 &= ~(1U<<13);
	EXTI->FTSR1 |= (1U<<13);
	// Enable (unmask) the EXTI13 interript
	EXTI->IMR1 |= (1U<<13);
	// Cleared by writing a 1 to this bit
	EXTI->PR1 = (1U<<13);
	// Enabling & prioritizing EXTI15_10 interrupt
	// NVIC_SetPriority(EXTI15_10_IRQn,2);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Interrupt handler for pedestrian button
void EXTI15_10_IRQHandler(void)
{

	// Reads to check if EXTI13 is triggered
	if (EXTI->PR1 & (1U<<13))
	{
		EXTI->PR1 = (1U<<13);	// Clears pending register for EXTI13
		// Software debounce (adds delay to ignore bounce)
		// for (volatile uint32_t i = 0; i < 20000; ++i) { __NOP(); }
		// If the button is still pressed...
		if ((GPIOC->IDR & (1U<<13)) == 0)
		{
			toggle_LED(GREEN_LED);
			// GPIOA->BRR |= 1 << GREEN_LED;
		}
	}
	else if (EXTI->PR1 & EXTI_PR1_PIF15) {
		EXTI->PR1 = EXTI_PR1_PIF15;   // clear pending register
		// Example: you *could* toggle a debug LED here if needed.
		if ((GPIOC->IDR & (1 << PED_BUTTON)) == 0) {
			toggle_LED(YELLOW_LED);
			// GPIOA->BRR |= 1 << YELLOW_LED;
		}
	}
	else if (EXTI->PR1 & EXTI_PR1_PIF14) {
		EXTI->PR1 = EXTI_PR1_PIF14;   // clear pending register
		// Example: you *could* toggle a debug LED here if needed.
		if ((GPIOC->IDR & (1 << SET_BUTTON)) == 0) {
			toggle_LED(RED_LED);
			// GPIOA->BRR |= 1 << RED_LED;
		}
	}
}

/*
// Interrupt handler for settings button
void EXTI14_IRQHandler(void)
{
   if (EXTI->PR1 & EXTI_PR1_PIF14) {
       EXTI->PR1 = EXTI_PR1_PIF14;   // clear pending register
       // Example: you *could* toggle a debug LED here if needed.
       if ((GPIOC->IDR & (1 << SET_BUTTON)) == 0) {
    	   // toggle_LED(RED_LED);
    	   GPIOA->BRR |= 1 << RED_LED;
       }
   }
}

// Interrupt handler for pedestrian button
void EXTI15_IRQHandler(void)
{
   if (EXTI->PR1 & EXTI_PR1_PIF15) {
       EXTI->PR1 = EXTI_PR1_PIF15;   // clear pending register
       // Example: you *could* toggle a debug LED here if needed.
       if ((GPIOC->IDR & (1 << PED_BUTTON)) == 0) {
    	   // toggle_LED(YELLOW_LED);
    	   GPIOA->BRR |= 1 << YELLOW_LED;
       }
   }
}

void EXTI15_10_IRQHandler(void)
{
	// Reads to check if EXTI13 is triggered
	if (EXTI->PR1 & (1U<<13))
	{
		EXTI->PR1 = (1U<<13);	// Clears pending register for EXTI13
		// Software debounce (adds delay to ignore bounce)
		// for (volatile uint32_t i = 0; i < 20000; ++i) { __NOP(); }
		// If the button is still pressed...
		if ((GPIOC->IDR & (1U<<13)) == 0)
		{
			// toggle_LED(GREEN_LED);
	    	   GPIOA->BRR |= 1 << GREEN_LED;
		}
	}
}

*/


/*

// Initialization of GPIO Port C for the buttons
void GPIOC_Initialization(void)
{
	// Enable GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// Configure GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(0x3UL<<(2U*SET_BUTTON));
	GPIOC->MODER |= (0x1UL<<(2U*SET_BUTTON));
	GPIOC->MODER &= ~(0x3UL<<(2U*PED_BUTTON));
	GPIOC->MODER |= (0x1UL<<(2U*PED_BUTTON));
	// Configure GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	// GPIOD->PUPDR &= ~(0x3UL<<(2U*SET_BUTTON));
	// GPIOD->PUPDR &= ~(0x3UL<<(2U*PED_BUTTON));
	GPIOC->PUPDR |= 0UL<<(2*SET_BUTTON);
	GPIOC->PUPDR |= 0UL<<(2*PED_BUTTON);
}
*/
