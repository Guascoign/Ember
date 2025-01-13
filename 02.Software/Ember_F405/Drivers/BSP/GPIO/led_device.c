/********************************************************************************
    * 文件名称 ：led_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：led设备
*********************************************************************************
V1.0 2025-01-13 First version
*********************************************************************************/
#include "BSP/GPIO/led_device.h"


// static uint8_t LED_Set(LED_DeviceTypeDef *p_LEDDev, LED_BlinkType blink ,LED_BlinkMode mode)
// {
//     if(p_LEDDev == NULL)
//         return 1; //参数错误
//     p_LEDDev->blink.blink = blink;
//     p_LEDDev->blink.mode = mode;
//     p_LEDDev->blink.flag = 0;
//     p_LEDDev->timer.timeout = ~0;
//     p_LEDDev->timer.func = LED_Callback;
//     p_LEDDev->timer.args = p_LEDDev;
//     p_LEDDev->priv_data = ; 
//     switch (blink)
//     {
//     case LED_Blink_Single:{
//         Start_Soft_Timer(&p_LEDDev->timer, 500);
//         break;
//     }
//     case LED_Blink_Double:{
//         Start_Soft_Timer(&p_LEDDev->timer, 500);
//         break;
//     }
//     case LED_Blink_Three:{
//         Start_Soft_Timer(&p_LEDDev->timer, 500);
//         break;
//     }
//     case LED_ON:{
//         p_LEDDev->Set(p_LEDDev, PIN_SET);
//         break;
//     }
//     case LED_OFF:{
//         p_LEDDev->
//         break;
//     }
//     default:
//         p_LEDDev->Set(p_LEDDev, PIN_RESET); //默认常灭
//     return 0;
// }

// void LED_Callback()
// {
    

// }
int8_t LED_Init(LED_DeviceTypeDef *p_LEDDev,char *name,void *Instance ,uint16_t pin)
{
    if(p_LEDDev == NULL)
        return 1; //参数错误
    p_LEDDev->name = name;
    //Instance 与 pin 存入GPIO_DeviceTypeDef结构体的priv_data
    p_LEDDev->priv_data = malloc(sizeof(GPIO_DeviceTypeDef));
    if(p_LEDDev->priv_data == NULL)
        return 1; //内存分配失败
    //初始化GPIO
    ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->instance = Instance;
    ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->pin = pin;
    ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Mode = GPIO_OUTPUT_PP;
    GPIO_Init((GPIO_DeviceTypeDef *)p_LEDDev->priv_data, name, (void *)Instance, pin);

    //p_LEDDev->Set = LED_Set;
   // p_LEDDev->timer.func = LED_Callback;
    p_LEDDev->timer.args = p_LEDDev;
    p_LEDDev->timer.timeout = ~0;
    Add_Device(name, p_LEDDev, LED_DEVICE);
    return 0;
}
