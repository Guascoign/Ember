/********************************************************************************
    * 文件名称 ：soft_timer.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：环形缓冲区实现
*********************************************************************************/
#include "soft_timer.h"
#include "main.h" // HAL_GetTick 的声明

/* 修改定时器超时时间 */
void Start_Soft_Timer(struct soft_timer *pTimer, uint32_t timeout)
{
	pTimer->timeout = HAL_GetTick() + timeout; // 当前时间加超时值
}

/* 检查定时器是否超时 */
void Check_Soft_Timer(void)
{
	extern struct soft_timer key_timer; // 声明外部定时器
	if (key_timer.timeout <= HAL_GetTick()) // 如果定时器超时
	{
		key_timer.func(key_timer.args); // 调用超时回调函数
	}
}
