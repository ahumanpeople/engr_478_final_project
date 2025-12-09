/*
 * LED.h
 *
 *  Created on: Dec 6, 2025
 *      Author: josebarrios
 */

#ifndef LED_H_
#define LED_H_

#include "stm32l476xx.h"

// Initialization of GPIO Port A for LEDs
void GPIOA_Initialization(void);

// Modular function to turn on LED
void turn_on_LED(uint32_t LED_PIN);

// Modular function to turn off LED
void turn_off_LED(uint32_t LED_PIN);

// Modular function to toggle LED
void toggle_LED(uint32_t LED_PIN);

#endif /* LED_H_ */
