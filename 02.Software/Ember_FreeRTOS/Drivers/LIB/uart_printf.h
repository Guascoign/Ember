/********************************************************************************
    * 文件名称 ：uart_printf.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-05-08
    * 功     能：串口打印重写
*********************************************************************************
V1.0 2024-05-08 add STM32 HAL

*********************************************************************************/

#ifndef UART_PRINTF_H
#define UART_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include <stdio.h>


extern UART_HandleTypeDef huart1;
#define Print_UART huart1

int fputc(int ch, FILE *f);
int fgetc(FILE *f);



#ifdef __cplusplus
}
#endif

#endif // UART_PRINTF_H
