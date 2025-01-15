/********************************************************************************
    * 文件名称 ：key_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：led设备
*********************************************************************************
V1.0 2025-01-13 First version 
*********************************************************************************
void Check_KEY_Soft_Timer(void *args) 检查按键定时器是否超时
void Key_Process(KEY_DeviceTypeDef *p_keydev) 按键处理函数
*********************************************************************************/
#include "BSP/GPIO/key_device.h"
#include "BSP/LCD/lcd_consle.h"

extern KEY_DeviceTypeDef key1;
extern KEY_DeviceTypeDef key2;

#ifdef USE_HAL

/**
 * @brief    HAL库中断回调函数
 *
 * @param   GPIO_Pin	引脚号
 *
 * @return  void
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == KEY1_Pin)
	{
		Start_Soft_Timer(&key1.DeBounce_timer, DEBOUNCE_TIME); // 启动按键1消抖定时器
	}
  if (GPIO_Pin == KEY2_Pin)
	{
		Start_Soft_Timer(&key2.DeBounce_timer, DEBOUNCE_TIME); // 启动按键2消抖定时器
	}
}
#endif//USE_HAL

/**
 * @brief    按键消抖回调函数,判断长按
 *
 * @param   args	按键设备句柄
 *
 * @return  void
 */
void Key_Mode_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	if(KEY->Read(KEY) == 0)
	{
		if(KEY->value == Press)
		{
			KEY->value = Long_Press;
			KEY->Mode_timer.Disable_Refresh = 0;//使能时间刷新
			Start_Soft_Timer(&KEY->Mode_timer, CONTINUE_PRESS_TIME); // 启动按键连续按下定时器
		}
		else if(KEY->value == Long_Press)
		{
			KEY->value = Continue_Press;
			KEY->Callback(KEY);//处理连续按下事件
			KEY->Mode_timer.timeout = ~0;//复位
		}
	}
}

/**
 * @brief    按键多击超时回调函数，判断单击
 *
 * @param   args	按键设备句柄
 *
 * @return  void
 */
void Key_Click_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	KEY->Click_timer.timeout = ~0;//复位
	if(KEY->value == Release || KEY->value == Double_Release || KEY->value == Triple_Release)//处理单击事件
	{
		KEY->Callback(KEY);
	}
	KEY->PressCount = 0;//按键次数清零
}

/**
 * @brief    按键消抖回调函数
 *
 * @param   args	按键设备句柄
 *
 * @return  void
 */
void Key_DeBounce_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	if(KEY->Read(KEY) == 0)//按下事件
	{
		if(KEY->PressCount == 0)//一个按键事件周期结束
		{
			KEY->value = Press;
			if(KEY->Mode_timer.Disable_Refresh == 0)
			{
				Start_Soft_Timer(&KEY->Mode_timer, LONG_PRESS_TIME); // 启动按键连续按下定时器
			}
			Start_Soft_Timer(&KEY->Click_timer, CLICK_TIME); // 启动按键多击超时定时器,超时后处理单击事件
		}
		else if(KEY->PressCount == 1)//双击
		{
			KEY->value = Double_Press;
			Start_Soft_Timer(&KEY->Click_timer, CLICK_TIME); // 启动按键多击超时定时器,超时后处理双击事件
		}
		else if(KEY->PressCount == 2)//三击
		{
			KEY->value = Triple_Press;
			Start_Soft_Timer(&KEY->Click_timer, CLICK_TIME); // 启动按键多击超时定时器,超时后处理三击事件
		}
	}
	else//松开事件
	{
		if(KEY->value == Press)
		{
			KEY->value = Release;
			KEY->Mode_timer.timeout = ~0;//复位长按定时器
			KEY->PressCount++;
		}
		else if(KEY->value == Long_Press)
		{
			KEY->Mode_timer.timeout = ~0;//复位
			KEY->value = Long_Release;
			KEY->Callback(KEY);//处理长按事件
		}
		else if(KEY->value == Continue_Press)
		{
			KEY->Mode_timer.timeout = ~0;//复位
			KEY->value = Continue_Release;
			KEY->Callback(KEY);//关闭连续按下事件
		}
		else if(KEY->value == Double_Press)
		{
			KEY->value = Double_Release;
			KEY->PressCount++;
		}
		else if(KEY->value == Triple_Press)
		{
			KEY->value = Triple_Release;
			KEY->PressCount++;
		}
	}
	KEY->DeBounce_timer.timeout = ~0;//复位
}

