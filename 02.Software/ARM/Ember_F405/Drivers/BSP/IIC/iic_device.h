/********************************************************************************
    * 文件名称 ：iic_device.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-16
    * 功     能：IIC驱动
*********************************************************************************/
#ifndef __IIC_DEVICE_H
#define __IIC_DEVICE_H

#include "BSP/Peripherals/Peripherals.h"
#include "BSP/GPIO/gpio_device.h"

#define IIC_Delay 4
#define Delay_us(x) P_IICDev->delay_us(x)
#define IIC_SCL(PinState) ((GPIO_DeviceTypeDef *)P_IICDev->priv_data)->Set((GPIO_DeviceTypeDef *)P_IICDev->priv_data, PinState)
#define IIC_SDA(x) ((GPIO_DeviceTypeDef *)P_IICDev->priv_data + 1)->Set((GPIO_DeviceTypeDef *)P_IICDev->priv_data + 1, x)
#define IIC_READ_SDA (((GPIO_DeviceTypeDef *)P_IICDev->priv_data + 1)->Read((GPIO_DeviceTypeDef *)P_IICDev->priv_data + 1))

typedef struct IIC_Device IIC_DeviceTypeDef , *p_IIC_DeviceTypeDef;

typedef struct IIC_Device
{
    char *name; //设备名称
    void (*init)(IIC_DeviceTypeDef *P_IICDev);
    void (*start)(IIC_DeviceTypeDef *P_IICDev);
    void (*restart)(IIC_DeviceTypeDef *P_IICDev);
    void (*stop)(IIC_DeviceTypeDef *P_IICDev);
    void (*Ack)(IIC_DeviceTypeDef *P_IICDev, uint8_t ack);
    void (*Nack)(IIC_DeviceTypeDef *P_IICDev);
    uint8_t (*waitAck)(IIC_DeviceTypeDef *P_IICDev);
    void (*sendByte)(IIC_DeviceTypeDef *P_IICDev, uint8_t data);
    uint8_t (*readByte)(IIC_DeviceTypeDef *P_IICDev, uint8_t ack);
    void (*delay_us)(uint32_t us);
    void *priv_data;//存入GPIO_HandleTypeDef结构体指针x2 先scl 后sda
} IIC_DeviceTypeDef, *p_IIC_DeviceTypeDef;

void IIC_Init(IIC_DeviceTypeDef *P_IICDev,char *name,void *SCL_Instance ,uint16_t SCL_Pin,void *SDA_Instance ,uint16_t SDA_Pin);

#endif  // __IIC_DEVICE_H
