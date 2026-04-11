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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
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

