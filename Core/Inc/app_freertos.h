/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.h
 * Description        : FreeRTOS applicative header file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#ifndef __APP_FREERTOS_H
#define __APP_FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
  int16_t Gyro_X;
  int16_t Gyro_Y;
  int16_t Gyro_Z;

  float Accel_X;
  float Accel_Y;
  float Accel_Z;
  
} IMUSample_t;

typedef struct {
  int16_t Gyro_X;
  int16_t Gyro_Y;
  int16_t Gyro_Z;

  float Accel_X;
  float Accel_Y;
  float Accel_Z;
} UIState_t;

typedef struct {
  int16_t Gyro_X;
  int16_t Gyro_Y;
  int16_t Gyro_Z;

  float Accel_X;
  float Accel_Y;
  float Accel_Z;
} UARTSample_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Exported macro -------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */
extern osThreadId_t Read_IMU_TaskHandle;
extern osThreadId_t Update_Dashboard_TaskHandle;
extern osThreadId_t Update_DisplayHandle;
extern osThreadId_t Update_UART_LogHandle;
extern osMutexId_t ScreenMutexHandle;
extern osMessageQueueId_t ImuSampleQHandle;
extern osMessageQueueId_t UiStateQHandle;
extern osMessageQueueId_t UartLogQHandle;
extern osSemaphoreId_t IMUDmaDoneSemHandle;

/* Exported function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartImuTask(void *argument);
void StartDashboardTask(void *argument);
void StartDisplayTask(void *argument);
void Update_UART_Log(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

#ifdef __cplusplus
}
#endif
#endif /* __APP_FREERTOS_H */
