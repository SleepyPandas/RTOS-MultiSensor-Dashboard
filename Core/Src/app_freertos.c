/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7789V3.h"
#include "fonts/fonts.h"
#include "mpu6500.h"
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

  

  ST7789V3_Config config = {
      .spi_write = spi_write,
      .spi_write_dma = spi_write_dma, // Set to NULL to disable DMA
      .delay_ms = HAL_Delay,
      .set_cs = set_cs,
      .set_dc = set_dc,
      .set_rst = set_rst,
      .LCD_Width = 172,
      .LCD_Height = 320,
      .State = ST7789_STATE_READY,
  };

  MPU6500_Config mpu_config =
  {
    .write = I2C_Write,
    .read = I2C_Read,
    .delay_ms = HAL_Delay,
  };

/* USER CODE END Variables */
/* Definitions for Read_IMU_Task */
osThreadId_t Read_IMU_TaskHandle;
const osThreadAttr_t Read_IMU_Task_attributes = {
  .name = "Read_IMU_Task",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 384 * 4
};
/* Definitions for Update_Dashboard_Task */
osThreadId_t Update_Dashboard_TaskHandle;
const osThreadAttr_t Update_Dashboard_Task_attributes = {
  .name = "Update_Dashboard_Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 384 * 4
};
/* Definitions for Update_Display */
osThreadId_t Update_DisplayHandle;
const osThreadAttr_t Update_Display_attributes = {
  .name = "Update_Display",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 512 * 4
};
/* Definitions for ImuSampleQ */
osMessageQueueId_t ImuSampleQHandle;
const osMessageQueueAttr_t ImuSampleQ_attributes = {
  .name = "ImuSampleQ"
};
/* Definitions for UiStateQ */
osMessageQueueId_t UiStateQHandle;
const osMessageQueueAttr_t UiStateQ_attributes = {
  .name = "UiStateQ"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */
  /* creation of ImuSampleQ */
  ImuSampleQHandle = osMessageQueueNew (8, sizeof(IMUSample_t), &ImuSampleQ_attributes);
  /* creation of UiStateQ */
  UiStateQHandle = osMessageQueueNew (2, sizeof(UIState_t), &UiStateQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of Read_IMU_Task */
  Read_IMU_TaskHandle = osThreadNew(StartImuTask, NULL, &Read_IMU_Task_attributes);

  /* creation of Update_Dashboard_Task */
  Update_Dashboard_TaskHandle = osThreadNew(StartDashboardTask, NULL, &Update_Dashboard_Task_attributes);

  /* creation of Update_Display */
  Update_DisplayHandle = osThreadNew(StartDisplayTask, NULL, &Update_Display_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartImuTask */
/**
* @brief Function implementing the Read_IMU_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartImuTask */
void StartImuTask(void *argument)
{
  /* USER CODE BEGIN Read_IMU_Task */
  IMUSample_t sample;
  MPU6500_Gyro_Data gyro;
  MPU6500_Accel_Data accel;
  MPU6500_Init(&mpu_config);
  uint32_t next;

  /* Infinite loop */
   for (;;)
  {
    if (MPU6500_Read_Gyro_Data(&mpu_config, &gyro) == 0 &&
        MPU6500_Read_Accel_Data(&mpu_config, &accel) == 0)
    {
      sample.Gyro_X = gyro.Gyro_X;
      sample.Gyro_Y = gyro.Gyro_Y;
      sample.Gyro_Z = gyro.Gyro_Z;
      sample.Accel_X = accel.Accel_X;
      sample.Accel_Y = accel.Accel_Y;
      sample.Accel_Z = accel.Accel_Z;

      osMessageQueuePut(ImuSampleQHandle, &sample, 0U, 0U);
    }

    next += 10;           // 10 ms = 100 Hz
    osDelayUntil(next);   // fixed period
  }
  /* USER CODE END Read_IMU_Task */
}

/* USER CODE BEGIN Header_StartDashboardTask */
/**
* @brief Function implementing the Update_Dashboard_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDashboardTask */
void StartDashboardTask(void *argument)
{
  /* USER CODE BEGIN Update_Dashboard_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Update_Dashboard_Task */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
* @brief Function implementing the Update_Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN Update_Display */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Update_Display */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

