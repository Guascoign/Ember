#ifndef KEY_H
#define KEY_H

#include "stm32f4xx_hal.h" // 包含 HAL 库头文件
#include "FreeRTOS.h"
#include "timers.h"
#include "main.h"
// 按键管理器
#define MAX_KEYS 2  //最大按键数量
#define LONG_PRESS_Delay 2000   //按下事件>2s即为长按
#define _DOUBLE_CLICK_Delay 500 //按下-松开-<0.5S-按下-松开 为双击

// 按键事件类型
typedef enum {
    KEY_EVENT_IDLE,             // 无事发生
    KEY_EVENT_PRESS,            // 按下事件
    KEY_EVENT_RELEASE,          // 松开事件
//    KEY_EVENT_LONG_PRESS,       // 长按事件
//    KEY_EVENT_DOUBLE_CLICK,     // 双击事件
} KeyEvent_t;

// 按键对象定义
typedef struct KEY_Device{
    GPIO_TypeDef *KEY_GPIO_Port;    // 按键 GPIO 端口
    uint16_t GPIO_Pin;          // 按键 GPIO 引脚
    void (*KEY_Init)(const struct KEY_Device *p_KEYDev); //初始化
    char *name;             //按键名称
    KeyEvent_t Status;      // 当前按键状态
    void (*KEY_Callback)(const struct KEY_Device *p_KEYDev);     // 按键事件回调函数
    void *priv_data;//私有数据
} KEY_DeviceTypeDef;

extern struct KEY_Device KEY1;//KEY.c定义接口
extern struct KEY_Device KEY2;//KEY.c定义接口



// 函数接口
void Key_HandleInterrupt(GPIO_TypeDef *port, uint16_t pin);

#endif // KEY_H
