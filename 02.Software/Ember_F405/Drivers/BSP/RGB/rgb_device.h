/********************************************************************************
    * 文件名称 ：rgb_device.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：RGB驱动
*********************************************************************************/
#ifndef __RGB_DEVICE_H
#define __RGB_DEVICE_H

#include <stdint.h>
#include <stdlib.h>
#include "BSP/RGB/ws2812.h"

/**
 * @brief   RGB颜色结构体
 * @param   R   红色
 * @param   G   绿色
 * @param   B   蓝色
 */
typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_TypeDef;//RGB结构体

/**
 * @brief   HSV颜色结构体
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 */
typedef struct{
    uint8_t H;
    uint8_t S;
    uint8_t V;
}HSV_TypeDef;//HSV结构体

/**
 * @brief   RGB设备结构体
 * @param   name    设备名称
 * @param   num     灯珠数量
 * @param   SetRGB  设置单个灯珠的RGB值
 * @param   SetHSV  设置单个灯珠的HSV值
 * @param   SetAllRGB   设置所有灯珠的RGB值
 * @param   SetAllHSV   设置所有灯珠的HSV值
 * @param   Update  发送灯珠颜色数据
 * @param   Init    初始化
 * @param   DeInit  反初始化
 * @param   priv_data   私有数据
 */
typedef struct RGB_Device{
    char *name;
    uint16_t num;
    void (*SetRGB)(struct RGB_Device *rgb, uint8_t R, uint8_t G, uint8_t B,uint16_t num);//设置单个灯珠的RGB值
    void (*SetHSV)(struct RGB_Device *rgb, uint8_t H, uint8_t S, uint8_t V,uint16_t num);//设置单个灯珠的HSV值
    void (*SetAllRGB)(struct RGB_Device *rgb, uint8_t R, uint8_t G, uint8_t B);//设置所有灯珠的RGB值
    void (*SetAllHSV)(struct RGB_Device *rgb, uint8_t H, uint8_t S, uint8_t V);//设置所有灯珠的HSV值
    void (*Update)(struct RGB_Device *rgb);//发送灯珠颜色数据
    void (*Init)(struct RGB_Device *rgb);//初始化
    void (*DeInit)(struct RGB_Device *rgb);
    void *priv_data;//私有数据 存放WS2812B_DeviceTypeDef结构体句柄
}RGB_DeviceTypeDef;

RGB_TypeDef HSV_To_RGB(HSV_TypeDef hsv);//HSV转RGB

void RGB_Init(RGB_DeviceTypeDef *rgb, char *name , uint16_t num);//初始化

#endif//__RGB_DEVICE_H




