/********************************************************************************
    * 文件名称 ：led_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：led设备
*********************************************************************************
V1.0 2025-01-13 First version
*********************************************************************************
Check_LED_Soft_Timer( (void *)&RUNLED );//放入1ms定时器中断
*********************************************************************************/
#include "BSP/GPIO/led_device.h"

static uint8_t LED_Set(LED_DeviceTypeDef *p_LEDDev, LED_BlinkType blink ,LED_BlinkMode mode)
{
    if(p_LEDDev == NULL)
        return 1; //参数错误
    p_LEDDev->blink.blink = blink;
    p_LEDDev->blink.mode = mode;
    switch (blink)
    {
    case LED_Blink_Single:{
        Start_Soft_Timer(&p_LEDDev->timer, (p_LEDDev->blink.delay) * 4);
        p_LEDDev->blink.flag = 2;
        break;
    }
    case LED_Blink_Double:{
        Start_Soft_Timer(&p_LEDDev->timer, (p_LEDDev->blink.delay) * 6);
        p_LEDDev->blink.flag = 4;
        break;
    }
    case LED_Blink_Three:{
        Start_Soft_Timer(&p_LEDDev->timer, (p_LEDDev->blink.delay) * 8);
        p_LEDDev->blink.flag = 6;
        break;
    }
    case LED_ON:{
        ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Set(p_LEDDev->priv_data, GPIO_PIN_SET); //常亮
        break;
    }
    case LED_OFF:{
        ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Set(p_LEDDev->priv_data, GPIO_PIN_RESET); //常亮
        break;
    }
    default:
        ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Set(p_LEDDev->priv_data, GPIO_PIN_SET); //常亮
    return 0;
    }
    return 0;
}

void LED_Callback(void *args)
{
	LED_DeviceTypeDef *p_LEDDev = (LED_DeviceTypeDef *)args;
    if(p_LEDDev->blink.flag == 0)
    {
        if(p_LEDDev->blink.mode == continue_Blink)
        {
            p_LEDDev->Set(p_LEDDev, p_LEDDev->blink.blink, p_LEDDev->blink.mode);//再次传入参数
        }
        else
        {
            p_LEDDev->Set(p_LEDDev, LED_OFF , p_LEDDev->blink.mode);//再次传入参数
            return;
        }
        return;
    }
    else
    {
        p_LEDDev->blink.flag--;
        if(p_LEDDev->blink.flag % 2 == 0)
        {
            ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Set(p_LEDDev->priv_data, GPIO_PIN_RESET);
        }
        else
        {
            ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->Set(p_LEDDev->priv_data, GPIO_PIN_SET);
        }
			Start_Soft_Timer(&p_LEDDev->timer, p_LEDDev->blink.delay);
    }
    
}

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
    ((GPIO_DeviceTypeDef *)p_LEDDev->priv_data)->name = name;
    GPIO_Init((GPIO_DeviceTypeDef *)p_LEDDev->priv_data,name,Instance,pin);
    //初始化定时器
    p_LEDDev->timer.func = LED_Callback;
    p_LEDDev->timer.args = p_LEDDev;
    p_LEDDev->timer.timeout = ~0;
    //初始化LED
    p_LEDDev->Set = LED_Set;
    p_LEDDev->Set(p_LEDDev, LED_OFF, single_Blink);
    //设置延时
    p_LEDDev->blink.delay = LED_Delay_100ms;
    return 0;
}
