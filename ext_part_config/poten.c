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
// #include "buzzer.h"
// #include "poten.h"

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
	// GPIOB_Initialization();
	configure_EXTI_Buttons();
	// EXTI15_10_IRQHandler();
	// GPIOD_Initialization();

	// SysTick_Init(4000000);


	ADC_Init();
	ADC1->CR |= ADC_CR_ADSTART;
	volatile uint32_t last_raw = 0;

	while (1) {
	    uint32_t raw = adc_result;
	    if (raw != last_raw) {            // change detector
	        last_raw = raw;
	        // temporary: gate LEDs off if below mid-scale to see movement
	        if (raw < 2750) {
	            turn_on_LED(RED_LED);
	            turn_off_LED(YELLOW_LED);
	            turn_off_LED(GREEN_LED);
	        } else {
	            turn_on_LED(YELLOW_LED);
	            turn_off_LED(RED_LED);
	            turn_off_LED(GREEN_LED);
	        }
	        // set a breakpoint here; inspect 'raw'
	    }
	}

	/*
	while(1) // while loop is true; infinite loop
	{
		float raw_poten_data = adc_result;
		// float adjusted_poten = ((raw_poten_data) / (4095.0f))*3.0f;
		// if (adjusted_poten >= 2.0f)
		if (raw_poten_data >= 2730)
		{
			turn_on_LED(YELLOW_LED);
		}
		// else if (adjusted_poten <= 1.0f)
		else if (raw_poten_data <= 1370)
		{
			turn_on_LED(RED_LED);
		}
		else
		{
			turn_on_LED(GREEN_LED);
		}
	}
	*/


	/*
	volatile uint32_t last_raw = 0;

	while (1) {
	    uint32_t raw = adc_result;
	    if (raw != last_raw) {            // change detector
	        last_raw = raw;
	        // temporary: gate LEDs off if below mid-scale to see movement
	        if (raw < 620) {
	            turn_on_LED(RED_LED);
	            turn_off_LED(YELLOW_LED);
	            turn_off_LED(GREEN_LED);
	        } else {
	            turn_on_LED(YELLOW_LED);
	            turn_off_LED(RED_LED);
	            turn_off_LED(GREEN_LED);
	        }
	        // set a breakpoint here; inspect 'raw'
	    }
	}
	*/


	/*
	ADC1->IER &= ~ADC_IER_EOC;            // disable EOC IRQ temporarily
	ADC1->CR  |= ADC_CR_ADSTART;

	while (1) {
	    if (ADC1->ISR & ADC_ISR_EOC) {    // poll EOC
	        adc_result = ADC1->DR;        // read the sample
	    }
	    turn_on_LED(GREEN_LED);
	}
	*/
}
