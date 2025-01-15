/********************************************************************************
    * 文件名称 ：key_device.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LED驱动
*********************************************************************************/
#ifndef __KEY_DEVICE_H
#define __KEY_DEVICE_H

#include "BSP/Peripherals/Peripherals.h"
#include "BSP/GPIO/gpio_device.h"
#include "LIB/soft_timer.h"//软件定时器
//#include "circle_buffer.h"

#define LONG_PRESS_TIME  500 //长按时间 0.5s
#define CONTINUE_PRESS_TIME  1000 //连续按下时间 1s
#define CONTINUE_PRESS  100 //连续按下触发递增（长按1s后 每0.1s触发一次按下事件）
#define DOUBLE_CLICK_TIME  300 //双击间隔时间 300ms
#define DEBOUNCE_TIME  20 //消抖时间 10ms

typedef enum {
  Press         = 0x00U,//按下
  Release       = 0x01U,//释放
  LongPress     = 0x02U,//长按
  LongRelease   = 0x03U,//长按释放
  ContinuePress = 0x04U,//连续按下
  ContinueRelease = 0x05U,//连续释放
  DoubleClick   = 0x06U,//双击
  TripleClick   = 0x07U,//三击
}Button_Event;

/* 按键结构体 */
typedef struct KEY_Device {
	char *name;				//按键名称
    uint8_t (*Read)(struct KEY_Device *p_KeyDev);//读取按键状态
    void (*Callback)(void *args); //按键回调处理函数
	soft_timer DeBounce_timer;	// 按键消抖定时器
    soft_timer Mode_timer; //按键类定时器
	Button_Event value;				// 按键值
    void *priv_data;//私有数据 存入GPIO_DeviceTypeDef句柄
}KEY_DeviceTypeDef, *p_KEY_DeviceTypeDef;

int8_t Key_Init(KEY_DeviceTypeDef *p_keydev, char *name ,void *Instance ,uint16_t pin);

/* 定时器回调函数 */
void Key_DeBounce_Callback(void *args);//消抖回调函数
void Key_Mode_Callback(void *args);//按键类定时器回调函数

#endif  //__KEY_DEVICE_H
