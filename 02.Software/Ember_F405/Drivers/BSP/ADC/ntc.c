/********************************************************************************
    * 文件名称 ：ntc103at.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-08
    * 功     能：NTC热敏电阻ADC转换温度
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL

*********************************************************************************/
#include "ntc103at.h"

/**
 * @brief   查找温度
 *
 * @param   ADC_Value  ADC采样值
 * @return  温度值（摄氏度）
 */
float lookup_temperature(uint16_t ADC_Value) {
    // 计算电阻值
    float voltage = ADC_Value * ADC_REF_VOLTAGE / ADC_MAX_VALUE;
    float resistance = NTC_10K_R * voltage / (ADC_REF_VOLTAGE - voltage);

    // 查找温度
    for (int i = 0; i < sizeof(ntc_10k_table) / sizeof(ntc_10k_table[0]); ++i) {
        if (resistance >= ntc_10k_table[i].resistance) {
            if (i == 0) {
                return ntc_10k_table[i].temperature;
            }
            // 进行线性插值
            float temp_range = ntc_10k_table[i - 1].temperature - ntc_10k_table[i].temperature;
            float res_range = ntc_10k_table[i - 1].resistance - ntc_10k_table[i].resistance;
            float temp_diff = resistance - ntc_10k_table[i].resistance;
            float temp_interpolated = ntc_10k_table[i].temperature + (temp_diff / res_range) * temp_range;
            return temp_interpolated;
        }
    }
    // 如果电阻值高于表格中的最高值，则返回一个错误值
    return -255.0;
}

/**
 * @brief   获取ADC采样值
 *
 * @param   p_NTCDev    NTC设备结构体指针
 * @return  ADC采样值
 */
static int16_t get_adc_value(struct NTC_Device *p_NTCDev) {
    HAL_ADC_Start(p_NTCDev->priv_data);
    HAL_ADC_PollForConversion(p_NTCDev->priv_data, HAL_MAX_DELAY);
    int16_t adc_value = HAL_ADC_GetValue(p_NTCDev->priv_data);
    HAL_ADC_Stop(p_NTCDev->priv_data);
    return adc_value;
}

/**
 * @brief   获取温度值
 *
 * @param   p_NTCDev    NTC设备结构体指针
 * @return  温度值（摄氏度）
 */
static float get_temperature(struct NTC_Device *p_NTCDev) {
    int16_t adc_value = p_NTCDev->get_adc_value(p_NTCDev);
    return lookup_temperature(adc_value);
}

/**
 * @brief   初始化ADC
 *
 * @param   p_NTCDev    NTC设备结构体指针
 * @param   resolution  ADC分辨率
 * @param   sampleTime  ADC采样时间
 * @return  返回值为0表示初始化成功，其他值表示初始化失败
 */
static int Init(struct NTC_Device *p_NTCDev, void *Instance, ADC_ResolutionTypeDef resolution, ADC_SampleTimeTypeDef sampleTime) {
    if (p_NTCDev == NULL || Instance == NULL) {
        return 2; // 参数错误
    }

    if(p_NTCDev->priv_data == NULL){
        p_NTCDev->priv_data = (void *)malloc(sizeof(ADC_HandleTypeDef));
        if(p_NTCDev->priv_data == NULL)
            return 3; // 内存分配失败
        memset(p_NTCDev->priv_data, 0, sizeof(ADC_HandleTypeDef));
    }

    ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Instance = (ADC_HandleTypeDef *)Instance;
    switch (resolution)
{
        case RESOLUTION_12B:
            ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Init.Resolution = ADC_RESOLUTION_12B;
            break;
        case RESOLUTION_10B:
            ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Init.Resolution = ADC_RESOLUTION_10B;
            break;
        case RESOLUTION_8B:
            ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Init.Resolution = ADC_RESOLUTION_8B;
            break;
        case RESOLUTION_6B:
            ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Init.Resolution = ADC_RESOLUTION_6B;
            break;
        default:
            ((ADC_HandleTypeDef *)(p_NTCDev->priv_data))->Init.Resolution = ADC_RESOLUTION_12B;
        break;
    }

    
    if (HAL_ADC_Init(p_NTCDev->priv_data) != HAL_OK) {
        return 1; // 初始化失败
    }

    // sConfig.Channel = p_NTCDev->channel;
    // sConfig.Rank = ADC_REGULAR_RANK_1;
    // sConfig.SamplingTime = sampleTime;

    // if (HAL_ADC_ConfigChannel(p_NTCDev->priv_data, &sConfig) != HAL_OK) {
    //     return 1; // 配置通道失败
    // }

    return 0; // 初始化成功
}

/**
 * @brief   初始化NTC设备
 *
 * @param   p_NTCDev    NTC设备结构体指针
 * @param   name        设备名称
 * @param   ADCx        ADC外设实例基地址
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
    ADC_SampleTimeTypeDef sampleTime) {
    if (p_NTCDev == NULL) {
        return 1; // 参数错误
    }
    p_NTCDev->name = name;
    p_NTCDev->channel = channel;
    p_NTCDev->Init = Init;
    p_NTCDev->get_adc_value = get_adc_value;
    p_NTCDev->get_temperature = get_temperature;
    p_NTCDev->get_temperature_filtered = NULL; // 可以根据需要实现滤波功能

    if (p_NTCDev->Init(p_NTCDev, resolution, sampleTime) != 0) {
        return 2; // 初始化失败
    }

    Add_Device(name, p_NTCDev, NTC_DEVICE); // 添加到外设列表
    return 0; // 初始化成功
}
