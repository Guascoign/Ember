/********************************************************************************
    * 文件名称 ：soft_timer.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-12-19
    * 功     能：软件定时器实现
*********************************************************************************/
#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <stdint.h>

/**
 * @brief    软件定时器结构体
 * @param    timeout	超时时间
 * @param    args		回调参数
 * @param    func		超时回调函数
 * @param    Disable_Refresh	禁用刷新
 */
typedef struct Soft_timer {
	uint32_t timeout;            // 超时时间
	void *args;                  // 回调参数
	void (*func)(void *);        // 超时回调函数
    uint8_t Disable_Refresh;      // 使能刷新
}Soft_TimerTypeDef, *p_Soft_TimerTypeDef;

/* 修改定时器超时时间 */
void Start_Soft_Timer(Soft_TimerTypeDef *p_Timer, uint32_t timeout);

/* 检查所有定时器状态 */
void Check_LED_Soft_Timer(void *args);
void Check_KEY_Soft_Timer(void *args);

#endif // SOFT_TIMER_H
