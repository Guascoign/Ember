/********************************************************************************
     *文件名称 ：at24cxx.h
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动头文件
*********************************************************************************/
#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "main.h"
#include <stdint.h>
#include "iic.h"

#define AT24_Address 0xA0

// #define USE_AT24C01    //使用AT24C01
 #define USE_AT24C02    //使用AT24C02
// #define USE_AT24C04    //使用AT24C04
// #define USE_AT24C08    //使用AT24C08
// #define USE_AT24C16    //使用AT24C16
// #define USE_AT24C32    //使用AT24C32
// #define USE_AT24C64    //使用AT24C64
// #define USE_AT24C128   //使用AT24C128
// #define USE_AT24C256   //使用AT24C256



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
    IIC_DeviceTypeDef *IIC_Device;                          // IIC设备接口
    void (*AT24CXX_Init)(struct AT24CXX_Device *p_AT24Dev); //初始化AT24CXX
    uint8_t (*AT24CXX_ReadOneByte)(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr);//读取一个字节
    void (*AT24CXX_WriteOneByte)(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t DataToWrite);//写入一个字节
    void (*AT24CXX_Write)(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);//连续写入(页写)
    void (*AT24CXX_Read)(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);//连续读取(页读)
    uint8_t (*AT24CXX_Check)(struct AT24CXX_Device *p_AT24Dev);   //检查器件
    void *priv_data;//私有数据
}AT24CXX_DeviceDef;

struct AT24CXX_Device *AT24CXX_GetDevice(char *name);

#endif //__AT24CXX_H
