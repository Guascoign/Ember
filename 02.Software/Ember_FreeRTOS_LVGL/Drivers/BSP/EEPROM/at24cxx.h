/********************************************************************************
     *文件名称 ：at24cxx.h
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动头文件
*********************************************************************************/
#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "IIC/iic.h"
#include "main.h"
#include <stdint.h>

#define AT24_Address1 0x50  //七位地址  1010 （A0）（A1）（A2）（R/W）

typedef enum {
    AT24C01 = 0,
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32,
    AT24C64,
    AT24C128,
    AT24C256
} AT24CXX_EEP_TYPE;

typedef struct AT24CXX_Type
{
    uint32_t EEP_SIZE;      //存储器类型（存储器容量）
    uint16_t EEP_PAGENUM;    //页数
    uint16_t EEP_PAGESIZE;   //每页字节数
    uint8_t ADDR;           //地址
}AT24CXX_TypeDef;           //存储器类型定义

//定义AT24CXX类
typedef struct AT24CXX_Device
{
    char *name;             //存储器名称
    AT24CXX_TypeDef *EEP_TYPE;        //存储器类型（存储器容量）
    IIC_BusTypeDef *IIC_Bus;                          // IIC设备接口
    uint8_t Address;
    uint8_t (*AT24CXX_ReadReg)(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr);//读取一个字节
    void (*AT24CXX_WriteReg)(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t DataToWrite);//写入一个字节
    void (*AT24CXX_ReadRegs)(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);//连续读取(页读)
    void (*AT24CXX_WriteRegs)(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);//连续写入(页写)
    uint8_t (*AT24CXX_Check)(struct AT24CXX_Device *p_AT24Dev);   //检查器件
    void *priv_data;//私有数据
}AT24CXX_DeviceDef;

AT24CXX_DeviceDef Create_AT24CXX(char *name,uint8_t Address,AT24CXX_EEP_TYPE EEP_TYPE,GPIO_TypeDef  *IIC_SCL_Port,GPIO_TypeDef  *IIC_SDA_Port,uint16_t IIC_SCL_Pin,uint16_t IIC_SDA_Pin);

#endif //__AT24CXX_H
