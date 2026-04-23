# Architecture Notes

This firmware is intentionally small. The purpose is to show basic RTOS concepts

## Runtime Flow

```text
MPU6500 over I2C DMA
  -> Read_IMU_Task
  -> ImuSampleQ
  -> Update_Dashboard_Task
  -> UiStateQ + UartLogQ
  -> Update_Display + Update_UART_Log
  -> ST7789V3 LCD + USART3 USB-C VCP
```

## RTOS Primitives

- Queue: passes sensor samples between tasks without sharing live variables.
- Semaphore: lets the IMU task sleep until the I2C DMA transfer completes.
- Mutex: keeps display writes serialized so multiple tasks cannot draw at once.

## File Ownership

- `Core/Src/main.c`: CubeMX startup, peripheral init, and board-specific driver
  callbacks.
- `Core/Src/app_freertos.c`: RTOS objects, tasks, and data flow.
- `Custom_Drivers/`: reusable sensor and display drivers.

The CubeMX-generated structure is kept because it shows how the application fits
inside a real STM32 firmware project.
