/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY2_Pin GPIO_PIN_13
#define KEY2_GPIO_Port GPIOC
#define KEY2_EXTI_IRQn EXTI15_10_IRQn
#define IIC1_SCL_Pin GPIO_PIN_0
#define IIC1_SCL_GPIO_Port GPIOC
#define IIC1_SDA_Pin GPIO_PIN_1
#define IIC1_SDA_GPIO_Port GPIOC
#define TFT_BLK_Pin GPIO_PIN_1
#define TFT_BLK_GPIO_Port GPIOA
#define SPI1_CS1_Pin GPIO_PIN_4
#define SPI1_CS1_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_6
#define TFT_DC_GPIO_Port GPIOA
#define TFT_RST_Pin GPIO_PIN_4
#define TFT_RST_GPIO_Port GPIOC
#define BEEPER_Pin GPIO_PIN_1
#define BEEPER_GPIO_Port GPIOB
#define RUNLED_Pin GPIO_PIN_2
#define RUNLED_GPIO_Port GPIOB
#define SPI2_CS1_Pin GPIO_PIN_8
#define SPI2_CS1_GPIO_Port GPIOA
#define RGB_Pin GPIO_PIN_15
#define RGB_GPIO_Port GPIOA
#define IIC2_SCL_Pin GPIO_PIN_4
#define IIC2_SCL_GPIO_Port GPIOB
#define IIC2_SDA_Pin GPIO_PIN_5
#define IIC2_SDA_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
