
#include "ws2812b.h"

uint8_t LED_Data[LED_COUNT][3]; // 每个LED包含3个字节（RGB）
uint16_t PWM_Buffer[LED_COUNT * 24 + 50]; // 每个LED 24位数据 + 复位信号


void Set_LED_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    LED_Data[index][0] = green; // WS2812使用GRB格式
    LED_Data[index][1] = red;
    LED_Data[index][2] = blue;
}

// 发送数据到WS2812
void WS2812_Send(void)
{
    uint16_t pos = 0;

    // 填充PWM_Buffer
    for (int i = 0; i < LED_COUNT; i++) {
        for (int j = 0; j < 24; j++) {
            if (LED_Data[i][j / 8] & (1 << (7 - (j % 8)))) {
                PWM_Buffer[pos++] = 100; // T1H
            } else {
                PWM_Buffer[pos++] = 25; // T0H
            }
        }
    }

    // 填充复位信号
    for (int i = 0; i < 50; i++) {
        PWM_Buffer[pos++] = 0;
    }

    // 开始PWM信号传输
    HAL_TIM_PWM_Start_DMA(&htim5, TIM_CHANNEL_2, (uint32_t *)PWM_Buffer, pos);
    HAL_Delay(10); // 等待数据发送完成
}
