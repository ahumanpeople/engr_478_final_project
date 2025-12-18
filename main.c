/*
* 	File Name: main.c
*	Author: Oscar T. Madong, Jose Barrios
*/

/* Include all necessary header files */
#include "stm32l476xx.h"
#include "initialize.h"
#include "systick.h"
#include "ADC.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define POTEN		0	// Modified from PB3 to PB0 since PB0 is wired to ADC12_IN15
#define ADCWIRE		15	// PB0 is wired to ADC12_IN15, so a macro was created to facilitate a proper connection
#define SET_BUTTON	10	// PC13 presented conflict with on-board button
#define PED_BUTTON	12
#define BUZZER		2

volatile uint32_t adc_input = 0;
volatile uint32_t reload_value = 4000000;
// extern const uint32_t total_duration = 6000000;
extern const uint32_t total_duration;

int main(void)
{
	// Initialization of GPIO Port A for LEDs
	LED_Initialization();
	// Initialization of GPIO Port C for push buttons, and their EXTI configuration
	Configure_GPIO_EXTI_Buttons();
	// Initialization of GPIO Port D for the buzzer
	Buzzer_Initialization();

	SysTick_Init(reload_value);

	while (1) /* while loop set to 1 for infinite loop */
	{

	}
}
