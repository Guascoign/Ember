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
#define CONTINUE_PRESS_TIME  500 //连续按下时间 0.5s
#define CONTINUE_PRESS  100 //连续按下触发递增（长按1s后 每0.1s触发一次按下事件）
#define CLICK_TIME  200 //多击间隔时间 200ms
#define DEBOUNCE_TIME  20 //消抖时间 10ms

typedef enum {
  Idle          = 0x00U,//空闲
  Press         = 0x01U,//按下
  Release       = 0x02U,//释放 调用单击回调
  Long_Press     = 0x03U,//长按
  Long_Release   = 0x04U,//长按释放 调用长按回调
  Continue_Press = 0x05U,//连续按下 连续调用连续按下回调
  Continue_Release = 0x06U,//连续释放 停用连续按下
  Double_Press   = 0x07U,//双击
  Double_Release= 0x08U,//双击结束 调用双击回调
  Triple_Press   = 0x09U,//三击
  Triple_Release= 0x0AU,//三击结束 调用三击回调
}Button_Event;

/* 按键结构体 */
typedef struct KEY_Device {
	char *name;				//按键名称
    uint8_t (*Read)(struct KEY_Device *p_KeyDev);//读取按键状态
    void (*Callback)(void *args); //按键回调处理函数
    uint8_t PressCount; //按键按下次数
	Soft_TimerTypeDef DeBounce_timer;	// 按键消抖定时器
    Soft_TimerTypeDef Mode_timer; //按键长按类定时器
    Soft_TimerTypeDef Click_timer; //双击定时器
	Button_Event value;				// 按键值
    void *priv_data;//私有数据 存入GPIO_DeviceTypeDef句柄
}KEY_DeviceTypeDef, *p_KEY_DeviceTypeDef;

int8_t Key_Init(KEY_DeviceTypeDef *p_keydev, char *name ,void *Instance ,uint16_t pin);

/* 定时器回调函数 */
void Key_DeBounce_Callback(void *args);//消抖回调函数
void Key_Mode_Callback(void *args);//按键类定时器回调函数

#endif  //__KEY_DEVICE_H
