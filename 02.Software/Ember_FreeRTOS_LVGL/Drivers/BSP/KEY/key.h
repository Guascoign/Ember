/********************************************************************************
	* 文件名称 ：key.h
	* 作 	 	 者：ZM
	* 版 	 	 本：V1.0
	* 编写日期 ：2024-12-19
	* 功 	 	 能：按键外部中断处理
*********************************************************************************/
#ifndef KEY_H
#define KEY_H

#include "soft_timer.h"
#include "main.h"

#define KEY_PRESSED 1	//按键按下
#define KEY_RELEASED 2	//按键松开
/* 按键结构体 */
typedef struct key {
	 	 uint8_t pin;				// 按键对应的引脚
	 	 GPIO_TypeDef *port;		// 按键对应的GPIO端口
		 char *name;				//按键名称
	 	 uint8_t value;				// 按键值
	 	 soft_timer debounce_timer;	// 按键消抖定时器
} key_t;

/* 初始化按键 */
void Key_Init(key_t *key, GPIO_TypeDef *port, uint8_t pin, char *name);
/* 按键外部中断触发函数 */
void Key_Interrupt_Handler(key_t *key);
/* 定时器回调函数 */
void Key_Debounce_Callback(void *args);

#endif // KEY_H
