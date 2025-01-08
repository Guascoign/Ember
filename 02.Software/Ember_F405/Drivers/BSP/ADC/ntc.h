/********************************************************************************
    * 文件名称 ：ntc103at.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-08
    * 功     能：NTC热敏电阻ADC转换温度
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL

*********************************************************************************/

#ifndef __NTC103AT_H
#define __NTC103AT_H

#include <stdint.h>
#include "Peripherals/Peripherals.h"
#include "stm32f1xx_hal.h"

#define NTC_10K_R 10//分压电阻阻值 单位K
#define ADC_MAX_VALUE 4096//ADC最大值
#define ADC_REF_VOLTAGE 3.3//ADC参考电压

typedef struct{
   int16_t    temperature;//单位：°C
   float  resistance;//单位：Ω
}NTC_10KTYPE;

#define NTC_LIST_MAX_SIZE   (sizeof(ntc_10k_table) / sizeof(ntc_10k_table[0]))/*表的长度*/


// NTC 10K 温度-电阻表格
static const NTC_10KTYPE ntc_10k_table[] = {
    {-40, 235.8307559}, {-39, 221.6724098}, {-38, 208.473826}, {-37, 196.1630569}, {-36, 184.6740349},
    {-35, 173.9460536}, {-34, 163.9232991}, {-33, 154.5544238}, {-32, 145.7921607}, {-31, 137.5929735},
    {-30, 129.9167384}, {-29, 122.7264551}, {-28, 115.9879839}, {-27, 109.6698071}, {-26, 103.7428109},
    {-25, 98.18008736}, {-24, 92.95675344}, {-23, 88.04978631}, {-22, 83.43787284}, {-21, 79.1012721},
    {-20, 75.0216899}, {-19, 71.18216392}, {-18, 67.56695872}, {-17, 64.16146957}, {-16, 60.95213444},
    {-15, 57.92635333}, {-14, 55.07241424}, {-13, 52.37942535}, {-12, 49.83725271}, {-11, 47.43646304},
    {-10, 45.16827118}, {-9, 43.02449173}, {-8, 40.99749462}, {-7, 39.08016422}, {-6, 37.26586165},
    {-5, 35.54839008}, {-4, 33.92196277}, {-3, 32.38117357}, {-2, 30.92096971}, {-1, 29.53662669},
    {0, 28.22372509}, {1, 26.97812912}, {2, 25.79596688}, {3, 24.67361196}, {4, 23.60766657},
    {5, 22.59494578}, {6, 21.63246309}, {7, 20.71741687}, {8, 19.84717797}, {9, 19.01927811},
    {10, 18.23139919}, {11, 17.48136327}, {12, 16.76712341}, {13, 16.08675502}, {14, 15.4384479},
    {15, 14.82049884}, {16, 14.23130468}, {17, 13.66935597}, {18, 13.1332309}, {19, 12.62158981},
    {20, 12.13317001}, {21, 11.66678088}, {22, 11.22129948}, {23, 10.79566624}, {24, 10.38888114},
    {25, 10}, {26, 9.628131096}, {27, 9.272431959}, {28, 8.932106406}, {29, 8.606401759},
    {30, 8.294606244}, {31, 7.996046556}, {32, 7.710085586}, {33, 7.436120291}, {34, 7.173579697},
    {35, 6.921923035}, {36, 6.680637987}, {37, 6.449239052}, {38, 6.227265999}, {39, 6.014282436},
    {40, 5.809874448}, {41, 5.613649332}, {42, 5.425234404}, {43, 5.244275879}, {44, 5.070437823},
    {45, 4.90340116}, {46, 4.742862746}, {47, 4.588534498}, {48, 4.440142569}, {49, 4.297426576},
    {50, 4.160138877}, {51, 4.028043881}, {52, 3.900917407}, {53, 3.778546077}, {54, 3.660726742},
    {55, 3.547265944}, {56, 3.437979407}, {57, 3.332691561}, {58, 3.231235085}, {59, 3.133450484},
    {60, 3.039185685}, {61, 2.948295658}, {62, 2.860642056}, {63, 2.776092875}, {64, 2.694522137},
    {65, 2.615809586}, {66, 2.539840398}, {67, 2.466504917}, {68, 2.395698395}, {69, 2.327320749},
    {70, 2.261276335}, {71, 2.19747373}, {72, 2.135825526}, {73, 2.076248134}, {74, 2.018661605},
    {75, 1.962989451}, {76, 1.909158479}, {77, 1.857098639}, {78, 1.80674287}, {79, 1.758026963},
    {80, 1.710889424}, {81, 1.665271351}, {82, 1.62111631}, {83, 1.578370221}, {84, 1.536981253},
    {85, 1.496899717}, {86, 1.458077968}, {87, 1.420470316}, {88, 1.384032933}, {89, 1.348723772},
    {90, 1.314502486}, {91, 1.281330351}, {92, 1.249170196}, {93, 1.217986331}, {94, 1.187744486},
    {95, 1.158411744}, {96, 1.129956484}, {97, 1.102348327}, {98, 1.075558075}, {99, 1.049557669},
    {100, 1.024320132}, {101, 0.999819529}, {102, 0.976030918}, {103, 0.952930309}, {104, 0.930494626},
    {105, 0.908701665}, {106, 0.88753006}, {107, 0.866959246}, {108, 0.846969428}, {109, 0.827541548},
    {110, 0.808657252}, {111, 0.790298866}, {112, 0.772449364}, {113, 0.755092345}, {114, 0.738212003},
    {115, 0.721793108}, {116, 0.70582098}, {117, 0.690281467}, {118, 0.675160924}, {119, 0.660446194},
    {120, 0.646124587}, {121, 0.632183863}, {122, 0.618612213}, {123, 0.605398243}, {124, 0.592530958},
    {125, 0.579999745}, {126, 0.56779436}, {127, 0.555904913}
};

