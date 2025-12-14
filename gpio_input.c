/*
 * gpio_input.c
 *
 *  Created on: Dec 8, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "gpio_input.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED 9
#define YELLOW_LED 6
#define GREEN_LED 7

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

// Initialization of GPIO Port A for LEDs
void GPIOA_Initialization(void)
{
	// Enable GPIO Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(0x3UL<<(2U*RED_LED));
	GPIOA->MODER |= (0x1UL<<(2U*RED_LED));
	GPIOA->MODER &= ~(0x3UL<<(2U*YELLOW_LED));
	GPIOA->MODER |= (0x1UL<<(2U*YELLOW_LED));
	GPIOA->MODER &= ~(0x3UL<<(2U*GREEN_LED));
	GPIOA->MODER |= (0x1UL<<(2U*GREEN_LED));
	// GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1)
	GPIOA->OTYPER |= 0UL<<(RED_LED);
	GPIOA->OTYPER |= 0UL<<(YELLOW_LED);
	GPIOA->OTYPER |= 0UL<<(GREEN_LED);
	// Configure GPIO Pull-Up/Pull-Down to 'No Pull-up or Pull-down':
	// No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR |= 0UL<<(2U*RED_LED);
	GPIOA->PUPDR |= 0UL<<(2U*YELLOW_LED);
	GPIOA->PUPDR |= 0UL<<(2U*GREEN_LED);
}

// Modular function to turn on LED
void turn_on_LED(uint32_t LED_PIN)
{
	// GPIOA->ODR |= 1 << LED_PIN;
	GPIOA->BSRR |= 1 << LED_PIN;
}

// Modular function to turn off LED
void turn_off_LED(uint32_t LED_PIN)
{
	// GPIOA->ODR &= ~(1 << LED_PIN);
	GPIOA->BRR |= 1 << LED_PIN;
}

// Modular function to toggle LED
void toggle_LED(uint32_t LED_PIN) {GPIOA->ODR ^= (1 << LED_PIN);}

// Function to switch which LED should be on
void switch_LED(uint32_t NUM_IDEN)
{
	switch (NUM_IDEN)
	{
	case 1: // When '1' is selected, the red light comes on
		turn_on_LED(RED_LED);
		turn_off_LED(YELLOW_LED);
		turn_off_LED(GREEN_LED);
		break;
	case 2: // When '2' is selected, the green light comes on
		turn_off_LED(RED_LED);
		turn_off_LED(YELLOW_LED);
		turn_on_LED(GREEN_LED);
		break;
	case 3: // When '3' is selected, the yellow light comes on
		turn_off_LED(RED_LED);
		turn_on_LED(YELLOW_LED);
		turn_off_LED(GREEN_LED);
		break;
	default: // When neither of the expected options is selected, turn on all LEDs (easier to troubleshoot issue
		turn_on_LED(RED_LED);
		turn_on_LED(YELLOW_LED);
		turn_on_LED(GREEN_LED);
	}

}

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


// Initialization of GPIO Port D for the buzzer
void GPIOD_Initialization(void)
{
	// Enable GPIO Port D
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOD->MODER &= ~(0x3UL<<(2U*BUZZER));
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
