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

#include "uart_printf.h"
#include "BSP/UART/uart_device.h"
#include <stdio.h>
#include "task.h"

/**
 * @brief   串口打印
 * 
 * @param ch    字
 * @param *f     文件指针
 * 
 * @return ch   字
 */
int fputc(int ch, FILE *f)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    Printf_Uart.Send(&Printf_Uart, (uint8_t *)&ch, 1, 0xFFff);
    taskEXIT_CRITICAL();            /* 退出临界区 */
    return ch;
}

/**
 * @brief   串口接收
 * 
 * @param *f     文件指针
 * 
 * @return ch   字
 */
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    Printf_Uart.Recv(&Printf_Uart, &ch, 0xffff);
    return ch;
}


