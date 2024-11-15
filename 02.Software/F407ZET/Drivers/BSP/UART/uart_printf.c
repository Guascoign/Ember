/********************************************************************************
    * 文件名称 ：uart_printf.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-05-08
    * 功     能：UART串口封装
*********************************************************************************
V1.0 2024-05-08 add STM32 HAL

*********************************************************************************/

#include "uart_printf.h"
#include "usart.h"
#include <stdio.h>

int fputc(int ch, FILE *f)
{

    HAL_UART_Transmit(&Print_UART, (uint8_t *)&ch, 1, 0xFFff);

    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&Print_UART, &ch, 1, 0xffff);
    return ch;
}


