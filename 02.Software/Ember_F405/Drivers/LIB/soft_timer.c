/********************************************************************************
    * 文件名称 ：soft_timer.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-12-19
    * 功     能：软件定时器实现
*********************************************************************************/
#include "LIB/soft_timer.h"
#include "main.h" // HAL_GetTick 的声明
#include "BSP/Peripherals/Peripherals.h"

/* 修改定时器超时时间 */
void Start_Soft_Timer(struct soft_timer *pTimer, uint32_t timeout)
{
	pTimer->timeout = HAL_GetTick() + timeout; // 当前时间加超时值
}

void Check_KEY_Soft_Timer(void *args)
{
    KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
    if(((KEY_DeviceTypeDef *)KEY)->DeBounce_timer.timeout <= HAL_GetTick())
    {
        // 调用回调函数
        if(KEY->DeBounce_timer.func != NULL)
        {
        KEY->DeBounce_timer.func(KEY->DeBounce_timer.args); // 进入定时器回调函数
        }
    }
    if(((KEY_DeviceTypeDef *)KEY)->Mode_timer.timeout <= HAL_GetTick())
    {
        // 调用回调函数
        if(KEY->Mode_timer.func != NULL)
        {
        KEY->Mode_timer.func(KEY->Mode_timer.args); // 进入定时器回调函数
        }
    }
}

/* 检查定时器是否超时 */
void Check_LED_Soft_Timer(void *args)
{
    LED_DeviceTypeDef *LED = (LED_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
    if(((LED_DeviceTypeDef *)LED)->timer.timeout <= HAL_GetTick())
    {
        // 调用回调函数
        if(LED->timer.func != NULL)
        {
        LED->timer.func(LED->timer.args); // 进入定时器回调函数
        }
    }
}
