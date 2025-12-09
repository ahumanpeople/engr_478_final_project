/*
 * button.h
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32l476xx.h"

void configure_EXTI_Buttons(void);

void EXTI13_IRQHandler(void);

void EXTI14_IRQHandler(void);

// Initialization of GPIO Port C for the buttons
void GPIOC_Initialization(void);

#endif /* BUTTON_H_ */
