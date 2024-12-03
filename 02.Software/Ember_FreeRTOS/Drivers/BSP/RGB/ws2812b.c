
#include "ws2812b.h"

uint8_t LED_Data[LED_COUNT][3]; // ÿ��LED����3���ֽڣ�RGB��
uint16_t PWM_Buffer[LED_COUNT * 24 + 50]; // ÿ��LED 24λ���� + ��λ�ź�


void Set_LED_Color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    LED_Data[index][0] = green; // WS2812ʹ��GRB��ʽ
    LED_Data[index][1] = red;
    LED_Data[index][2] = blue;
}

// �������ݵ�WS2812
void WS2812_Send(void)
{
    uint16_t pos = 0;

    // ���PWM_Buffer
    for (int i = 0; i < LED_COUNT; i++) {
        for (int j = 0; j < 24; j++) {
            if (LED_Data[i][j / 8] & (1 << (7 - (j % 8)))) {
                PWM_Buffer[pos++] = 100; // T1H
            } else {
                PWM_Buffer[pos++] = 25; // T0H
            }
        }
    }

    // ��临λ�ź�
    for (int i = 0; i < 50; i++) {
        PWM_Buffer[pos++] = 0;
    }

    // ��ʼPWM�źŴ���
    HAL_TIM_PWM_Start_DMA(&htim5, TIM_CHANNEL_2, (uint32_t *)PWM_Buffer, pos);
    HAL_Delay(10); // �ȴ����ݷ������
}
