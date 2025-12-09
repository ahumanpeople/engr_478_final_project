/*
 * button.c
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "button.h"

/* Define macros for the pin numbers connected to external components */
#define SET_BUTTON 14
#define PED_BUTTON 15

void configure_EXTI_Buttons(void){

	// Configure the SYSCFG module to link EXTI line 2 and 3 to GPIO PC2 and PC3, respectively
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;										// Enable the clock to SYSCFG
	SYSCFG->EXTICR[4] &= ~(SYSCFG_EXTICR4_EXTI13 | SYSCFG_EXTICR4_EXTI14);     	// Clear the EXTI13 bits in SYSCFG's EXTICR1 register.
	SYSCFG->EXTICR[4] |= (SYSCFG_EXTICR4_EXTI13_PD | SYSCFG_EXTICR4_EXTI14_PD); 	// Set PC2 as the EXTI2 source in SYSCFG_EXTICR1.
	// Enable (unmask) the EXTI2 and EXTI3 interrupt by setting its corresponding bit in the EXTI's IMR.
	EXTI->IMR1 |= (1U << SET_BUTTON) | (1U << PED_BUTTON);	// Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)
	// Enable interrupt trigger for both rising (button release) and falling (button press) edges.
	EXTI->RTSR1 &= ~((1U << SET_BUTTON) | (1U << PED_BUTTON));	// Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	EXTI->FTSR1 |= ((1U << SET_BUTTON) | (1U << PED_BUTTON)); 	// Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
	// Cleared by writing a 1 to this bit
	EXTI->PR1 |= ((1U << SET_BUTTON) | (1U << PED_BUTTON));
	// Enable NVIC for EXTI15_10 interrupt using a function from CMSIS's core_cm4.h.
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}
/*
void EXTI13_IRQHandler(void)
{
   if (EXTI->PR13 & (1 << SET_BUTTON)) {
       EXTI->PR13 |= (1 << SET_BUTTON);   // clear pending
       // Example: you *could* toggle a debug LED here if needed.
       // if (GPIOC->IDR & (1 << SW1_PIN)) {
       //     toggle_LED(PA, PA2);
       // }
   }
}

void EXTI14_IRQHandler(void)
{
   if (EXTI->PR14 & (1 << PED_BUTTON)) {
       EXTI->PR14 |= (1 << PED_BUTTON);   // clear pending
       // Example: you *could* toggle a debug LED here if needed.
       // if (GPIOC->IDR & (1 << SW1_PIN)) {
       //     toggle_LED(PA, PA2);
       // }
   }
}

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
