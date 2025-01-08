/********************************************************************************
    * 文件名称 ：adc_pack.h
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：ADC封装
*********************************************************************************
V1.0 2024-01-30 
*********************************************************************************/
/*使用说明*************************************************************************

*********************************************************************************/
#ifndef __ADC_PACK_H
#define __ADC_PACK_H

#include <stdint.h>
#include "string.h"
#include <stdlib.h>
#include "BSP/Peripherals/Peripherals.h"

#define USE_HAL //使用STM32 HAL库
// #define USE_REG //使用寄存器库
// #define USE_LL  //使用LL库

#ifdef USE_HAL
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "adc.h"
#endif  //USE_HAL

// typedef enum ADC_SAMPLETIME{
//     SAMPLETIME_3CYCLES = 0X00U, //3个采样周期
//     SAMPLETIME_15CYCLES = 0X01U, //15个采样周期
//     SAMPLETIME_28CYCLES = 0X02U, //28个采样周期
//     SAMPLETIME_56CYCLES = 0X03U, //56个采样周期
//     SAMPLETIME_84CYCLES = 0X04U, //84个采样周期
//     SAMPLETIME_112CYCLES = 0X05U, //112个采样周期
//     SAMPLETIME_144CYCLES = 0X06U, //144个采样周期
//     SAMPLETIME_480CYCLES = 0X07U //480个采样周期
// } ADC_SAMPLETIMETypeDef;

typedef struct ADC_Conf{
    char *name; //设备名称
    void *Instance; //设备基地址
    uint8_t channel; //通道
    void (*Init)(void);//初始化 调用 MX_ADC1_Init();
    uint16_t (*Read)(struct ADC_Device *p_ADCDev);//读取通道电压
    void *priv_data;//私有数据
} ADC_ConfTypeDef;

uint8_t ADC_Init(
    ADC_ConfTypeDef *p_ADCDev,
    char *name, 
    void *Instance);

uint32_t ADC_Read(ADC_ConfTypeDef *p_ADCDev, uint8_t channel);

#endif//__ADC_PACK_H

