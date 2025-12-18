/*
* 	File Name: main.c
*	Author: Oscar T. Madong, Jose Barrios
*/

/* Include all necessary header files */
#include "stm32l476xx.h"
#include "LED.h"
#include "ADC.h"
#include "button.h"
// #include "systick.h"
#include "buzzer.h"

/* Define macros for the pin numbers connected to external components */
#define RED_LED		9	// PA5 presents conflict with on-board LED
#define YELLOW_LED	6
#define GREEN_LED	7
#define POTEN		0	// Modified from PB3 to PB0 since PB0 is wired to ADC12_IN15
#define ADCWIRE		15	// PB0 is wired to ADC12_IN15, so a macro was created to facilitate a proper connection
#define SET_BUTTON	10	// PC13 presented conflict with on-board button
#define PED_BUTTON	12
#define BUZZER		2

#define BUZZER_FREQ	44
volatile uint32_t adc_input = 0;

int main(void)
{
	GPIOA_Initialization();
	configure_EXTI_Buttons();
	GPIOD_Initialization();

	// SysTick_Init(4000000);


	ADC_Init();
	ADC1->CR |= ADC_CR_ADSTART;
	volatile uint32_t last_raw_value = 0;
	volatile uint32_t raw_poten_value = 0; // max value is 4095.0 --> 0 to 4096

	while (1) {
		raw_poten_value = adc_result;
		// float adjusted_poten = ((raw_poten_data) / (4095.0f))*3.0f;
		// if (adjusted_poten >= 2.0f)
	    if (raw_poten_value != last_raw_value) {            // change detector
	        last_raw_value = raw_poten_value;
	        // temporary: gate LEDs off if below mid-scale to see movement
	        if (raw_poten_value >= 2048)
	        {
	            switch_LED(1);
	        }
	        else if (raw_poten_value < 2048)
	        {
	            switch_LED(3);
	            GPIOD->ODR ^= (1U << BUZZER);
	        }
	    }
	}
}
