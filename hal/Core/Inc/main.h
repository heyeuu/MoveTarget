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
#define receive_tof_Pin GPIO_PIN_7
#define receive_tof_GPIO_Port GPIOB
#define nearby_high_Pin GPIO_PIN_0
#define nearby_high_GPIO_Port GPIOF
#define nearby_read_Pin GPIO_PIN_1
#define nearby_read_GPIO_Port GPIOF
#define LEDA_Pin GPIO_PIN_12
#define LEDA_GPIO_Port GPIOH
#define LEDB_Pin GPIO_PIN_11
#define LEDB_GPIO_Port GPIOH
#define remote_low_Pin GPIO_PIN_12
#define remote_low_GPIO_Port GPIOB
#define remote_read_Pin GPIO_PIN_13
#define remote_read_GPIO_Port GPIOB
#define remote_high_Pin GPIO_PIN_14
#define remote_high_GPIO_Port GPIOB
#define nearby_low_Pin GPIO_PIN_15
#define nearby_low_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
