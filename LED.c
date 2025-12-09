/*
 * LED.c
 *
 *  Created on: Dec 6, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "LED.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED 9
#define YELLOW_LED 6
#define GREEN_LED 7

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
