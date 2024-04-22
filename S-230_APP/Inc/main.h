/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "app.h"
#include "board_config.h"
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
#define LINK_Pin GPIO_PIN_6
#define LINK_GPIO_Port GPIOA
#define LINK_EXTI_IRQn EXTI9_5_IRQn
#define LINK1_Pin GPIO_PIN_6
#define LINK1_GPIO_Port GPIOB
#define RS485_WRITE_Pin GPIO_PIN_12
#define RS485_WRITE_GPIO_Port GPIOB
#define PUMP_CHECK_Pin GPIO_PIN_15
#define PUMP_CHECK_GPIO_Port GPIOD
#define IIC_SDA_Pin GPIO_PIN_11
#define IIC_SDA_GPIO_Port GPIOA
#define IIC_SCL_Pin GPIO_PIN_12
#define IIC_SCL_GPIO_Port GPIOA
#define WDI_Pin GPIO_PIN_2
#define WDI_GPIO_Port GPIOE
#define PUMP_PWRKEY_Pin GPIO_PIN_7
#define PUMP_PWRKEY_GPIO_Port GPIOB
// 临时修改IO 
#define GPRS_4G_EN_Pin  GPIO_PIN_15
#define GPRS_4G_EN_GPIO_Port GPIOA

#define SENSOR_PWRKEY_Pin GPIO_PIN_8
#define SENSOR_PWRKEY_GPIO_Port GPIOC
#define MCU_STATE_Pin GPIO_PIN_4
#define MCU_STATE_GPIO_Port GPIOC

#define CAN1_RX_Pin GPIO_PIN_8
#define CAN1_RX_GPIO_Port GPIOB
#define CAN1_TX_Pin GPIO_PIN_9
#define CAN1_TX_GPIO_Port GPIOB

#define LCD_CS_Pin GPIO_PIN_14
#define LCD_CS_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_8
#define LCD_BL_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_9
#define LCD_RST_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_10
#define LCD_DC_GPIO_Port GPIOD


#ifdef SUPPORT_IRED
#define IRED_CHECK_Pin  GPIO_PIN_0
#define IRED_CHECK_GPIO_Port  GPIOA 
#endif

/* USER CODE BEGIN Private defines */
#define SIZE_128B					(0x80)
#define SIZE_256B					(0x100)
#define SIZE_512B					(0x200)
#define SIZE_1K						(0x400)
#define SIZE_2K						(0x800)
#define SIZE_4K						(0x1000)

#define DATA_MAX_SIZE				(SIZE_256B)

#define OTHER_DATA_MAX_SIZE			(0x40)
#define USED_DATA_MAX_SIZE			(DATA_MAX_SIZE + 0x20)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
