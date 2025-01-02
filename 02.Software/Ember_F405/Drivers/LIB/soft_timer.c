/********************************************************************************
    * 文件名称 ：soft_timer.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-12-19
    * 功     能：软件定时器实现
*********************************************************************************/
#include "soft_timer.h"
#include "main.h" // HAL_GetTick 的声明

/* 修改定时器超时时间 */
void Start_Soft_Timer(struct soft_timer *pTimer, uint32_t timeout)
{
	pTimer->timeout = HAL_GetTick() + timeout; // 当前时间加超时值
}

extern key_t key1;
/* 检查定时器是否超时 */
void Check_Soft_Timer(void *args)
{
	
    key_t *key = (key_t *)args;  // 将 void * 转换为 key_t 指针

    // 检查定时器是否超时
    if (key->debounce_timer.timeout <= HAL_GetTick()) {
        // 调用回调函数
        if (key->debounce_timer.func) {
            key->debounce_timer.func(key->debounce_timer.args);	//进入定时器回调函数
					key->debounce_timer.timeout = ~0;	//复位软件定时器超时时间
        }
    }
}
