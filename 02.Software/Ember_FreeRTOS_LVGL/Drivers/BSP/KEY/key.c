#include "KEY\key.h"
#include "LCD/lcd_consle.h"
#include "stm32f4xx_hal.h" // 确保包含 HAL 库头文件
#include "stdint.h"
#include "BEEP/beep.h"

extern TIM_HandleTypeDef    htim10; //10ms消抖定时器
extern uint8_t g_htim11_flag;
// 按键中断处理函数
void Key_HandleInterrupt(GPIO_TypeDef *port, uint16_t pin) {
    if(port == KEY1.KEY_GPIO_Port && pin == KEY1.GPIO_Pin)
   {
    HAL_TIM_Base_Start_IT(&htim10);
    if(g_htim11_flag)
    {
        KEY1.KEY_Callback(&KEY1);
    }
    
   }
    if(port == KEY2.KEY_GPIO_Port && pin == KEY2.GPIO_Pin)
   {
    HAL_TIM_Base_Start_IT(&htim10);
    if(g_htim11_flag)
    {
        KEY2.KEY_Callback(&KEY2);
    }
   }

}

// 按键初始化函数
void key_init(struct KEY_Device *p_KEYDev) {


}

// 按键中断处理函数
void key_callabck(struct KEY_Device *p_KEYDev) {
    switch (p_KEYDev->Status)
    {
    case KEY_EVENT_RELEASE:
        {
            if(HAL_GPIO_ReadPin(p_KEYDev->KEY_GPIO_Port , p_KEYDev->GPIO_Pin) == 0)
            {
                Beeper_Perform(KEY_PRESS);		// 蜂鸣器响声
                p_KEYDev->Status = KEY_EVENT_PRESS;
            }
        }
    case KEY_EVENT_PRESS:
        {
            if(HAL_GPIO_ReadPin(p_KEYDev->KEY_GPIO_Port , p_KEYDev->GPIO_Pin) == 1)
            {
                Beeper_Perform(KEY_RELEASE);		// 蜂鸣器响声
                p_KEYDev->Status = KEY_EVENT_RELEASE;
            }
            break;
        }
        
    default:
        break;
    }
}


//KEY1初始化
struct KEY_Device KEY1 = {
    .KEY_GPIO_Port = KEY1_GPIO_Port,
    .GPIO_Pin = KEY1_Pin,
    .KEY_Init = key_init,//待编写
    .name = "KEY1",
    .Status = KEY_EVENT_RELEASE,
    .KEY_Callback = key_callabck,//待编写
    .priv_data = NULL
};
//KEY2初始化
struct KEY_Device KEY2 = {
    .KEY_GPIO_Port = KEY2_GPIO_Port,
    .GPIO_Pin = KEY2_Pin,
    .KEY_Init = key_init,//待编写
    .name = "KEY2",
    .Status = KEY_EVENT_RELEASE,
    .KEY_Callback = key_callabck,//待编写
    .priv_data = NULL
};
