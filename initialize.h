/*
 * initialize.h
 *
 *  Created on: Dec 17, 2025
 *      Author: josebarrios
 */

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

// Initialization of GPIO Port A for LEDs
void LED_Initialization(void);

// Initialization of GPIO Port C for push buttons, and their EXTI configuration
void Configure_GPIO_EXTI_Buttons(void);

// Initialization of GPIO Port D for the buzzer
void Buzzer_Initialization(void);

// Modular function to turn on LED
void turn_on_LED(uint32_t LED_PIN);

// Modular function to turn off LED
void turn_off_LED(uint32_t LED_PIN);

// Modular function to toggle LED
void toggle_LED(uint32_t LED_PIN);

// Function to switch which LED should be on
void switch_LED(uint32_t NUM_IDEN);

// Interrupt handler for pedestrian button
void EXTI15_10_IRQHandler(void);

#endif /* INITIALIZE_H_ */
