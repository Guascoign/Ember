#include "KEY\key.h"
#include "LCD/lcd_consle.h"
#include "stm32f4xx_hal.h" // 确保包含 HAL 库头文件



// 按键中断处理函数
void Key_HandleInterrupt(GPIO_TypeDef *port, uint16_t pin) {


}

// 按键中断处理函数
void key_init(const struct KEY_Device *p_KEYDev) {


}

// 按键中断处理函数
void key_callabck(const struct KEY_Device *p_KEYDev) {
    switch (p_KEYDev->Status)
    {
    case KEY_EVENT_PRESS:
        {
            lcdprintf("key press");
        }
    case KEY_EVENT_RELEASE:
        {
            lcdprintf("key release");
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
    .Status = KEY_EVENT_IDLE,
    .KEY_Callback = key_callabck,//待编写
    .priv_data = NULL
};
