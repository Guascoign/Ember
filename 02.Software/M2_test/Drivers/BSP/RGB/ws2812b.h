#ifndef _WS2812B_H
#define _WS2812B_H


//#include "main.h"
#include "tim.h"
#include "stdio.h"
#include <stdint.h>  // ���ڶ��� uint16_t ����

#define LED_COUNT 2
#define PWM_RESOLUTION 125  // WS2812����1.25us��Ӧ��ʱ������������Ƶ100 MHz��



// ����LED��ɫ
void Set_LED_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

// ����ɫ����ת��ΪPWM�ź�
void WS2812_Send(void);

#endif//_WS2812B_H
