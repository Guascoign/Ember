/********************************************************************************
    * 文件名称 ：Peripherals.h
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-08
    * 功     能：外设注册表
*********************************************************************************
V1.0 2025-01-08 First version
*********************************************************************************/
/*使用说明*************************************************************************
Add_Device(name,p_UARTDev->priv_data);//添加到外设列表
Print_Devices();//打印所有已注册的外设
*********************************************************************************/
#ifndef __PERIPHERALS_H
#define __PERIPHERALS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "BSP/UART/uart_device.h"
#include "BSP/GPIO/led_device.h"
#include "BSP/GPIO/gpio_device.h"
#include "BSP/RGB/rgb_device.h"
#include "BSP/BEEPER/beeper_device.h"
#include "BSP/GPIO/key_device.h"

#define MAX_UART_DEVICES 10 //最大设备数量

/**
 * @brief    设备类型
 * @param    Empty_DEVICE	空设备
 * @param    UART_DEVICE	UART设备
 * @param    NTC_DEVICE	NTC设备
 * @param    EEPROM_DEVICE	EEPROM设备
 * @param    INPUT_DEVICE	输入设备(GPIO)
 * @param    OUTPUT_DEVICE	输出设备(GPIO)
 * @param    KEY_DEVICE	按键设备
 * @param    LED_DEVICE	LED设备
 * @param    FLASH_DEVICE	FLASH设备
 * @param    RGB_DEVICE	RGB设备
 * @param    BEEP_DEVICE	蜂鸣器设备
 * @param    ADC_DEVICE	ADC设备
 */
typedef enum DeviceType{
    Empty_DEVICE = 0x00U,    //空设备
    UART_DEVICE = 0x01U,    //UART设备
    NTC_DEVICE = 0x02U,     //NTC设备
    EEPROM_DEVICE = 0x03U,  //EEPROM设备
    INPUT_DEVICE = 0x04U,   //输入设备(GPIO)
    OUTPUT_DEVICE = 0x05U,  //输出设备(GPIO)
    KEY_DEVICE = 0x06U,     //按键设备
    LED_DEVICE = 0x07U,     //LED设备
    FLASH_DEVICE = 0x08U,   //FLASH设备
    RGB_DEVICE = 0x09U,     //RGB设备
    BEEP_DEVICE = 0x0AU,    //蜂鸣器设备
    ADC_DEVICE = 0x0BU,     //ADC设备
} DeviceTypeDef;

/**
 * @brief    外设结构体
 * @param    name	设备名称
 * @param    handle	设备句柄
 * @param    type	设备类型
 */
typedef struct PeripheralDevice{
    char *name; //设备名称
    void *handle;   //设备句柄
    DeviceTypeDef type; //设备类型
} PeripheralDeviceTypeDef, *p_PeripheralDeviceTypeDef;

extern PeripheralDeviceTypeDef **peripheral_devices;
extern int peripheral_device_count;

void All_Peripherals_Init(void);
int Add_Device(char *name, void *handle, DeviceTypeDef type);
void Print_Devices(void);
void *Get_Device(char *name);
void Free_Devices(void *name);
void *Get_Device_Handle(char *name);
DeviceTypeDef Get_Device_Type(char *name);
void Print_Device_Info(char *name);
void Print_AllDevice_Info(void);

#endif  // __PERIPHERALS_H

