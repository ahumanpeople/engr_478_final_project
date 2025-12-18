/*
 * systick.c
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#include "stm32l476xx.h"
#include "initialize.h"
#include "systick.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define POTEN		0	// Modified from PB3 to PB0 since PB0 is wired to ADC12_IN15
#define ADCWIRE		15	// PB0 is wired to ADC12_IN15, so a macro was created to facilitate a proper connection
#define SET_BUTTON	10	// PC13 presented conflict with on-board button
#define PED_BUTTON	12
#define BUZZER		2


/* Define macros for the following constants used in this file */
#define TICKINT 1
// 1 = counting down to zero asserts the SysTick exception request
// 0 = counting down to zero does not assert the SysTick exception request
#define CLKSOURCE 1
// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
// If CLKSOURCE = 1, the processor clock is used.

volatile uint32_t count_ms = 0; 			// ms count the amount of time that has past
const uint32_t total_duration = 6000000;	// total duration of the traffic light cycle
// ^^^ float values to proportionately set the timing for each of the LEDs
uint32_t red_duration = (total_duration / 20UL) * 9UL;
uint32_t yellow_duration = total_duration / 200UL;
uint32_t green_duration = (total_duration / 20UL) * 9UL;
// ^^^ Assume values to be in milliseconds; amount of time each LED should be lit up for
volatile uint32_t duration_limit_ms = total_duration / 20UL;	// ms count for the next time the program needs to switch light

enum LED_STATE {RED_STATE=1, GREEN_STATE, YELLOW_STATE};	// An enumeration to easily select which LED is being focused on
enum LED_STATE current_state = RED_STATE; 	// Select the current state to be red; start off with the stop light on


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

void State_Init(void)
{
	// switches to new state in the sequence
	uint32_t i = current_state;
	uint32_t new_duration;
	if (i == YELLOW_STATE)
	{
		i = RED_STATE;
		new_duration = red_duration;
	}
	else if (i == RED_STATE)
	{
		i = GREEN_STATE;
		new_duration = green_duration;
	}
	else
	{
		i = YELLOW_STATE;
		new_duration = yellow_duration;
	}

	switch_LED(i); // switch to the right LED to be lit up
	/*
	uint32_t new_duration = (i == RED_STATE) ? red_duration :
							(i == GREEN_STATE) ? green_duration :
							(i == YELLOW_STATE) ? yellow_duration :
							yellow_duration;

	if (i == YELLOW_STATE) { i = RED_STATE; }
	else if (i == RED_STATE) { i = GREEN_STATE; }
	else { i = YELLOW_STATE; }
	*/

	// using the identifier "i" to select the LED that should be lit up now,
	// add the time it should be lit up for to the limit,
	// extending the duration limit and leaving the LED on for the desired time
	duration_limit_ms += new_duration;
	current_state = i;
}

void SysTick_Handler(void)
{
	count_ms++; // adds time to count; should be in 1 ms intervals
	if ((count_ms - duration_limit_ms) >= 0)
	{
		State_Init();
	}
}
