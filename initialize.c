/*
 * initialize.c
 *
 *  Created on: Dec 17, 2025
 *      Author: josebarrios
 */

/* Include all necessary header files */
#include "stm32l476xx.h"
#include "initialize.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define POTEN		0	// Modified from PB3 to PB0 since PB0 is wired to ADC12_IN15
#define ADCWIRE		15	// PB0 is wired to ADC12_IN15, so a macro was created to facilitate a proper connection
#define SET_BUTTON	10	// PC13 presented conflict with on-board button
#define PED_BUTTON	12
#define BUZZER		2

enum SET_STATE {NO_MOD=1, MOD_TIMING, MOD_BUZZER};	// An enumeration to choose between settings
enum SET_STATE current_setting = NO_MOD; 	// Select the current state to be none; user uses device as a typical traffic light

// Initialization of GPIO Port A for LEDs
void LED_Initialization(void)
{
	// Enable GPIO Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(0x3UL << (2U*RED_LED));
	GPIOA->MODER |= (0x1UL << (2U*RED_LED));
	GPIOA->MODER &= ~(0x3UL << (2U*YELLOW_LED));
	GPIOA->MODER |= (0x1UL << (2U*YELLOW_LED));
	GPIOA->MODER &= ~(0x3UL << (2U*GREEN_LED));
	GPIOA->MODER |= (0x1UL << (2U*GREEN_LED));
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

// Initialization of GPIO Port C for push buttons, and their EXTI configuration
void Configure_GPIO_EXTI_Buttons(void)
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

// Initialization of GPIO Port D for the buzzer
void Buzzer_Initialization(void)
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
void toggle_LED(uint32_t LED_PIN)
{
	GPIOA->ODR ^= (1 << LED_PIN);
}


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

// Interrupt handler for settings and pedestrian button
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR1 & (1 << SET_BUTTON)) {
		EXTI->PR1 = (1 << SET_BUTTON);   // clear pending register
		// Example: you *could* toggle a debug LED here if needed.
		if ((GPIOC->IDR & (1 << SET_BUTTON)) == 0) {
			toggle_LED(RED_LED);
			// GPIOA->BRR |= 1 << RED_LED;
		}
	}
	else if (EXTI->PR1 & (1U << PED_BUTTON)) {
		EXTI->PR1 = (1U << PED_BUTTON);		// clear pending register
		// Software debounce (adds delay to ignore bounce)
		for (volatile uint32_t i = 0; i < 20000; ++i) { __NOP(); }
		if ((GPIOC->IDR & (1 << PED_BUTTON)) == 0) {
			toggle_LED(YELLOW_LED);
			// GPIOA->BRR |= 1 << YELLOW_LED;
		}
	}
}
