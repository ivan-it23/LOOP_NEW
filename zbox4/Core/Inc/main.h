/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define RX_BUFFER_SIZE 256
#define F_MIN 150000
#define F_MAX 5000000
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern char RxBuffer[2], RxBuffer2[2];
extern char TxBuffer[256], TxBuffer2[256];
extern char cmdBuf[RX_BUFFER_SIZE], cmdBuf2[RX_BUFFER_SIZE];;
extern volatile uint8_t bCmdFlag, bCmdFlag2;
//extern unsigned long iFreq;
extern volatile uint8_t n_bytes, n_bytes2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SendString(char *str);
void SendString2(char *str);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define C5_Pin GPIO_PIN_0
#define C5_GPIO_Port GPIOA
#define C10_Pin GPIO_PIN_1
#define C10_GPIO_Port GPIOA
#define VBAT_Pin GPIO_PIN_4
#define VBAT_GPIO_Port GPIOA
#define R1_Pin GPIO_PIN_0
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_1
#define R2_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOB
#define C3_Pin GPIO_PIN_10
#define C3_GPIO_Port GPIOB
#define C4_Pin GPIO_PIN_11
#define C4_GPIO_Port GPIOB
#define C6_Pin GPIO_PIN_12
#define C6_GPIO_Port GPIOB
#define C7_Pin GPIO_PIN_13
#define C7_GPIO_Port GPIOB
#define C8_Pin GPIO_PIN_14
#define C8_GPIO_Port GPIOB
#define C9_Pin GPIO_PIN_15
#define C9_GPIO_Port GPIOB
#define OFF_Pin GPIO_PIN_6
#define OFF_GPIO_Port GPIOF
#define TESTPIN_Pin GPIO_PIN_7
#define TESTPIN_GPIO_Port GPIOF
#define LED_LOW_Pin GPIO_PIN_15
#define LED_LOW_GPIO_Port GPIOA
#define R4_Pin GPIO_PIN_3
#define R4_GPIO_Port GPIOB
#define R5_Pin GPIO_PIN_4
#define R5_GPIO_Port GPIOB
#define R6_Pin GPIO_PIN_5
#define R6_GPIO_Port GPIOB
#define R7_Pin GPIO_PIN_6
#define R7_GPIO_Port GPIOB
#define R8_Pin GPIO_PIN_7
#define R8_GPIO_Port GPIOB
#define C1_Pin GPIO_PIN_8
#define C1_GPIO_Port GPIOB
#define C2_Pin GPIO_PIN_9
#define C2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
