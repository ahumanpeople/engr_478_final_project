/*
 * button.h
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32l476xx.h"
#include "LED.h"

// Initialization of GPIO Port C for push buttons
void configure_EXTI_Buttons(void);

void EXTI15_10_IRQHandler(void);

#endif /* BUTTON_H_ */
