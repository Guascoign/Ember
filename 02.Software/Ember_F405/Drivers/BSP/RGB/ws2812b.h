#ifndef __WS2812B_H__
#define __WS2812B_H__

#include "stdint.h"  // 包含 stdint.h 以便使用 uint16_t



#define WS2812B_ARR 125 // TIM2的自动重装值//使得PWM输出频率在800kHz
#define T0H 35          // 0编码高电平时间占1/3
#define T1H 70          // 1编码高电平时间占2/3

/*使用灯珠的个数*/
#define WS2812B_NUM 3
#define DATA_SIZE 24 // WS2812B传输一个数据的大小是3个字节（24bit）
#define DMA_DATA_SIZE (DATA_SIZE * WS2812B_NUM +50)

void PWM_WS2812B_Init(uint16_t arr);
void WS2812B_Write_24Bits(uint16_t num, uint32_t GRB_Data);
void WS2812B_Show(void);
void PWM_WS2812B_Red(uint16_t num);
void PWM_WS2812B_Green(uint16_t num);
void PWM_WS2812B_Blue(uint16_t num);
void PWM_WS2812B_Close(uint16_t num);

void WS2812B_Write_24Bits_independence(uint16_t num, uint32_t *GRB_Data); // 独立写像素的颜色
void set_ws2812_breathing(uint8_t index);                                 // 呼吸灯

#endif
