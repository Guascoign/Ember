/********************************************************************************
    * 文件名称 ：iic.h
    * 版     本：V1.1
    * 编写日期 ：2024-05-09
    * 功     能：IIC驱动
*********************************************************************************
V1.0 2024-01-30 First release @ZM
V1.1 2024-05-09 Add IIC_TypeDef @ZM
*********************************************************************************/
#ifndef __IIC_H
#define __IIC_H

#include <stdint.h>
#include "main.h"
#include <stdio.h>
#include "delay.h"
#include "crc.h"

 #define IIC_Delay     1

//定义IIC接口
typedef struct IIC_Device
{
	char *name;//设备名称
	//管脚
	  GPIO_TypeDef  *IIC_SCL_Port; 	//GPIO_SCL所属的GPIO组(如：GPIOA)
	  GPIO_TypeDef  *IIC_SDA_Port; 	//GPIO_SDA所属的GPIO组(如：GPIOA)
	  uint16_t IIC_SCL_Pin;    	//GPIO_SCL的IO引脚(如：GPIO_PIN_0)
	  uint16_t IIC_SDA_Pin;    	//GPIO_SDA的IO引脚(如：GPIO_PIN_0)
	  //操作函数
	  void (*IIC_Init)(const struct IIC_Device *p_IICDev); //初始化IIC
	  void (*IIC_Deint)(const struct IIC_Device *p_IICDev); //反初始化IIC
	  void (*IIC_Start)(const struct IIC_Device *p_IICDev); //发送IIC开始信号
	  void (*IIC_Restart)(const struct IIC_Device *p_IICDev); //发送IIC重新开始信号
	  void (*IIC_Stop)(const struct IIC_Device *p_IICDev); //发送IIC停止信号
	  void (*IIC_Ack)(const struct IIC_Device *p_IICDev); //IIC发送ACK信号
	  void (*IIC_Nack)(const struct IIC_Device *p_IICDev); //IIC不发送ACK信号
	  uint8_t (*IIC_WaitAck)(const struct IIC_Device *p_IICDev); //IIC等待ACK信号
	  void (*IIC_SendByte)(const struct IIC_Device *p_IICDev,uint8_t Send_data); //IIC发送一个字节
	  uint8_t (*IIC_ReadByte)(const struct IIC_Device *p_IICDev , uint8_t ack); //IIC读取一个字节
	  void (*IIC_WriteRegByte)(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData);//向寄存器写入一个字节
	  uint8_t (*IIC_ReadRegByte)(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress);//从寄存器读取一个字节
	  void (*IIC_WriteReg)(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t len , uint8_t *pbuf);//向寄存器连续写
	  void (*IIC_ReadReg)(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress , uint8_t len , uint8_t *pbuf);//向寄存器连续读
	  void (*delay_us)(uint32_t nus); //延时函数
	  void *priv_data;//私有数据
}IIC_DeviceTypeDef;

struct IIC_Device *GetIICDevice(char *name);//获取外设地址指针

extern struct IIC_Device IIC1;//IIC.c定义接口

#endif
