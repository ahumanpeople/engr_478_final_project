/*
 * systick.c
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
/*
#include "systick.h"
#include "LED.h"
#include "buzzer.h"
*/

/* Define macros for the pin numbers connected to external components */
#define RED_LED 9
#define YELLOW_LED 6
#define GREEN_LED 7
#define BUZZER 2
/* Define macros for the following constants used in this file */
#define TICKINT 1
// 1 = counting down to zero asserts the SysTick exception request
// 0 = counting down to zero does not assert the SysTick exception request
#define CLKSOURCE 1
// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
// If CLKSOURCE = 1, the processor clock is used.
#define RED_DURATION 1000
#define GREEN_DURATION 1000
#define YELLOW_DURATION 1000
// Assume value to be in milliseconds; amount of time each LED should be lit up for

static volatile uint32_t count_ms = 0; 			// ms count the amount of time that has past
static volatile uint32_t duration_limit_ms = 0;	// ms count for the next time the program needs to switch light

enum LED_STATE {RED=1, GREEN, YELLOW};	// An enumeration to easily select which LED is being focused on
enum LED_STATE current_state = RED; 	// Select the current state to be red; start off with the stop light on


void SysTick_Init(uint32_t Reload)
{
	// Disable SysTick Counter before all configurations are done
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	// Program reload value by configuring SysTick Reload Value Register.
	SysTick->LOAD = Reload - 1;	// Note: Time Interval = (Content of the register + 1) � Source_Clock_Period
	// Clear reload value by configuring SysTick Current Value Register.
	SysTick->VAL = 0;
	// Enable SysTick exception request
	SysTick->CTRL |= (TICKINT << 1U);
	// Select processor clock
	SysTick->CTRL |= (CLKSOURCE << 2U);
	// Enable SysTick Timer once all configurations are done
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void State_Init(enum LED_STATE i)
{
	switch_LED(i); // switch to the right LED to be lit up
	current_state = i; //
	uint32_t new_duration = (i == RED) ? RED_DURATION :
							(i == GREEN) ? GREEN_DURATION :
							YELLOW_DURATION;
	// using the identifier i to select the LED that should be lit up now,
	// add the time it should be lit up for to the limit,
	// extending the duration limit and leaving the LED on for the desired time
	duration_limit_ms += new_duration;
}

void SysTick_Handler(void)
{
	count_ms++; // adds time to count; should be in 1 ms intervals
	if ((count_ms - duration_limit_ms) >= 0)
	{
		switch (current_state)
		{
		case RED:
			State_Init(RED);
			break;
		case GREEN:
			State_Init(GREEN);
			break;
		case YELLOW:
			State_Init(YELLOW);
			break;
		default:
			switch_LED(5);
		}
	}
}
