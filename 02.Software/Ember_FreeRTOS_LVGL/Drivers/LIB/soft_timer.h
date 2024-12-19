/********************************************************************************
    * 文件名称 ：circle_buffer.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：环形缓冲区实现
*********************************************************************************/
#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <stdint.h>

/* 软件定时器结构体 */
typedef struct soft_timer {
	uint32_t timeout;            // 超时时间
	void *args;                  // 回调参数
	void (*func)(void *);        // 超时回调函数
} soft_timer;

/* 修改定时器超时时间 */
void Start_Soft_Timer(struct soft_timer *pTimer, uint32_t timeout);

/* 检查所有定时器状态 */
void Check_Soft_Timer(void);

#endif // SOFT_TIMER_H
