/*----------------------------------------------------------------------------
 * Name:    STM32_Init.h
 * Purpose: STM32 peripherals initialisation definitions
 * Version: V1.00
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2005-2008 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Initialize system by steps
 1. Goto file STM32_Init.c for configure peripherals
 
 Peripheral 		Page 		search keyword
 Clock					0142		__CLOCK_SETUP
 Timer/PWM			2006		__TIMER_SETUP
 UART						2196		__USART_SETUP
 GPIO  					4448		__GPIO_SETUP
 Flash					4479		__EFI_SETUP
 
 3. Call stm32_Init() from main.c
 (look at line 5544 in STM32_Init.c for more information of stm32_Init())
 
 *----------------------------------------------------------------------------*/
 

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __STM32_INIT_H
#define __STM32_INIT_H

extern void         stm32_Init     (void);
extern unsigned int stm32_GetPCLK1 (void);
extern unsigned int stm32_GetPCLK2 (void);
#endif
