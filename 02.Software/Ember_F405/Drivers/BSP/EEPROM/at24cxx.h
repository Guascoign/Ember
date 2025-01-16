/********************************************************************************
    * 文件名称 ：at24cxx.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-16
    * 功     能：EEPROM驱动
*********************************************************************************/
#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "BSP/IIC/iic_device.h"
#include "BSP/Peripherals/Peripherals.h"

#define AT24CXX_ADDR 0X50 //七位地址  1010 （A0）（A1）（A2）（R/W）

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

typedef struct AT24CXX_Device AT24CXX_DeviceTyepeDef, *p_AT24CXX_DeviceTyepeDef;

typedef struct AT24CXX_Device
{
    char *name;             //存储器名称
    AT24CXX_TypeDef *EEP_TYPE;        //存储器类型（存储器容量）
    uint8_t Address;
    uint8_t (*ReadReg)(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t ReadAddr);//读取一个字节
    void (*WriteReg)(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t WriteAddr, uint8_t DataToWrite);//写入一个字节
    void (*ReadRegs)(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);//连续读取(页读)
    void (*WriteRegs)(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);//连续写入(页写)
    uint8_t (*Check)(AT24CXX_DeviceTyepeDef *p_AT24Dev);   //检查器件
    void *priv_data;//私有数据 存入IIC_DeviceTypeDef结构体指针
}AT24CXX_DeviceTyepeDef, *p_AT24CXX_DeviceTyepeDef;

void AT24CXX_Init(AT24CXX_DeviceTyepeDef *p_AT24Dev, char *name, 
                    AT24CXX_EEP_TYPE EEP_TYPE, uint8_t Address ,
                    void *SCL_Instance ,uint16_t SCL_Pin ,
                    void *SDA_Instance ,uint16_t SDA_Pin);


#endif  // __AT24CXX_H
