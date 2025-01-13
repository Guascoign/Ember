/********************************************************************************
    * 文件名称 ：gpio_device.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#ifndef __GPIO_DEVICE_H
#define __GPIO_DEVICE_H

#include "BSP/Peripherals/Peripherals.h"

#define USE_HAL //使用STM32 HAL库

#ifdef USE_HAL
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#endif  //USE_HAL

typedef enum {
    PIN_RESET = 0x00U, //低电平
    PIN_SET = 0x01U //高电平
} PinState;

typedef enum {
    GPIO_INPUT = 0x00U, //输入模式
    GPIO_OUTPUT_PP = 0x01U, //推挽输出模式
    GPIO_OUTPUT_OD = 0x02U, //开漏输出模式
}GPIO_ModeTypeDef;

typedef struct GPIO_Device {
    char *name; //设备名称
    GPIO_ModeTypeDef Mode; //模式
    void *instance; //基地址 GPIO_TypeDef
    uint16_t pin; //引脚
    uint8_t (*Init)(struct GPIO_Device *p_GPIODev);//初始化
    uint8_t (*DeInit)(struct GPIO_Device *p_GPIODev);//去初始化
    void (*Set)(struct GPIO_Device *p_GPIODev, GPIO_PinState PinState);//设置
    uint8_t (*Read)(struct GPIO_Device *p_GPIODev);//获取
    void *priv_data;//私有数据
}GPIO_DeviceTypeDef, *p_GPIO_DeviceTypeDef;

int8_t GPIO_Init(GPIO_DeviceTypeDef *p_GPIODev,char *name ,void *Instance ,uint16_t pin);
#endif



