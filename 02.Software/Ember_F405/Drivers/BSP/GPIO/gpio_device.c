/********************************************************************************
    * 文件名称 ：gpio_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：GPIO设备
*********************************************************************************
V1.0 2025-01-13 First version
*********************************************************************************/
#include "BSP/GPIO/gpio_device.h"

#ifdef USE_HAL
/**
 * @brief    读取GPIO状态
 *
 * @param   p_GPIODev	GPIO设备句柄
 *
 * @return  uint8_t
 */
static uint8_t Read(GPIO_DeviceTypeDef *p_GPIODev)
{
    return HAL_GPIO_ReadPin(p_GPIODev->instance, p_GPIODev->pin);
}

/**
 * @brief    设置GPIO状态
 *
 * @param   p_GPIODev	GPIO设备句柄
 * @param   PinState	状态
 *
 * @return  void
 */
static void Set(GPIO_DeviceTypeDef *p_GPIODev, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(p_GPIODev->instance, p_GPIODev->pin, PinState);
}

/**
 * @brief    初始化GPIO
 *
 * @param   p_GPIODev	GPIO设备句柄
 *
 * @return  uint8_t
 */
static uint8_t Init(GPIO_DeviceTypeDef *p_GPIODev)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = p_GPIODev->pin;
    switch (p_GPIODev->Mode)
    {
    case GPIO_INPUT:
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        break;
    case GPIO_OUTPUT_PP:
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        break;
    case GPIO_OUTPUT_OD:
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        break;
    default:
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        break;
    }
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(p_GPIODev->instance, &GPIO_InitStruct);
    return 0;
}
#endif  //USE_HAL

/**
 * @brief    初始化GPIO
 *
 * @param   p_GPIODev	GPIO设备句柄
 * @param   name	GPIO名称
 * @param   Instance	引脚基地址
 * @param   pin	引脚号
 *
 * @return  int8_t
 */
int8_t GPIO_Init(GPIO_DeviceTypeDef *p_GPIODev,char *name ,void *Instance ,uint16_t pin)
{
    if(p_GPIODev == NULL)
        return 1; //参数错误
    p_GPIODev->name = name;
    p_GPIODev->Read = Read;
    p_GPIODev->instance = Instance;
    p_GPIODev->pin = pin;
    p_GPIODev->Set = Set;
    p_GPIODev->Init = Init;
    // p_GPIODev->DeInit = DeInit;
    return 0;
}
