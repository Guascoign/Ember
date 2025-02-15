/********************************************************************************
    * 文件名称 ：iic_device.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-16
    * 功     能：IIC驱动
*********************************************************************************/
#include "BSP/IIC/iic_device.h"

static void IIC_Start(IIC_DeviceTypeDef *P_IICDev)
{
    IIC_SDA(1);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    IIC_SDA(0);
    Delay_us(IIC_Delay);
    IIC_SCL(0);
    Delay_us(IIC_Delay);
}

static void IIC_Restart(IIC_DeviceTypeDef *P_IICDev)
{
    IIC_SDA(1);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    IIC_SDA(0);
    Delay_us(IIC_Delay);
    IIC_SCL(0);
    Delay_us(IIC_Delay);
}

static void IIC_Stop(IIC_DeviceTypeDef *P_IICDev)
{
    IIC_SDA(0);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    IIC_SDA(1);
    Delay_us(IIC_Delay);
}

static void IIC_Ack(IIC_DeviceTypeDef *P_IICDev, uint8_t ack)
{
    IIC_SDA(ack);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    IIC_SCL(0);
    Delay_us(IIC_Delay);
}

static void IIC_Nack(IIC_DeviceTypeDef *P_IICDev)
{
    IIC_SDA(1);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    IIC_SCL(0);
    Delay_us(IIC_Delay);
}

static uint8_t IIC_WaitAck(IIC_DeviceTypeDef *P_IICDev)
{
    uint8_t ucErrTime = 0;
    IIC_SDA(1);
    Delay_us(IIC_Delay);
    IIC_SCL(1);
    Delay_us(IIC_Delay);
    while (IIC_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop(P_IICDev);
            return 1;
        }
    }
    IIC_SCL(0);
    Delay_us(IIC_Delay);
    return 0;
}

static void IIC_SendByte(IIC_DeviceTypeDef *P_IICDev, uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        IIC_SDA((data & 0x80) >> 7);
        data <<= 1;
        Delay_us(IIC_Delay);
        IIC_SCL(1);
        Delay_us(IIC_Delay);
        IIC_SCL(0);
        Delay_us(IIC_Delay);
    }
}

static uint8_t IIC_ReadByte(IIC_DeviceTypeDef *P_IICDev, uint8_t ack)
{
    uint8_t i, receive = 0;
    IIC_SDA(1);
    for (i = 0; i < 8; i++)
    {
        IIC_SCL(1);
        Delay_us(IIC_Delay);
        receive <<= 1;
        if (IIC_READ_SDA)
            receive++;
        IIC_SCL(0);
        Delay_us(IIC_Delay);
    }
    IIC_Ack(P_IICDev, ack);
    return receive;
}

void IIC_Init(IIC_DeviceTypeDef *P_IICDev,char *name,void *SCL_Instance ,uint16_t SCL_Pin,void *SDA_Instance ,uint16_t SDA_Pin)
{
    P_IICDev->name = name;
    P_IICDev->init = IIC_Init;
    P_IICDev->start = IIC_Start;
    P_IICDev->restart = IIC_Restart;
    P_IICDev->stop = IIC_Stop;
    P_IICDev->Ack = IIC_Ack;
    P_IICDev->Nack = IIC_Nack;
    P_IICDev->waitAck = IIC_WaitAck;
    P_IICDev->sendByte = IIC_SendByte;
    P_IICDev->readByte = IIC_ReadByte;
    P_IICDev->priv_data = (void *)malloc(sizeof(GPIO_DeviceTypeDef) * 2);
    GPIO_Init((GPIO_DeviceTypeDef *)P_IICDev->priv_data, name ,SCL_Instance , SCL_Pin);
    GPIO_Init((GPIO_DeviceTypeDef *)P_IICDev->priv_data + 1, name ,SDA_Instance , SDA_Pin);
    IIC_Stop(P_IICDev);
}
