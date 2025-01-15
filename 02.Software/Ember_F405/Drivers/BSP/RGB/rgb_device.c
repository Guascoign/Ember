/********************************************************************************
    * 文件名称 ：rgb_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：RGB驱动
*********************************************************************************/
#include "BSP/RGB/rgb_device.h"

/**
 * @brief   HSV颜色结构体
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 */
RGB_TypeDef HSV_To_RGB(HSV_TypeDef hsv)
{
    RGB_TypeDef rgb;
    uint8_t region, remainder, p, q, t;
    if (hsv.S == 0)
    {
        rgb.R = hsv.V;
        rgb.G = hsv.V;
        rgb.B = hsv.V;
        return rgb;
    }
    region = hsv.H / 43;
    remainder = (hsv.H - (region * 43)) * 6;

    p = (hsv.V * (255 - hsv.S)) >> 8;
    q = (hsv.V * (255 - ((hsv.S * remainder) >> 8))) >> 8;
    t = (hsv.V * (255 - ((hsv.S * (255 - remainder)) >> 8))) >> 8;
    switch (region)
    {
    case 0:
        rgb.R = hsv.V;
        rgb.G = t;
        rgb.B = p;
        break;
    case 1:
        rgb.R = q;
        rgb.G = hsv.V;
        rgb.B = p;
        break;
    case 2:
        rgb.R = p;
        rgb.G = hsv.V;
        rgb.B = t;
        break;
    case 3:
        rgb.R = p;
        rgb.G = q;
        rgb.B = hsv.V;
        break;
    case 4:
        rgb.R = t;
        rgb.G = p;
        rgb.B = hsv.V;
        break;
    default:
        rgb.R = hsv.V;
        rgb.G = p;
        rgb.B = q;
        break;
    }
    return rgb;
}

/**
 * @brief   RGB颜色结构体
 * @param   *p_RGBDev   RGB设备句柄
 * @param   R   红色
 * @param   G   绿色
 * @param   B   蓝色
 */
static void set_rgb(RGB_DeviceTypeDef *p_RGBDev, uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    if (num >= p_RGBDev->num)
    {
        return;
    }
    ((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data)->SetRGB((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data, R, G, B, num);
}

/**
 * @brief   HSV颜色结构体
 * @param   *p_RGBDev   RGB设备句柄
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 */
static void set_hsv(RGB_DeviceTypeDef *p_RGBDev, uint8_t H, uint8_t S, uint8_t V, uint16_t num)
{
    RGB_TypeDef rgb_temp;
    HSV_TypeDef hsv_temp;
    hsv_temp.H = H;
    hsv_temp.S = S;
    hsv_temp.V = V;
    rgb_temp = HSV_To_RGB(hsv_temp);
    set_rgb(p_RGBDev, rgb_temp.R, rgb_temp.G, rgb_temp.B, num);
}

/**
 * @brief   设置所有灯的颜色
 * @param   *p_RGBDev   RGB设备句柄
 * @param   R   红色
 * @param   G   绿色
 * @param   B   蓝色
 */
static void set_allrgb(RGB_DeviceTypeDef *p_RGBDev, uint8_t R, uint8_t G, uint8_t B)
{
    ((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data)->SetAllRGB((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data, R, G, B);
    p_RGBDev->Update(p_RGBDev);
}

/**
 * @brief   设置所有灯的颜色
 * @param   *p_RGBDev   RGB设备句柄
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 */
static void set_allhsv(RGB_DeviceTypeDef *p_RGBDev, uint8_t H, uint8_t S, uint8_t V)
{
    RGB_TypeDef rgb_temp;
    HSV_TypeDef hsv_temp;
    hsv_temp.H = H;
    hsv_temp.S = S;
    hsv_temp.V = V;
    rgb_temp = HSV_To_RGB(hsv_temp);
    set_allrgb(p_RGBDev, rgb_temp.R, rgb_temp.G, rgb_temp.B);
}

/**
 * @brief   更新灯的颜色
 * @param   *p_RGBDev   RGB设备句柄
 */
static void update(RGB_DeviceTypeDef *p_RGBDev)
{
    ((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data)->Update((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data);
}

/**
 * @brief   初始化RGB
 * @param   *p_RGBDev   RGB设备句柄
 */
static void init(RGB_DeviceTypeDef *p_RGBDev)
{
    ((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data)->Init((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data, WS2812B_ARR);
}

/**
 * @brief   初始化RGB
 * @param   *p_RGBDev   RGB设备句柄
 * @param   name        RGB名称
 * @param   num         灯的数量
 */
void RGB_Init(RGB_DeviceTypeDef *p_RGBDev, char *name, uint16_t num)
{
    p_RGBDev->name = name;
    p_RGBDev->num = num;
    p_RGBDev->priv_data = (WS2812B_DeviceTypeDef *)malloc(sizeof(WS2812B_DeviceTypeDef));
    p_RGBDev->priv_data =malloc(sizeof(WS2812B_DeviceTypeDef));
    if (p_RGBDev->priv_data == NULL)
    {
        return;
    }
    WS2812B_Init((WS2812B_DeviceTypeDef *)p_RGBDev->priv_data, &htim2, TIM_CHANNEL_1, "WS2812B");
    p_RGBDev->SetRGB = set_rgb;
    p_RGBDev->SetHSV = set_hsv;
    p_RGBDev->SetAllRGB = set_allrgb;
    p_RGBDev->SetAllHSV = set_allhsv;
    p_RGBDev->Update = update;
    p_RGBDev->Init = init;
    //p_RGBDev->DeInit = deinit;
}
