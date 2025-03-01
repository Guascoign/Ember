/********************************************************************************
    * 文件名称 ：uart_printf.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-08
    * 功     能：串口打印重写
*********************************************************************************
V1.0 2024-05-08 add STM32 HAL
V1.1 2025-01-08 更换为UART设备结构体
*********************************************************************************/

#ifndef UART_PRINTF_H
#define UART_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

extern struct UART_Device Test_Uart;
#define Printf_Uart Test_Uart
int fputc(int ch, FILE *f);
int fgetc(FILE *f);



#ifdef __cplusplus
}
#endif

#endif // UART_PRINTF_H
