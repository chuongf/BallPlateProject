/*
 ******************************************************************************
 * @file    platform_cfg.h 
 * @author  Chuong Nguyen
 * @brief   Define peripherals used on MCBSTM32 board
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H


/*********************** UART Interface  ***************************/
//  CONSOLE         USART1                 // USART1 for console SIO
//  FEEDBACK        USART2                 // USART2 for camera feedback SIO


/*********************** LEDs  *****************************/

	#define LED_PORT         GPIOB
  #define LED_10           GPIO_Pin_10            // PB.10
  #define LED_11           GPIO_Pin_11            // PB.11
  #define LED_12           GPIO_Pin_12            // PB.12
  #define LED_13           GPIO_Pin_13            // PB.13
  #define LED_14           GPIO_Pin_14            // PB.14
  #define LED_15           GPIO_Pin_15            // PB.15
	

/*********************** TIMER for servo-PWM generation ***********************/

//  #define TIM_USED             TIM4                   // TIMER4 used for PWMs
//  #define TIM_XYPERIOD         TIM_USED->ARR          // 
//  #define TIM_XDUTY     		   TIM_USED->CCR3         // PB.08
//  #define TIM_YDUTY     		   TIM_USED->CCR4         // PB.09

  #define TIM_USED             TIM3                   // TIMER4 used for PWMs
  #define TIM_XYPERIOD         TIM_USED->ARR          // 
  #define TIM_XDUTY     		   TIM_USED->CCR1         // PA.06
  #define TIM_YDUTY     		   TIM_USED->CCR2         // PA.07


#endif /* __PLATFORM_CONFIG_H */
