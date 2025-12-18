/*
 * systick.h
 *
 *  Created on: Dec 7, 2025
 *      Author: josebarrios
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32l476xx.h"
#include "initialize.h"

extern const uint32_t total_duration;

void SysTick_Init(uint32_t Reload);

void State_Init(void);

void SysTick_Handler(void);

#endif /* SYSTICK_H_ */
