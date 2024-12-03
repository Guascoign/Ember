#ifndef _WS2812B_H
#define _WS2812B_H


//#include "main.h"
#include "tim.h"
#include "stdio.h"
#include <stdint.h>  // 用于定义 uint16_t 类型

#define LED_COUNT 2
#define PWM_RESOLUTION 125  // WS2812周期1.25us对应的时钟周期数（主频100 MHz）



// 设置LED颜色
void Set_LED_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

// 将颜色数据转换为PWM信号
void WS2812_Send(void);

#endif//_WS2812B_H
