
#include "ws2812b.h"

 uint16_t send_Buf[NUM]; //���ͺ�����������


/**
 * @brief    DMA����PWM����
 *
 * @return  void
 */
void WS_Load(void)
{
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2, (uint32_t *)send_Buf, NUM);
}

/**
 * @brief    �ر����еƹ�
 *
 * @return  void
 */
void WS_CloseAll(void)
{
	uint16_t i;
	for (i = 0; i < PIXEL_NUM * 24; i++)
		send_Buf[i] = WS0; // д���߼�0��ռ�ձ�
	for (i = PIXEL_NUM * 24; i < NUM; i++)
		send_Buf[i] = 0; // ռ�ձȱ�Ϊ0��ȫΪ�͵�ƽ
	WS_Load();
}

/**
 * @brief    д��������ɫ����
 *
 * @param   red    ��ɫ 0-255
 * @param   green  ��ɫ 0-255
 * @param   blue   ��ɫ 0-255
 *
 * @return  void
 */
uint32_t WS_Color(uint8_t red, uint8_t green, uint8_t blue)
{
	return green << 16 | red << 8 | blue;
}

/**
 * @brief    д��n���Ƶ���ɫ
 *
 * @param   n    �Ƶ�˳��
 * @param   GRBColor  ��ɫ����
 *
 * @return  void
 */
void WS_SetPixelColor(uint16_t n, uint32_t GRBColor)
{
	uint8_t i;
	if (n < PIXEL_NUM)
	{
		for (i = 0; i < 24; ++i)
			send_Buf[24 * n + i] = (((GRBColor << i) & 0X800000) ? WS1 : WS0);
	}
	WS_Load();
}

/**
 * @brief    д��n���Ƶ���ɫ
 *
 * @param   n    �Ƶ�˳��
 * @param   red    ��ɫ 0-255
 * @param   green  ��ɫ 0-255
 * @param   blue   ��ɫ 0-255
 *
 * @return  void
 */
void WS_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t i;

	if (n < PIXEL_NUM)
	{
		for (i = 0; i < 24; ++i)
			send_Buf[24 * n + i] = (((WS_Color(red, green, blue) << i) & 0X800000) ? WS1 : WS0);
	}
	WS_Load();
}

/**
 * @brief    дȫ��������
 *
 * @param   red    ��ɫ 0-255
 * @param   green  ��ɫ 0-255
 * @param   blue   ��ɫ 0-255
 *
 * @return  void
 */
void WS_WriteAll_RGB(uint8_t n_R, uint8_t n_G, uint8_t n_B)
{
	uint16_t i, j;
	uint8_t dat[24];
	// ��RGB���ݽ���ת��
	for (i = 0; i < 8; i++)
	{
		dat[i] = ((n_G & 0x80) ? WS1 : WS0);
		n_G <<= 1;
	}
	for (i = 0; i < 8; i++)
	{
		dat[i + 8] = ((n_R & 0x80) ? WS1 : WS0);
		n_R <<= 1;
	}
	for (i = 0; i < 8; i++)
	{
		dat[i + 16] = ((n_B & 0x80) ? WS1 : WS0);
		n_B <<= 1;
	}
	for (i = 0; i < PIXEL_NUM; i++)
	{
		for (j = 0; j < 24; j++)
		{
			send_Buf[i * 24 + j] = dat[j];
		}
	}
	for (i = PIXEL_NUM * 24; i < NUM; i++)
		send_Buf[i] = 0; // ռ�ձȱ�Ϊ0��ȫΪ�͵�ƽ
	WS_Load();
}

/**
 * @brief    RGB��ɫתr
 *
 * @param   wheelPos  ��ɫ�Ƕ�
 *
 * @return  uint32_t
 */
uint32_t WS_Wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return WS_Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return WS_Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return WS_Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

/**
 * @brief    ��ɫ��д
 *
 * @param GRBColor
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_ColorWipe(uint32_t GRBColor, uint8_t wait) {
  for(uint16_t i=0; i<PIXEL_NUM; i++) {
    WS_SetPixelColor(i, GRBColor);
    WS_Load();
    HAL_Delay(wait);
  }
}

/**
 * @brief    �ʺ�
 *
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_Rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM; i++) {
      WS_SetPixelColor(i, WS_Wheel((i+j) & 255));
    }
    WS_Load();
    HAL_Delay(wait);
  }
}

/**
 * @brief    �ʺ�ѭ��(����)
 *
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_RainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM; i++) {
      WS_SetPixelColor(i,WS_Wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    WS_Load();
    HAL_Delay(wait);
  }
}

/**
 * @brief    ����ѭ�� Ƶ��
 *
 * @param GRBColor 32λ��ɫ
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_TheaterChase(uint32_t GRBColor, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        WS_SetPixelColor(i+q, GRBColor);    //turn every third pixel on
      }
      WS_Load();

      HAL_Delay(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        WS_SetPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

/**
 * @brief    ��ɫ����ѭ�� Ƶ��
 *
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_TheaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        WS_SetPixelColor(i+q, WS_Wheel( (i+j) % 255));    //turn every third pixel on
      }
      WS_Load();

      HAL_Delay(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        WS_SetPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}



/**
 * @brief    RGBתHSV
 *
 * @param   red    ��ɫ 0-255
 * @param   green  ��ɫ 0-255
 * @param   blue   ��ɫ 0-255
 *
 * @return  RGBColor
 */
RGBColor WS_HSV_To_RGB(HSVColor hsv)
{
 RGBColor rgb;
    int i;
    double f, p, q, t;

    if (hsv.s == 0) {
        // ������Ͷ�Ϊ0����ɫΪ�Ҷ�
        rgb.r = rgb.g = rgb.b = (int)hsv.v;
        return rgb;
    }

    hsv.h /= 60; // ��ɫ�����Ϊ����[0, 6)
    i = (int)hsv.h;
    f = hsv.h - i;

    p = hsv.v * (1 - hsv.s / 255.0);
    q = hsv.v * (1 - hsv.s / 255.0 * f);
    t = hsv.v * (1 - hsv.s / 255.0 * (1 - f));

    switch (i) {
        case 0: rgb.r = (int)hsv.v; rgb.g = (int)t; rgb.b = (int)p; break;
        case 1: rgb.r = (int)q; rgb.g = (int)hsv.v; rgb.b = (int)p; break;
        case 2: rgb.r = (int)p; rgb.g = (int)hsv.v; rgb.b = (int)t; break;
        case 3: rgb.r = (int)p; rgb.g = (int)q; rgb.b = (int)hsv.v; break;
        case 4: rgb.r = (int)t; rgb.g = (int)p; rgb.b = (int)hsv.v; break;
        default: rgb.r = (int)hsv.v; rgb.g = (int)p; rgb.b = (int)q; break;
    }

    return rgb;
}



/**
 * @brief    ������Ч
 *
 * @param GRBColor  ��ɫ����
 * @param wait ��ʱʱ��us
 *
 * @return  void
 */
void WS_Breath(HSVColor HSV , uint8_t wait)
{
  RGBColor RGB;
  for(; HSV.v<=254 ; HSV.v ++)  //V���� �ƹ⽥��
  {
    RGB = WS_HSV_To_RGB(HSV);
    WS_WriteAll_RGB(RGB.r , RGB.g ,RGB.b);
    HAL_Delay(wait);
  }

  for(; HSV.v>=1 ; HSV.v --)  //V�Լ� �ƹ⽥��
  {
    RGB = WS_HSV_To_RGB(HSV);
    WS_WriteAll_RGB(RGB.r , RGB.g ,RGB.b);
    HAL_Delay(wait);
  }
}
