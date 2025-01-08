/********************************************************************************
    * 文件名称 ：adc_pack.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：ADC封装
*********************************************************************************
V1.0 2024-01-30 
*********************************************************************************/

#include "BSP/ADC/adc_pack.h"

#ifdef USE_HAL
static int Read(struct ADC_Conf *p_ADCDev, uint8_t channel){
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel((ADC_HandleTypeDef *)p_ADCDev->Instance, &sConfig) != HAL_OK)
    {
        return 1; //配置通道失败
    }
    if (HAL_ADC_Start((ADC_HandleTypeDef *)p_ADCDev->Instance) != HAL_OK)
    {
        return 2; //启动失败
    }
    if (HAL_ADC_PollForConversion((ADC_HandleTypeDef *)p_ADCDev->Instance, HAL_MAX_DELAY) != HAL_OK)
    {
        return 3; //转换失败
    }
    return HAL_ADC_GetValue((ADC_HandleTypeDef *)p_ADCDev->Instance); //读取成功
}

static void Init(void){
    MX_ADC1_Init();
}
#endif  //USE_HAL

uint8_t ADC_Init(ADC_ConfTypeDef *p_ADCDev, char *name, void *Instance){
    p_ADCDev->name = name;
    p_ADCDev->Instance = Instance;
    p_ADCDev->Init = Init;
    p_ADCDev->Read = Read;
    p_ADCDev->priv_data = Instance;
    return 0;
}