/**
 * @brief   ADC分辨率
 * @note  该枚举类型定义了ADC的分辨率，包括12位、10位、8位和6位
 * @param  RESOLUTION_12B 12位分辨率 4096个电平
 * @param  RESOLUTION_10B 10位分辨率 1024个电平
 * @param  RESOLUTION_8B 8位分辨率 256个电平
 * @param  RESOLUTION_6B 6位分辨率 64个电平
 */
typedef enum ADC_Resolution{
    RESOLUTION_12B = 0x00U,
    RESOLUTION_10B = 0x01U,
    RESOLUTION_8B  = 0x02U,
    RESOLUTION_6B  = 0x03U
} ADC_ResolutionTypeDef;

typedef enum ADC_ScanConfig{
    SCAN_DISABLE = 0x00U,
    SCAN_ENABLE  = 0x01U
} ADC_ScanConfigTypeDef;

typedef enum ADC_ContinuousConvMode{
    CONTINUOUS_DISABLE = 0x00U,
    CONTINUOUS_ENABLE  = 0x01U
} ADC_ContinuousConvModeTypeDef;

typedef enum ADC_DiscontinuousConvMode{
    DISCONTINUOUS_DISABLE = 0x00U,
    DISCONTINUOUS_ENABLE  = 0x01U
} ADC_DiscontinuousConvModeTypeDef;

typedef enum ADC_dataAlign{
    DATAALIGN_RIGHT = 0x00U,
    DATAALIGN_LEFT  = 0x01U
} ADC_DataAlignTypeDef;


/**
 * @brief   ADC采样时间
 * @note  该枚举类型定义了ADC的采样时间，包括1.5个采样周期、7.5个采样周期、13.5个采样周期、28.5个采样周期、41.5个采样周期、55.5个采样周期、71.5个采样周期和239.5个采样周期
 * @param  SAMPLETIME_1_5 1.5个采样周期
 * @param  SAMPLETIME_7_5 7.5个采样周期
 * @param  SAMPLETIME_13_5 13.5个采样周期
 * @param  SAMPLETIME_28_5 28.5个采样周期
 * @param  SAMPLETIME_41_5 41.5个采样周期
 * @param  SAMPLETIME_55_5 55.5个采样周期
 * @param  SAMPLETIME_71_5 71.5个采样周期
 * @param  SAMPLETIME_239_5 239.5个采样周期
 */
typedef enum ADC_SampleTime{
    SAMPLETIME_1_5   = 0x00U,
    SAMPLETIME_7_5   = 0x01U,
    SAMPLETIME_13_5  = 0x02U,
    SAMPLETIME_28_5  = 0x03U,
    SAMPLETIME_41_5  = 0x04U,
    SAMPLETIME_55_5  = 0x05U,
    SAMPLETIME_71_5  = 0x06U,
    SAMPLETIME_239_5 = 0x07U
} ADC_SampleTimeTypeDef;

/**
 * @brief   NTC设备结构体
 * @note  该结构体中包含了NTC设备的基本信息和操作函数
 * @param  name 设备名称
 * @param  Instance ADC外设实例基地址
 * @param  channel ADC通道
 * @param  Init 设备初始化函数
 * @param  get_temperature 设备获取温度函数
 * @param  get_temperature_filtered 设备获取滤波后的温度函数
 * @param  get_adc_value 设备获取ADC采样值函数
 * @param  priv_data 私有数据，存储ADC_HandleTypeDef结构体指针
 */
typedef struct NTC_Device{
    char *name;
    void *Instance;
    uint8_t channel;
    int (*Init)(struct NTC_Device *p_NTCDev, ADC_ResolutionTypeDef resolution, ADC_SampleTimeTypeDef sampleTime); // 初始化
    float (*get_temperature)(struct NTC_Device *p_NTCDev);       // 获取温度
    float (*get_temperature_filtered)(struct NTC_Device *p_NTCDev);       // 获取滤波后的温度
    int16_t (*get_adc_value)(struct NTC_Device *p_NTCDev);    // 获取ADC采样值
    void *priv_data;//私有数据 存入ADC_HandleTypeDef结构体指针
} NTC_DeviceTypeDef;

/**
 * @brief   初始化NTC设备
 * @param   p_NTCDev    NTC设备结构体指针
 * @param   name        设备名称
 * @param   Instance    ADC外设实例基地址
 * @param   channel     ADC通道
 * @param   resolution  ADC分辨率
 * @param   sampleTime  ADC采样时间
 * @return  返回值为0表示初始化成功，其他值表示初始化失败
 */
int8_t NTC_Init(
    NTC_DeviceTypeDef *p_NTCDev,
    char *name,
    void *Instance, 
    uint8_t channel,
    ADC_ResolutionTypeDef resolution,
    ADC_SampleTimeTypeDef sampleTime);

float lookup_temperature(uint16_t ADC_Value);

#endif  // __NTC103AT_H

