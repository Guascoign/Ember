#include "key.h"

/* 初始化按键 */
void Key_Init(key_t *key, GPIO_TypeDef *port, uint8_t pin, char *name)
{
	key->port = port;
	key->pin = pin;
	key->name = name;
	key->value = KEY_RELEASED;	//初始化为释放状态
	key->debounce_timer.timeout = ~0;
	key->debounce_timer.func = Key_Debounce_Callback; // 设置回调函数
	key->debounce_timer.args = key;	//将按键实例传递给回调函数
}

/* 按键外部中断触发函数 */
void Key_Interrupt_Handler(key_t *key)
{
	// 启动消抖定时器10ms
	Start_Soft_Timer(&key->debounce_timer, 10);
}

/* 定时器回调函数（定时器超时时执行） */
void Key_Debounce_Callback(void *args)
{
	key_t *key = (key_t *)args;

	// 检查按键状态
	if (HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET) {
		// 按键按下后处理逻辑（记录按键值或触发事件）
		lcdprintf("key pressed!\n");
		Beeper_Perform(KEY_PRESS);		// 蜂鸣器响声
	}
	else{
		lcdprintf("key Released!\n");
		Beeper_Perform(KEY_RELEASE);		// 蜂鸣器响声
	}
}
