/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define BUTTON_USER_Pin GPIO_PIN_13
#define BUTTON_USER_GPIO_Port GPIOC
#define DISPLAY_TEARING_EFFECT_Pin GPIO_PIN_0
#define DISPLAY_TEARING_EFFECT_GPIO_Port GPIOA
#define DISPLAY_TEARING_EFFECT_EXTI_IRQn EXTI0_1_IRQn
#define DISPLAY_RESET_Pin GPIO_PIN_1
#define DISPLAY_RESET_GPIO_Port GPIOA
#define MCU_ACTIVE_Pin GPIO_PIN_4
#define MCU_ACTIVE_GPIO_Port GPIOC
#define FRAME_RATE_Pin GPIO_PIN_5
#define FRAME_RATE_GPIO_Port GPIOC
#define BUTTON_LEFT_Pin GPIO_PIN_0
#define BUTTON_LEFT_GPIO_Port GPIOB
#define BUTTON_RIGHT_Pin GPIO_PIN_1
#define BUTTON_RIGHT_GPIO_Port GPIOB
#define BUTTON_UP_Pin GPIO_PIN_12
#define BUTTON_UP_GPIO_Port GPIOB
#define DISPLAY_DCX_Pin GPIO_PIN_14
#define DISPLAY_DCX_GPIO_Port GPIOB
#define FLASH_CS_Pin GPIO_PIN_8
#define FLASH_CS_GPIO_Port GPIOA
#define DISPLAY_CSX_Pin GPIO_PIN_9
#define DISPLAY_CSX_GPIO_Port GPIOA
#define BUTTON_CENTER_Pin GPIO_PIN_7
#define BUTTON_CENTER_GPIO_Port GPIOC
#define RENDER_TIME_Pin GPIO_PIN_8
#define RENDER_TIME_GPIO_Port GPIOD
#define VSYNC_FREQ_Pin GPIO_PIN_9
#define VSYNC_FREQ_GPIO_Port GPIOD
#define BUTTON_DOWN_Pin GPIO_PIN_4
#define BUTTON_DOWN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