/**
 * @brief    读取按键状态
 *
 * @param   p_keydev	按键设备句柄
 *
 * @return  uint8_t
 */
static uint8_t Read(KEY_DeviceTypeDef *p_keydev)
{
	return ((GPIO_DeviceTypeDef *)p_keydev->priv_data)->Read(p_keydev->priv_data);
}

/**
 * @brief    按键回调函数
 *
 * @param   args	按键设备句柄
 *
 * @return  void
 */
static void Key_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	if(KEY->value == Release || KEY->PressCount == 1)//单击事件
	{
		/* 编写按键单击事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
	}
	else if(KEY->value == Double_Release || KEY->PressCount == 2)//双击事件
	{
		/* 编写按键双击事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
	}
	else if(KEY->value == Triple_Release || KEY->PressCount == 3)//三击事件
	{
		/* 编写按键三击事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
	}
	else if(KEY->value == Long_Release)//长按事件
	{
		/* 编写按键长按事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
	}
	else if(KEY->value == Continue_Press)//连续按下事件
	{
		/* 编写按键连续按下事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
		Start_Soft_Timer(&KEY->Continue_timer, CONTINUE_PRESS); // 启动按键连续按下定时器
	}
	else if(KEY->value == Continue_Release)//连续按下事件
	{
		/* 编写按键连续按下事件 */
		circle_buf_write(&KEY->Circle_buf, KEY->value);
		KEY->Continue_timer.timeout = ~0;//关闭连续按下事件
	}
}

/**
 * @brief    按键连续按下回调函数
 *
 * @param   args	按键设备句柄
 *
 * @return  void
 */
void Key_Continue_Callback(void *args)
{
	KEY_DeviceTypeDef *KEY = (KEY_DeviceTypeDef *)args;  // 将 void * 转换为 key_t 指针
	KEY->Continue_timer.timeout = ~0;//关闭连续按下事件
	circle_buf_write(&KEY->Circle_buf, Release);
	Start_Soft_Timer(&KEY->Continue_timer, CONTINUE_PRESS);
}

/**
 * @brief    初始化按键
 *
 * @param   p_keydev	按键设备句柄
 * @param   name	按键名称
 * @param   Instance	引脚基地址
 * @param   pin	引脚号
 *
 * @return  int8_t
 */
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
	p_keydev->Click_timer.func = Key_Click_Callback;
	p_keydev->Click_timer.args = (void *)p_keydev;
	p_keydev->Click_timer.timeout = ~0;
	p_keydev->Continue_timer.func = Key_Continue_Callback;
	p_keydev->Continue_timer.args = (void *)p_keydev;
	p_keydev->Continue_timer.timeout = ~0;
	//初始化环形缓冲区
	circle_buf_init(&p_keydev->Circle_buf, Circle_buf_size, (uint8_t *)malloc(Circle_buf_size));
	//初始化按键
	p_keydev->Read = Read;
	p_keydev->PressCount = 0;
	p_keydev->value = Idle;
	p_keydev->Callback = Key_Callback;
	return 0;
}

/**
 * @brief    按键处理函数
 *
 * @param   p_keydev	按键设备句柄
 *
 * @return  void
 */
void Key_Process(KEY_DeviceTypeDef *p_keydev)
{
	Button_Event value;
	if(circle_buf_read(&p_keydev->Circle_buf, &value) == 0)
	{
		switch (value)
		{
			case Release:
				{/* 编写按键单击事件 */
				lcdprintf("%s Press\n",p_keydev->name);
				break;
				}
			case Long_Release:
				{/* 编写按键长按事件 */
				lcdprintf("%s Long_Press\n",p_keydev->name);
				break;
				}
			case Continue_Press:
				{
				/* 编写按键连续按下事件 */
				lcdprintf("%s Continue_Press\n",p_keydev->name);
				break;
				}
			case Continue_Release:
				{/* 编写按键连续按下事件 */
				lcdprintf("%s Continue_Release\n",p_keydev->name);

				break;
				}
			case Double_Release:
				{/* 编写按键双击事件 */
				lcdprintf("%s Double_Press\n",p_keydev->name);
				break;
				}
			case Triple_Release:
				{/* 编写按键三击事件 */
				lcdprintf("%s Triple_Press\n",p_keydev->name);
				break;
				}
			default:
				break;
		}
	}
	
}
