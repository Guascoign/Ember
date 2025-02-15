/********************************************************************************
    * 文件名称 ：led_device.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LED驱动
*********************************************************************************/
#ifndef __LED_DEVICE_H
#define __LED_DEVICE_H

#include "BSP/Peripherals/Peripherals.h"
#include "BSP/GPIO/gpio_device.h"
#include "LIB/soft_timer.h"//软件定时器


#define USE_HAL //使用STM32 HAL库

#ifdef USE_HAL
#include "stm32f4xx_hal.h"
#endif  //USE_HAL

/**
 * @brief    LED闪烁类型
 * @param    LED_Blink_Single	单闪
 * @param    LED_Blink_Double	双闪
 * @param    LED_Blink_Three		三闪
 * @param    LED_ON				常亮
 * @param    LED_OFF				常灭
 */
typedef enum {
    LED_Blink_Single = 0x00U, //单闪
    LED_Blink_Double = 0x01U, //双闪
    LED_Blink_Three = 0x02U, //三闪
    LED_ON = 0x03U, //常亮
    LED_OFF = 0x04U, //常灭
}LED_BlinkType;

/**
 * @brief    LED闪烁模式
 * @param    continue_Blink	循环
 * @param    single_Blink		单次
 */
typedef enum {
    continue_Blink = 0x00U, //循环
    single_Blink = 0x01U, //单次
}LED_BlinkMode;

/**
 * @brief    LED闪烁延时
 * @param    LED_Delay_100ms	100ms
 * @param    LED_Delay_200ms	200ms
 * @param    LED_Delay_500ms	500ms
 * @param    LED_Delay_1000ms	1000ms
 */
typedef enum {
    LED_Delay_100ms = 100, //100ms
    LED_Delay_200ms = 200, //200ms
    LED_Delay_500ms = 500, //500ms
    LED_Delay_1000ms = 1000, //1000ms
}LED_Delay;//闪烁延时 n x 1ms + 2 x 1ms 为一组

/**
 * @brief    LED闪烁结构体
 * @param    blink	闪烁
 * @param    mode	闪烁模式
 * @param    delay	闪烁延时
 * @param    flag	闪烁标志位，记录次数
 */
typedef struct {
    LED_BlinkType blink; //闪烁
    LED_BlinkMode mode; //闪烁模式
    LED_Delay delay; //闪烁延时
    uint8_t flag; //闪烁标志位，记录次数
}LED_BlinkTypeDef;

/**
 * @brief    LED设备结构体
 * @param char name	设备名称
 * @param    blink	闪烁
 * @param    timer	定时器
 * @param    Init	初始化函数
 * @param    Set	设置函数
 * @param    priv_data	私有数据
 */
typedef struct LED_Device {
    char *name; //设备名称
    LED_BlinkTypeDef blink; //闪烁
    Soft_TimerTypeDef timer; // 定时器
    uint8_t (*Init)(struct LED_Device *p_LEDDev);//初始化,绑定GPIO
    uint8_t (*Set)(struct LED_Device *p_LEDDev, LED_BlinkType blink ,LED_BlinkMode mode);//设置
    void *priv_data;//私有数据 存入GPIO_DeviceTypeDef句柄
}LED_DeviceTypeDef, *p_LED_DeviceTypeDef;

int8_t LED_Init(LED_DeviceTypeDef *p_LEDDev,char *name ,void *Instance ,uint16_t pin);

void LED_Callback(void *args);  //定时器回调函数

#endif



