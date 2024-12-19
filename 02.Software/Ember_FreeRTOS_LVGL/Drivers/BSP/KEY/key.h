#ifndef KEY_H
#define KEY_H

#include "stm32f4xx_hal.h" // 包含 HAL 库头文件
#include "FreeRTOS.h"
#include "timers.h"
#include "main.h"
#include "BEEP/beep.h"
#include "circle_buffer.h"
#include "soft_timer.h"

/* 按键设备定义 */
typedef struct {
	GPIO_TypeDef *KEY_GPIO_Port;       // 按键 GPIO 端口
	uint16_t GPIO_Pin;                 // 按键 GPIO 引脚
	char *name;                        // 按键名称
	uint8_t Status;                    // 当前按键状态
	void (*KEY_Callback)(void *args);  // 按键事件回调函数
	void *priv_data;                   // 私有数据
} KEY_DeviceTypeDef;

/* 环形缓冲区声明 */
extern circle_buf g_key_bufs;

/* 全局按键计数器 */
extern int g_key_cnt;

/* 定义软件定时器 */
extern struct soft_timer key_timer;

/* 按键超时回调函数 */
void key_timeout_func(void *args);

#endif // KEY_H
