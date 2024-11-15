#ifndef _WS2812B_H
#define _WS2812B_H
#endif

//#include "main.h"
#include "tim.h"
#include "stdio.h"

#define PIXEL_NUM  4
#define NUM (24*PIXEL_NUM + 50)        // Reset 50us  T0H 0.35us  T1H 1.36us   T0L 1.36us   T1L 0.35us  锟斤拷频125
#define WS1 95
#define WS0  30


typedef struct {
    uint16_t h; // 色相 (0.0 to 360.0) 
    uint8_t s; // 饱和度 (0 to 255) 越低越灰
    uint8_t v; // 亮度 (0 to 255) 越高越亮
} HSVColor;

typedef struct {
    uint8_t r; // 红色分量 (0 to 255)
    uint8_t g; // 绿色分量 (0 to 255)
    uint8_t b; // 蓝色分量 (0 to 255)
} RGBColor;

void WS_Load(void);
void WS_WriteAll_RGB(uint8_t n_R, uint8_t n_G, uint8_t n_B);
void WS_CloseAll(void);
void WS_SetPixelColor(uint16_t n, uint32_t GRBColor);
void WS_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue);
uint32_t WS281x_Wheel(uint8_t wheelPos);
void WS_ColorWipe(uint32_t c, uint8_t wait);
void WS_Rainbow(uint8_t wait);
void WS_RainbowCycle(uint8_t wait);
void WS_TheaterChase(uint32_t c, uint8_t wait);
void WS_TheaterChaseRainbow(uint8_t wait);
RGBColor WS_HSV_To_RGB(HSVColor hsv);
void WS_Breath(HSVColor HSV , uint8_t wait);
