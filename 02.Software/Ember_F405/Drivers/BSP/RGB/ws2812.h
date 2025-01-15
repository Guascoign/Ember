/********************************************************************************
    * 文件名称 ：st7789.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#ifndef __ST7789_H
#define __ST7789_H

#include "stdint.h"
#include "main.h"
#include "tim.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_tim.h"
#include "rgb_device.h"

/* 定时器句柄 */
extern TIM_HandleTypeDef htim2;
/* DMA 句柄 */
extern DMA_HandleTypeDef hdma_tim2_ch1;


#define WS2812B_ARR 209 // TIM2的自动重装值//使得PWM输出频率在800kHz
#define T0H 63          // 0编码高电平时间占1/3
#define T1H 126          // 1编码高电平时间占2/3

/*使用灯珠的个数*/
#define WS2812B_NUM 3
#define DATA_SIZE 24 // WS2812B传输一个数据的大小是3个字节（24bit）
#define DMA_DATA_SIZE (DATA_SIZE * WS2812B_NUM +50)

typedef struct ws2812b_device
{
    char *name;
    uint8_t data[DMA_DATA_SIZE];//DMA数据缓存
    void *tim_handle;
    uint32_t tim_channel;
    void (*Init)(struct ws2812b_device *ws2812b , uint16_t arr);
    void (*DeInit)(struct ws2812b_device *ws2812b);
    void (*SetRGB)(struct ws2812b_device *ws2812b, uint8_t R, uint8_t G, uint8_t B, uint16_t num);//单像素存入数组
    void (*SetHSV)(struct ws2812b_device *ws2812b, uint8_t H, uint8_t S, uint8_t V, uint16_t num);//单像素存入数组
    void (*SetAllRGB)(struct ws2812b_device *ws2812b, uint8_t R, uint8_t G, uint8_t B);//所有像素存入数组
    void (*SetAllHSV)(struct ws2812b_device *ws2812b, uint8_t H, uint8_t S, uint8_t V);//所有像素存入数组
    void (*Update)(struct ws2812b_device *ws2812b);
} WS2812B_DeviceTypeDef, *p_WS2812B_DeviceTypeDef;

void WS2812B_Init(WS2812B_DeviceTypeDef *p_ws2812b,void *tim_handle, uint32_t tim_channel , char *name );

#endif
