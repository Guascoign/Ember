/********************************************************************************
    * 文件名称 ：uartpack.h
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：UART串口封装
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL
V1.1 2025-01-08 更换为UART设备结构体 暂只支持阻塞
*********************************************************************************/
/*使用说明*************************************************************************
UART_DeviceTypeDef Test_Uart; 全局变量声明
UART_Init(&Test_Uart,"Test_Uart_Uart3",USART3,UART_MODE_NORMAL,Rate_115200,Word_length_8B,PARITY_NONE,Stop_bits_1); 初始化函数
Printf_Uart.Send(&Printf_Uart, (uint8_t *)&ch, 1, 0xFFff); 发送函数
Printf_Uart.Recv(&Printf_Uart, &ch, 0xffff); 接收函数
*********************************************************************************/
#ifndef _UART_PACK_H
#define _UART_PACK_H

#include <stdint.h>
#include "string.h"
#include <stdlib.h>
#include "BSP/Peripherals/Peripherals.h"

#define USE_HAL //使用STM32 HAL库
// #define USE_REG //使用寄存器库
// #define USE_LL  //使用LL库

#ifdef USE_HAL
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#endif  //USE_HAL

#ifdef USE_REG
#include "stm32f1xx.h"
#endif  //USE_REG

#ifdef USE_LL
#include "stm32f1xx_ll_usart.h"
#endif  //USE_LL

/**
  * @brief  串口工作模式
  * @note  该枚举类型定义了串口的工作模式，包括普通模式、中断模式和DMA模式
  * @param  UART_MODE_NORMAL 普通模式
  * @param  UART_MODE_IT    中断模式
  * @param  UART_MODE_DMA   DMA模式
  */
typedef enum {
    UART_MODE_NORMAL    = 0X00U,   //串口普通模式
    UART_MODE_IT        = 0X01U,   //串口中断驱动模式
    UART_MODE_DMA       = 0X02U    //串口DMA驱动模式
} UART_ModeTypeDef;

/**
  * @brief  奇偶校验
  * @note  该枚举类型定义了串口的奇偶校验方式，包括无奇偶、奇数奇偶和偶数奇偶
  * @param  PARITY_NONE 无奇偶
  * @param  PARITY_ODD  奇数奇偶
  * @param  PARITY_EVEN 偶数奇偶
  */
typedef enum {
    PARITY_NONE = 0X00U, //无奇偶
    PARITY_ODD  = 0X01U, //奇数奇偶
    PARITY_EVEN = 0X02U  //偶数奇偶
} UART_ParityTypeDef;

/**
  * @brief  停止位
  * @note  该枚举类型定义了串口的停止位数，包括1位停止位和2位停止位
  * @param  Stop_bits_1 1位停止位
  * @param  Stop_bits_2 2位停止位
  */
typedef enum {
    Stop_bits_1 = 0X00U, //1位停止位
    Stop_bits_2 = 0X01U  //2位停止位
} UART_StopBitsTypeDef;

/**
  * @brief  数据位
  * @note  该枚举类型定义了串口的数据位数，包括8位数据位和9位数据位
  * @param  Word_length_8B 8位数据位
  * @param  Word_length_9B 9位数据位
  */
typedef enum {
    Word_length_8B = 0X00U, //8位数据位
    Word_length_9B = 0X01U  //9位数据位
} UART_WordLengthTypeDef;

/**
  * @brief  波特率
  * @note  该枚举类型定义了串口的波特率，包括115200、9600、4800、2400和1200
  * @param  Rate_115200 115200波特率
  * @param  Rate_9600 9600波特率
  * @param  Rate_4800 4800波特率
  * @param  Rate_2400 2400波特率
  * @param  Rate_1200 1200波特率
  */
typedef enum {
    Rate_115200 = 0X00U, //115200波特率
    Rate_9600  = 0X01U, //9600波特率
    Rate_4800  = 0X02U, //4800波特率
    Rate_2400  = 0X03U, //2400波特率
    Rate_1200  = 0X04U //1200波特率
} UART_RateTypeDef;


/**
  * @brief  UART设备结构体
  * @note  该结构体中包含了串口的基本信息和操作函数
  * @param  name 设备名称
  * @param  Init 设备初始化函数
  * @param  Send 设备发送函数
  * @param  Recv 设备接收函数
  * @param  priv_data 私有数据，存储UART_HandleTypeDef结构体指针
  */
typedef struct UART_Device {
    char *name; //设备名称
    int (*Init)(struct UART_Device *p_UARTDev,void *Instance , UART_RateTypeDef rate, UART_WordLengthTypeDef wordlength, UART_ParityTypeDef parity, UART_StopBitsTypeDef stop);//初始化
    int (*Send)(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms);//发送
    int (*Recv)(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms);//接收
    void *priv_data;//私有数据 存入UART_HandleTypeDef结构体指针
}UART_DeviceTypeDef;

/**
  * @brief   初始化UART设备
  * @param   name    设备名称
  * @param   Instance    UART外设实例基地址
  * @param   Status  工作模式，'I'表示中断模式，'DMA'表示DMA模式，其他值表示普通模式
  * @param   baud    波特率
  * @param   datas   数据位数
  * @param   parity  奇偶校验类型
  * @param   stop    停止位数
  * @return  返回值为0表示初始化成功，其他值表示初始化失败
  */
int8_t UART_Init(
    UART_DeviceTypeDef *p_UARTDev,
    char *name, 
    void *Instance, 
    UART_ModeTypeDef Mode ,
    UART_RateTypeDef Rate, 
    UART_WordLengthTypeDef WordLength, 
    UART_ParityTypeDef Parity, 
    UART_StopBitsTypeDef StopBits);   

// struct UART_Device *GetUARTDevice(char *name);//获取外设地址指针

#endif

