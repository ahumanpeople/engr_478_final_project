/*
* 	File Name: main.c
*	Author: Oscar T. Madong, Jose Barrios
*/

// Include all necessary header files
#include "stm32l476xx.h"
#include "LED.h"
// #include "systick.h"
// #include "buzzer.h"
// #include "poten.h"
// #include "button.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define POTEN		3
#define SET_BUTTON	14	// PC13 presented conflict with on-board button
#define PED_BUTTON	15
#define BUZZER		2

#define BUZZER_FREQ	44

int main(void)
{
	GPIOA_Initialization();
	// GPIOB_Initialization();
	// GPIOC_Initialization();
	// GPIOD_Initialization();

	// SysTick_Init(4000000);

	turn_on_LED(YELLOW_LED);
	turn_on_LED(RED_LED);
	turn_on_LED(GREEN_LED);

	while (1) { /* while loop is true; infinite loop */ }
}
