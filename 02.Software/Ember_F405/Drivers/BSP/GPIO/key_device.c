/********************************************************************************
    * 文件名称 ：key_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：led设备
*********************************************************************************
V1.0 2025-01-13 First version
*********************************************************************************
Check_LED_Soft_Timer( (void *)&RUNLED );//放入1ms定时器中断
*********************************************************************************/
#include "BSP/GPIO/key_device.h"
#include "BSP/LCD/lcd_consle.h"
#include "BSP/LCD/lcd.h" 

extern KEY_DeviceTypeDef key1;
extern KEY_DeviceTypeDef key2;
/* 外部中断回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == KEY1_Pin)
	{
		Start_Soft_Timer(&key1.DeBounce_timer, DEBOUNCE_TIME); // 启动按键1的定时
	}
  if (GPIO_Pin == KEY2_Pin)
	{
		Start_Soft_Timer(&key2.DeBounce_timer, DEBOUNCE_TIME); // 启动按键2定时器
	}
}

void Key_Mode_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	if(KEY->Read(KEY) == 0)
	{
		KEY->value = ContinuePress;
		//lcd_printf("ContinuePress Name:%s\r\n",KEY->name);
	}
	else
	{
		KEY->value = ContinueRelease;
		//lcd_printf("ContinueRelease Name:%s\r\n",KEY->name);
	}
}


void Key_DeBounce_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	if(KEY->Read(KEY) == 0)
	{
		KEY->value = Press;
		//lcd_printf("Press Name:%s\r\n",KEY->name);
		Start_Soft_Timer(&KEY->Mode_timer, LONG_PRESS_TIME); // 启动按键长按定时器
	}
	else
	{
		KEY->value = Release;
		//lcd_printf("Release Name:%s\r\n",KEY->name);
	}
}

int8_t Key_Init(KEY_DeviceTypeDef *p_keydev, char *name ,void *Instance ,uint16_t pin)
{
	if(p_keydev == NULL)
		return 1; //参数错误
	p_keydev->name = name;
	p_keydev->priv_data = malloc(sizeof(GPIO_DeviceTypeDef));
    if(p_keydev->priv_data == NULL)
        return 1; //内存分配失败
    //初始化GPIO
    ((GPIO_DeviceTypeDef *)p_keydev->priv_data)->instance = Instance;
    ((GPIO_DeviceTypeDef *)p_keydev->priv_data)->pin = pin;
    ((GPIO_DeviceTypeDef *)p_keydev->priv_data)->name = name;
    GPIO_Init((GPIO_DeviceTypeDef *)p_keydev->priv_data,name,Instance,pin);
	//初始化定时器
	p_keydev->DeBounce_timer.func = Key_DeBounce_Callback;
	p_keydev->DeBounce_timer.args = (void *)p_keydev;
	p_keydev->DeBounce_timer.timeout = ~0;
	p_keydev->Mode_timer.func = Key_Mode_Callback;
	p_keydev->Mode_timer.args = (void *)p_keydev;
	p_keydev->Mode_timer.timeout = ~0;
	p_keydev->Read = ((GPIO_DeviceTypeDef *)p_keydev->priv_data)->Read;
	return 0;
}
