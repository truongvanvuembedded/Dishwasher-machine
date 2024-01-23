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
#include "stm32f1xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define DOOR_Pin GPIO_PIN_14
#define DOOR_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOC
#define POWER_Pin GPIO_PIN_0
#define POWER_GPIO_Port GPIOA
#define CONFIRM_Pin GPIO_PIN_1
#define CONFIRM_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_3
#define RESET_GPIO_Port GPIOA
#define RESET_EXTI_IRQn EXTI3_IRQn
#define MODE_Pin GPIO_PIN_4
#define MODE_GPIO_Port GPIOA
#define MODE_EXTI_IRQn EXTI4_IRQn
#define INFOR_Pin GPIO_PIN_5
#define INFOR_GPIO_Port GPIOA
#define MECHICAL_WASH_Pin GPIO_PIN_6
#define MECHICAL_WASH_GPIO_Port GPIOA
#define MECHICAL_RINSE_Pin GPIO_PIN_7
#define MECHICAL_RINSE_GPIO_Port GPIOA
#define WATER_LEVEL_BOILER_Pin GPIO_PIN_0
#define WATER_LEVEL_BOILER_GPIO_Port GPIOB
#define WATER_LEVEL_BOILER_EXTI_IRQn EXTI0_IRQn
#define WATER_LEVEL_TANK_Pin GPIO_PIN_1
#define WATER_LEVEL_TANK_GPIO_Port GPIOB
#define DS18B20_BOILER_Pin GPIO_PIN_10
#define DS18B20_BOILER_GPIO_Port GPIOB
#define DS18B20_TANK_Pin GPIO_PIN_11
#define DS18B20_TANK_GPIO_Port GPIOB
#define PREVENTIVE_Pin GPIO_PIN_12
#define PREVENTIVE_GPIO_Port GPIOB
#define VALVE_INLET_Pin GPIO_PIN_13
#define VALVE_INLET_GPIO_Port GPIOB
#define PUMP_WASH_Pin GPIO_PIN_15
#define PUMP_WASH_GPIO_Port GPIOB
#define BOILER_HEATER_Pin GPIO_PIN_5
#define BOILER_HEATER_GPIO_Port GPIOB
#define PUMP_RINSE_Pin GPIO_PIN_8
#define PUMP_RINSE_GPIO_Port GPIOB
#define TANK_HEATER_Pin GPIO_PIN_9
#define TANK_HEATER_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define  RESET  	GPIO_PIN_RESET		
#define  SET 			GPIO_PIN_SET		
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
