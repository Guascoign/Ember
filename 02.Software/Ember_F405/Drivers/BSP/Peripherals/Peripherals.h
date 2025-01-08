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
#include "BSP/UART/uart_pack.h"
#include "BSP/ADC/adc_pack.h"

#define MAX_UART_DEVICES 10

typedef enum DeviceType{
    Empty_DEVICE = 0x00U,    //空设备
    UART_DEVICE = 0x01U,    //UART设备
    NTC_DEVICE = 0x02U,     //NTC设备
    EEPROM_DEVICE = 0x03U,  //EEPROM设备
} DeviceTypeDef;

typedef struct PeripheralDevice{
    char *name;
    void *handle;
    DeviceTypeDef type;
} PeripheralDeviceTypeDef;

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

