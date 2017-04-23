/*
 ******************************************************************************
 * @file    platform_cfg.h 
 * @author  MCD Application Team & Mys
 * @version V3.4.0
 * @date    18/08/2016
 * @brief   Application & target specific peripherals configuration file for
 *          BallPlate Control / Kalman Filter demo application & lab projects.
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 *
 * Modified:
 * 23.05.2011  Mooshammer/Maier for mp3-player project on MCBSTM32C board
 * 10.07.2011  Mys  adapted for RTSys-LabEx#2 sample code
 * 19.08.2016  Mys  adapted for BallPlate / Kalman Filter demo application
 * 07.12.2016  Mys  separate CONSOLE_UART vs AUX_UART #defines
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run application code
*/

#if !defined (USE_MCBSTM32C) && !defined (USE_MCBSTM32)
  #define USE_MCBSTM32
//#define USE_MCBSTM32C 
#endif

/******************************************************************
#ifdef USE_MCBSTM32C
  #define MESSAGE1   "STM32 Connectivity "
  #define MESSAGE2   "Line Device 107VC "
  #define MESSAGE3   "running on MCBSTM32C\r\n"
#elif defined (USE_MCBSTM32)
  #define MESSAGE1   "STM32 Medium Density "
  #define MESSAGE2   "Device 103RB "
  #define MESSAGE3   "running on MCBSTM32\r\n"
#endif
********************************************************************/


/* Define STM32Fxxx peripherals depending on evaluation board used */

#if defined USE_MCBSTM32
  #define TARGET_BOARD         "MCBSTM32"             //!< target board string
  #define TARGET_MCU           "STM32F103RB"          //!< target MCU string

/*********************** UART Interface  ***************************/

//  #define CONSOLE_USART1                 // USART1 for console SIO
//  #define FEEDBACK_USART2                 // USART2 for sensor feedback SIO

//  #define CONSOLE_USART         USART1                 // USART1 for console SIO
//  #define FEEDBACK_USART        USART2                 // USART2 for camera feedback SIO
/*********************** LEDs  *****************************/

	#define LED_PORT             GPIOB
  #define LED_10           GPIO_Pin_10            // PB.10
  #define LED_11           GPIO_Pin_11            // PB.11
  #define LED_12           GPIO_Pin_12            // PB.12
  #define LED_13           GPIO_Pin_13            // PB.13
  #define LED_14           GPIO_Pin_14            // PB.14
  #define LED_15           GPIO_Pin_15            // PB.15
	

/*********************** TIMER for servo-PWM generation ***********************/
//   #define TIM_GPIO             TIM3                   // TIMER3 used for PWMs
//   #define TIM_XAXIS_RSTVAL     TIM_GPIO->CCR1         // PA.06
//   #define TIM_YAXIS_RSTVAL     TIM_GPIO->CCR2         // PA.06

//  #define TIM_USED             TIM4                   // TIMER4 used for PWMs
//  #define TIM_XYPERIOD         TIM_USED->ARR          // 
//  #define TIM_XDUTY     		   TIM_USED->CCR3         // PB.08
//  #define TIM_YDUTY     		   TIM_USED->CCR4         // PB.09

  #define TIM_USED             TIM3                   // TIMER4 used for PWMs
  #define TIM_XYPERIOD         TIM_USED->ARR          // 
  #define TIM_XDUTY     		   TIM_USED->CCR1         // PA.06
  #define TIM_YDUTY     		   TIM_USED->CCR2         // PA.07

/* #endif USE_MCBSTM32 */



#elif defined USE_MCBSTM32C
  #define TARGET_BOARD         "MCBSTM32C"            //!< target board string
  #define TARGET_MCU           "STM32F107VC"          //!< target MCU string

/*********************** UART (Terminal) Interface  ***************************/

//#define USARTx               USART2                 // USART2 for console SIO
  #define CONSOLE_USART        USART2                 // USART2 for console SIO
  #define USARTx_GPIO          GPIOD                  // GPIOD
  #define USARTx_CLK           RCC_APB1Periph_USART2
  #define USARTx_GPIO_CLK      RCC_APB2Periph_GPIOD
  
  #define USARTx_CTSPin        GPIO_Pin_3             // PD.03
  #define USARTx_RTSPin        GPIO_Pin_4             // PD.04
  #define USARTx_TxPin         GPIO_Pin_5             // PD.05
  #define USARTx_RxPin         GPIO_Pin_6             // PD.06
  #define USARTx_CKPin         GPIO_Pin_7             // PD.07
  #define USARTx_BDRate        115200   

  #define AUX_USART            USART3                 // USART3 for auxiliary SIO

/*********************** LEDs / Stepmotor Control *****************************/
  #define GPIO_LED             GPIOE                  // GPIOE used for LEDs
  #define GPIO_LED_CLK         RCC_APB2Periph_GPIOE   
  
  #define GPIO_LED_1           GPIO_Pin_8             // PE.08
  #define GPIO_LED_2           GPIO_Pin_9             // PE.09
  #define GPIO_LED_3           GPIO_Pin_10            // PE.10
  #define GPIO_LED_4           GPIO_Pin_11            // PE.11
  #define GPIO_LED_5           GPIO_Pin_12            // PE.12
  #define GPIO_LED_6           GPIO_Pin_13            // PE.13
  #define GPIO_LED_7           GPIO_Pin_14            // PE.14
  #define GPIO_LED_8           GPIO_Pin_15            // PE.15

/*********************** TIMER for servo-PWM generation ***********************/
  #define TIM_USED             TIM4                   // TIMER4 used for PWMs
  #define TIM_XYPERIOD         TIM_USED->ARR         // 
  #define TIM_XDUTY     		   TIM_USED->CCR3         // PB.08
  #define TIM_YDUTY     		   TIM_USED->CCR4         // PB.09

	
/* #endif USE_MCBSTM32C */

#else
  #error: "Illegal target board selection!"
#endif

/******************************************************************************/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
