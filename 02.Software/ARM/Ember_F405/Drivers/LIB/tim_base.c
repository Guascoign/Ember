/********************************************************************************
    * 文件名称 ：tim_base.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-01-30
    * 功     能：定时处理功能
*********************************************************************************/
#include "tim_base.h"

static volatile uint32_t base_counter = 0;
static volatile uint8_t base_time_flag = 0;
static const uint32_t base_time_div[(uint8_t)BASE_NUM] = {1, 10, 20, 50, 100, 200, 500, 1000}; 
static uint8_t base_index = 0;

/**
 * @brief   设置定时器基准
 *
 * @return  void
 */
void time_base_setup(void)
{
    HAL_TIM_Base_Start_IT(&BASE_TIMER);
}

/**
 * @brief   获取定时器基准标志
 *
 * @param   base    定时器基准
 *
 * @return  uint8_t 定时器基准标志
 */
uint8_t time_base_get_flag(time_base_e base)
{
    return (base_time_flag & (1u << (uint8_t)base));
} 

/**
 * @brief   设置定时器基准标志
 *
 * @param   base    定时器基准
 *
 * @return  void
 */
void time_base_set_flag(time_base_e base)
{
    base_time_flag |= (1u << (uint8_t)base);
} 

/**
 * @brief   清除定时器基准标志
 *
 * @param   base    定时器基准
 *
 * @return  void
 */
void time_base_clear_flag(time_base_e base)
{
    uint8_t tmp = 1u << (uint8_t)base;
    tmp = ~tmp;
    base_time_flag &= tmp;
} 

/**
 * @brief   更新定时器基准
 *
 * @return  void
 */
void time_base_update(void)
{
    base_counter++;

    for (base_index = 0; base_index < (uint8_t)BASE_NUM; base_index++)
    {
        if (base_counter % base_time_div[base_index] == 0)
        {
            time_base_set_flag((time_base_e)base_index);

            if (base_index == (uint8_t)BASE_10MS)
            {
          //      button_process();
            }
        }
    }
}

/**
 * @brief   计算定时器的预分频器和计数周期
 *
 * @param   target_freq     目标频率（Hz）
 * @param   Prescaler       输出参数，预分频器值
 * @param   Counter_period  输出参数，计数周期值
 *
 * @return  int             返回值，0 表示成功，-1 表示失败
 */
int calculate_dividers(unsigned int target_freq, unsigned int *Prescaler, unsigned int *Counter_period) 
{
    unsigned int prescaler_value = 0;
    unsigned int divide_factor = 1;

    // 首先计算预分频系数
    while ((MAIN_FREQ / divide_factor) > target_freq * 65535) {
        divide_factor++;
        if (divide_factor > 65535) {
	#ifdef LOG_OUTPUT
						printf("[ERROR] Can't Find Prescaler & Counter_period\n");
	#endif
            return -1; // 错误：无法找到合适的预分频系数
        }
    }
    prescaler_value = divide_factor - 1;
		
    // 计算分频系数
    *Prescaler = prescaler_value;
    *Counter_period = (MAIN_FREQ / (prescaler_value + 1)) / target_freq;
	#ifdef LOG_OUTPUT
		printf("[info]\nMAIN_FREQ: %.2f MHz\nTARGET_FREQ: %.2f KHz\nPrescaler: %u\nCounter_period: %u\n", MAIN_FREQ / 1000000.0, TARGET_FREQ/ 1000.0, *Prescaler, *Counter_period);

	#endif
    return 0; // 成功
}

/**
 * @brief   更新运行时间（毫秒）
 *
 * @param   runtime     运行时间结构体指针
 *
 * @return  void
 */
void update_run_time_ms(RunTime *runtime) 
{
    // 模拟时间的增加
    runtime->milliseconds += 10;
    if (runtime->milliseconds >= 1000) {
        runtime->milliseconds -= 1000;
        runtime->seconds++;
        if (runtime->seconds >= 60) {
            runtime->seconds -= 60;
            runtime->minutes++;
            if (runtime->minutes >= 60) {
                runtime->minutes -= 60;
                runtime->hours++;
                if (runtime->hours >= 24) {
                    runtime->hours -= 24;
                    runtime->days++;
                }
            }
        }
    }
}

/**
 * @brief   更新运行时间（秒）
 *
 * @param   runtime     运行时间结构体指针
 *
 * @return  void
 */
void update_run_time_s(RunTime *runtime) 
{
    // 模拟时间的增加
    runtime->seconds += 1;
        if (runtime->seconds >= 60) {
            runtime->seconds -= 60;
            runtime->minutes++;
            if (runtime->minutes >= 60) {
                runtime->minutes -= 60;
                runtime->hours++;
                if (runtime->hours >= 24) {
                    runtime->hours -= 24;
                    runtime->days++;
                }
            }
        }
}
