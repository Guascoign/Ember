/********************************************************************************
    * 文件名称 ：uart_printf.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-05-08
    * 功     能：串口打印重写
*********************************************************************************
V1.0 2024-05-08 add STM32 HAL
V1.1 2025-01-08 更换为UART设备结构体
*********************************************************************************/

#include "uart_printf.h"
#include "BSP/UART/uart_pack.h"
#include <stdio.h>

int fputc(int ch, FILE *f)
{
    Printf_Uart.Send(&Printf_Uart, (uint8_t *)&ch, 1, 0xFFff);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    Printf_Uart.Recv(&Printf_Uart, &ch, 0xffff);
    return ch;
}


