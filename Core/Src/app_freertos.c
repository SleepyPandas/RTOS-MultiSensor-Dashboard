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
#include "main.h"
#include "mpu6500.h"
#include "stm32h503xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UI_UPDATE_PERIOD_MS 50U
#define UART_LOG_PERIOD_MS 50U

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
  .stack_size = 512 * 4
};
/* Definitions for Update_Display */
osThreadId_t Update_DisplayHandle;
const osThreadAttr_t Update_Display_attributes = {
  .name = "Update_Display",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 1024 * 4
};
/* Definitions for Update_UART_Log */
osThreadId_t Update_UART_LogHandle;
const osThreadAttr_t Update_UART_Log_attributes = {
  .name = "Update_UART_Log",
  .priority = (osPriority_t) osPriorityBelowNormal1,
  .stack_size = 256 * 4
};
/* Definitions for Blink_LED */
osThreadId_t Blink_LEDHandle;
const osThreadAttr_t Blink_LED_attributes = {
  .name = "Blink_LED",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 64 * 4
};
/* Definitions for ScreenMutex */
osMutexId_t ScreenMutexHandle;
const osMutexAttr_t ScreenMutex_attributes = {
  .name = "ScreenMutex"
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
/* Definitions for UartLogQ */
osMessageQueueId_t UartLogQHandle;
const osMessageQueueAttr_t UartLogQ_attributes = {
  .name = "UartLogQ"
};
/* Definitions for IMUDmaDoneSem */
osSemaphoreId_t IMUDmaDoneSemHandle;
const osSemaphoreAttr_t IMUDmaDoneSem_attributes = {
  .name = "IMUDmaDoneSem"
};
/* Definitions for UartTxDoneSemHandle */
osSemaphoreId_t UartTxDoneSemHandleHandle;
const osSemaphoreAttr_t UartTxDoneSemHandle_attributes = {
  .name = "UartTxDoneSemHandle"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void FormatFloat2(char *buffer, size_t buffer_size, float value);
static void DrawValueIfChanged(uint16_t x, uint16_t y, uint16_t width,
                               const char *value_text, char *last_text,
                               size_t last_text_size);

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* creation of ScreenMutex */
  ScreenMutexHandle = osMutexNew(&ScreenMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */
  /* creation of IMUDmaDoneSem */
  IMUDmaDoneSemHandle = osSemaphoreNew(1, 0, &IMUDmaDoneSem_attributes);

  /* creation of UartTxDoneSemHandle */
  UartTxDoneSemHandleHandle = osSemaphoreNew(1, 0, &UartTxDoneSemHandle_attributes);

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
  /* creation of UartLogQ */
  UartLogQHandle = osMessageQueueNew (8, sizeof(UARTSample_t), &UartLogQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of Read_IMU_Task */
  Read_IMU_TaskHandle = osThreadNew(StartImuTask, NULL, &Read_IMU_Task_attributes);

  /* creation of Update_Dashboard_Task */
  Update_Dashboard_TaskHandle = osThreadNew(StartDashboardTask, NULL, &Update_Dashboard_Task_attributes);

  /* creation of Update_Display */
  Update_DisplayHandle = osThreadNew(StartDisplayTask, NULL, &Update_Display_attributes);

  /* creation of Update_UART_Log */
  Update_UART_LogHandle = osThreadNew(Update_UART_Log, NULL, &Update_UART_Log_attributes);

  /* creation of Blink_LED */
  Blink_LEDHandle = osThreadNew(Blink_LED, NULL, &Blink_LED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  configASSERT(ScreenMutexHandle != NULL);
  configASSERT(IMUDmaDoneSemHandle != NULL);
  configASSERT(ImuSampleQHandle != NULL);
  configASSERT(UiStateQHandle != NULL);
  configASSERT(UartLogQHandle != NULL);
  configASSERT(Read_IMU_TaskHandle != NULL);
  configASSERT(Update_Dashboard_TaskHandle != NULL);
  configASSERT(Update_DisplayHandle != NULL);
  configASSERT(Update_UART_LogHandle != NULL);
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
  uint8_t gyro_raw[6] = {0};
  uint8_t accel_raw[6] = {0};
  int8_t gyro_offset[3] = {0};

  MPU6500_Gyro_Data gyro;
  MPU6500_Accel_Data accel;

  mpu_config.write = I2C_Write;
  mpu_config.read = I2C_Read;
  
  MPU6500_Init(&mpu_config);
  MPU6500_Gyro_Calibration(&mpu_config, gyro_offset);

  mpu_config.write = I2C_Write_DMA;
  mpu_config.read = I2C_Read_DMA;

  uint32_t next = osKernelGetTickCount();

  for (;;) {
    // Read gyro and accel sequentially because they share the I2C DMA path.
    if (MPU6500_Read_Gyro_DMA(&mpu_config, gyro_raw) == 0) {
      if (osSemaphoreAcquire(IMUDmaDoneSemHandle, 20) == osOK) {
        MPU6500_Process_Gyro_DMA(&mpu_config, gyro_raw, &gyro);
      } else {
        // Keep the task periodic if a DMA transfer does not finish in time.
        next += 10;
        osDelayUntil(next);
        continue;
      }
    } else {
      next += 10;
      osDelayUntil(next);
      continue;
    }

    if (MPU6500_Read_Accel_DMA(&mpu_config, accel_raw) == 0) {
      if (osSemaphoreAcquire(IMUDmaDoneSemHandle, 20) == osOK) {
        MPU6500_Process_Accel_DMA(&mpu_config, accel_raw, &accel);
      } else {
        next += 10;
        osDelayUntil(next);
        continue;
      }
    } else {
      next += 10;
      osDelayUntil(next);
      continue;
    }

    sample.Gyro_X = gyro.Gyro_X;
    sample.Gyro_Y = gyro.Gyro_Y;
    sample.Gyro_Z = gyro.Gyro_Z;
    sample.Accel_X = accel.Accel_X;
    sample.Accel_Y = accel.Accel_Y;
    sample.Accel_Z = accel.Accel_Z;

    osMessageQueuePut(ImuSampleQHandle, &sample, 0U, 0U);

    // osDelayUntil keeps a fixed 10 ms sample period and handles tick wrap.
    next += 10;
    osDelayUntil(next);
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

  IMUSample_t sample;
  UARTSample_t uart_sample;
  UIState_t ui;
  uint32_t last_ui_update_tick = 0U;
  uint32_t last_uart_log_tick = 0U;

  /* Infinite loop */
  for (;;) {
    if (osMessageQueueGet(ImuSampleQHandle, &sample, NULL, osWaitForever) ==
        osOK) {
      // One consumer receives raw IMU samples and fans out copies to outputs.
      ui.Accel_X = sample.Accel_X;
      ui.Accel_Y = sample.Accel_Y;
      ui.Accel_Z = sample.Accel_Z;
      ui.Gyro_X = sample.Gyro_X;
      ui.Gyro_Y = sample.Gyro_Y;
      ui.Gyro_Z = sample.Gyro_Z;

      uint32_t now = osKernelGetTickCount();

      if ((now - last_ui_update_tick) >= UI_UPDATE_PERIOD_MS) {
        osMessageQueuePut(UiStateQHandle, &ui, 0U, 0U);
        last_ui_update_tick = now;
      }

      if ((now - last_uart_log_tick) >= UART_LOG_PERIOD_MS) {
        uart_sample.Accel_X = sample.Accel_X;
        uart_sample.Accel_Y = sample.Accel_Y;
        uart_sample.Accel_Z = sample.Accel_Z;
        uart_sample.Gyro_X = sample.Gyro_X;
        uart_sample.Gyro_Y = sample.Gyro_Y;
        uart_sample.Gyro_Z = sample.Gyro_Z;

        osMessageQueuePut(UartLogQHandle, &uart_sample, 0U, 0U);
        last_uart_log_tick = now;
      }
    }
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
  UIState_t ui;
  char value_text[12];
  char last_accel_x[12] = "";
  char last_accel_y[12] = "";
  char last_accel_z[12] = "";
  char last_gyro_x[12] = "";
  char last_gyro_y[12] = "";
  char last_gyro_z[12] = "";

  if (osMutexAcquire(ScreenMutexHandle, osWaitForever) != osOK) {
    return;
  }

  ST7789V3_init(&st7789_config);

  InvertDisplay(&st7789_config, INVON);
  SetRotation(&st7789_config, Landscape);

  FillScreen(&st7789_config, ORANGE);

  DrawString(&st7789_config, 0, 0, "AX:", WHITE, &Font_24x24);
  DrawString(&st7789_config, 0, 56, "AY:", WHITE, &Font_24x24);
  DrawString(&st7789_config, 0, 112, "AZ:", WHITE, &Font_24x24);

  DrawString(&st7789_config, 160, 0, "GX:", WHITE, &Font_24x24);
  DrawString(&st7789_config, 160, 56, "GY:", WHITE, &Font_24x24);
  DrawString(&st7789_config, 160, 112, "GZ:", WHITE, &Font_24x24);

  osMutexRelease(ScreenMutexHandle);

  /* Infinite loop */
  for (;;) {
    if (osMessageQueueGet(UiStateQHandle, &ui, NULL, osWaitForever) == osOK) {
      // Render the newest UI state if drawing falls behind the sample rate.
      while (osMessageQueueGet(UiStateQHandle, &ui, NULL, 0U) == osOK) {
      }

      if (osMutexAcquire(ScreenMutexHandle, osWaitForever) != osOK) {
        continue;
      }

      FormatFloat2(value_text, sizeof(value_text), ui.Accel_X);
      DrawValueIfChanged(0, 24, 150, value_text, last_accel_x,
                         sizeof(last_accel_x));

      FormatFloat2(value_text, sizeof(value_text), ui.Accel_Y);
      DrawValueIfChanged(0, 80, 150, value_text, last_accel_y,
                         sizeof(last_accel_y));

      FormatFloat2(value_text, sizeof(value_text), ui.Accel_Z);
      DrawValueIfChanged(0, 136, 150, value_text, last_accel_z,
                         sizeof(last_accel_z));

      snprintf(value_text, sizeof(value_text), "%d", ui.Gyro_X);
      DrawValueIfChanged(160, 24, 150, value_text, last_gyro_x,
                         sizeof(last_gyro_x));

      snprintf(value_text, sizeof(value_text), "%d", ui.Gyro_Y);
      DrawValueIfChanged(160, 80, 150, value_text, last_gyro_y,
                         sizeof(last_gyro_y));

      snprintf(value_text, sizeof(value_text), "%d", ui.Gyro_Z);
      DrawValueIfChanged(160, 136, 150, value_text, last_gyro_z,
                         sizeof(last_gyro_z));

      osMutexRelease(ScreenMutexHandle);
    }
  }
  /* USER CODE END Update_Display */
}

/* USER CODE BEGIN Header_Update_UART_Log */
/**
 * @brief Function implementing the Update_UART_Log thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Update_UART_Log */
void Update_UART_Log(void *argument)
{
  /* USER CODE BEGIN Update_UART_Log */
  UARTSample_t sample;
  char buffer[128];
  char accel_x_text[12];
  char accel_y_text[12];
  char accel_z_text[12];
  /* Infinite loop */
  for (;;) {
    if (osMessageQueueGet(UartLogQHandle, &sample, NULL, osWaitForever) ==
        osOK) {
      FormatFloat2(accel_x_text, sizeof(accel_x_text), sample.Accel_X);
      FormatFloat2(accel_y_text, sizeof(accel_y_text), sample.Accel_Y);
      FormatFloat2(accel_z_text, sizeof(accel_z_text), sample.Accel_Z);

      snprintf(buffer, sizeof(buffer),
               "Gyro: X |%-4i|, Y|%-4i|, Z|%-4i| --- |  Accel: X |%s|, Y "
               "|%s|, Z |%s| \r\n",
               sample.Gyro_X, sample.Gyro_Y, sample.Gyro_Z, accel_x_text,
               accel_y_text, accel_z_text);

      HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), 100);
    }
  }
  /* USER CODE END Update_UART_Log */
}

/* USER CODE BEGIN Header_Blink_LED */
/**
* @brief Function implementing the Blink_LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Blink_LED */
void Blink_LED(void *argument)
{
  /* USER CODE BEGIN Blink_LED */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
    osDelay(750);
  }
  /* USER CODE END Blink_LED */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// Format two decimal places without requiring printf float support.
static void FormatFloat2(char *buffer, size_t buffer_size, float value) {
  const char *sign = (value < 0.0f) ? "-" : "";
  float abs_value = (value < 0.0f) ? -value : value;
  int32_t centi_value = (int32_t)((abs_value * 100.0f) + 0.5f);
  int32_t whole = centi_value / 100;
  int32_t decimal = centi_value % 100;

  snprintf(buffer, buffer_size, "%s%ld.%02ld", sign, (long)whole,
           (long)decimal);
}

static void DrawValueIfChanged(uint16_t x, uint16_t y, uint16_t width,
                               const char *value_text, char *last_text,
                               size_t last_text_size) {
  if (strcmp(value_text, last_text) == 0) {
    return;
  }

  DrawFilledRectangle(&st7789_config, x, y, width, 24, ORANGE);
  DrawString(&st7789_config, x, y, value_text, WHITE, &Font_24x24);
  snprintf(last_text, last_text_size, "%s", value_text);
}

/* USER CODE END Application */

