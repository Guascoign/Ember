#include "KEY\key.h"

#include "key.h"
#include "circle_buffer.h" // 假定环形缓冲区的实现

/* 按键计数器 */
int g_key_cnt = 0;

/* 按键环形缓冲区 */
circle_buf g_key_bufs;

/* 软件定时器实例 */
struct soft_timer key_timer = {~0, NULL, key_timeout_func};

/* 按键超时回调函数 */
void key_timeout_func(void *args)
{
	uint8_t key_val; /* 按下是 0x1, 松开是 0x81 */
	g_key_cnt++;     // 计数器递增
	key_timer.timeout = ~0; // 关闭定时器

	/* 读取 GPIO 状态 */
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
		key_val = 0x1;
	else
		key_val = 0x9;

	/* 将按键值写入环形缓冲区 */
	circle_buf_write(&g_key_bufs, key_val);
}



