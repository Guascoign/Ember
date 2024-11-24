/********************************************************************************
    * 文件名称 ：tim_base.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-01-30
    * 功     能：定时处理功能
*********************************************************************************/
#ifndef _TIME_BASE_H_
#define _TIME_BASE_H_

#include <stdio.h>
#include "tim.h"
#include "main.h"

#define BASE_TIMER htim2
#define MAIN_FREQ 100000000  // 主频为 100 MHz
#define TARGET_FREQ 1000     // 目标频率为 1 kHz

extern unsigned int Prescaler;
extern unsigned int Counter_period;

// 定义一个结构体来表示时间
typedef struct {
    uint32_t milliseconds; // 毫秒
    uint32_t seconds;      // 秒
    uint32_t minutes;      // 分
    uint32_t hours;        // 小时
		uint32_t days;
} RunTime;

extern RunTime runtime;

typedef enum
{
    BASE_1MS = 0u,
    BASE_10MS,
    BASE_20MS,
    BASE_50MS,
    BASE_100MS,
    BASE_200MS,
    BASE_500MS,
    BASE_1000MS,
    BASE_NUM,
}time_base_e;

uint8_t time_base_get_flag(time_base_e base);
void time_base_set_flag(time_base_e base);
void time_base_clear_flag(time_base_e base);
void time_base_setup(void);
void time_base_update(void);
int calculate_dividers(unsigned int target_freq, unsigned int *prescaler, unsigned int *counter_period);
void update_run_time_ms(RunTime *runtime); 
void update_run_time_s(RunTime *runtime); 
#endif // !_TIME_BASE_H_
