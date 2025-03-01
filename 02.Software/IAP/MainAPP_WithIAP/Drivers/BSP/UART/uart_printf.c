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
/* 如果使用os,则包括下面的头文件即可. */
#if SYS_SUPPORT_OS
#include "task.h" /* os 使用 */
#endif


/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

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
    Printf_Uart.Send(&Printf_Uart, (uint8_t *)&ch, 1, 0xFFff);
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
    while ((((UART_HandleTypeDef *)(Printf_Uart.priv_data))->Instance->SR & USART_SR_RXNE) == 0); // 等待接收寄存器非空
    ch = (uint8_t)(((UART_HandleTypeDef *)(Printf_Uart.priv_data))->Instance->DR & 0xFF); // 读取接收到的字符

    return ch;
}

#endif

