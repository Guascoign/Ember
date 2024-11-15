/********************************************************************************
     *文件名称 ：at24cxx.c
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动
*********************************************************************************/
#include "at24cxx.h"
#include "string.h"

#ifdef USE_AT24C01
struct AT24CXX_Type AT24C01 = { 
    .EEP_SIZE = 128,
    .EEP_PAGENUM = 16,
    .EEP_PAGESIZE = 8,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C02
struct AT24CXX_Type AT24C02 = { 
    .EEP_SIZE = 256,
    .EEP_PAGENUM = 32,
    .EEP_PAGESIZE = 8,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C04
struct AT24CXX_Type AT24C04 = { 
    .EEP_SIZE = 512,
    .EEP_PAGENUM = 32,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C08
struct AT24CXX_Type AT24C08 = { 
    .EEP_SIZE = 1024,
    .EEP_PAGENUM = 64,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C16
struct AT24CXX_Type AT24C16 = { 
    .EEP_SIZE = 2048,
    .EEP_PAGENUM = 128,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C32
struct AT24CXX_Type AT24C32 = { 
    .EEP_SIZE = 4096,
    .EEP_PAGENUM = 128,
    .EEP_PAGESIZE = 32,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C64
struct AT24CXX_Type AT24C64 = { 
    .EEP_SIZE = 8192,
    .EEP_PAGENUM = 256,
    .EEP_PAGESIZE = 32,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C128
struct AT24CXX_Type AT24C128 = { 
    .EEP_SIZE = 16384,
    .EEP_PAGENUM = 512,
    .EEP_PAGESIZE = 64,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C256
struct AT24CXX_Type AT24C256 = { 
    .EEP_SIZE = 32768,
    .EEP_PAGENUM = 512,
    .EEP_PAGESIZE = 128,
    .ADDR = AT24_Address
};
#endif

/**
 * @brief   读取AT24CXX存储器中指定地址的数据
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 * @param   ReadAddr    开始读取数据的地址
 *
 * @return  读取到的数据
 */
static uint8_t AT24CXX_ReadOneByte(struct AT24CXX_Device *p_AT24Dev,uint16_t ReadAddr)
{
	uint8_t temp=0;                          
    p_AT24Dev->IIC_Device->IIC_Start(p_AT24Dev->IIC_Device);   //发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048) //AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, ReadAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    }
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, ReadAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    p_AT24Dev->IIC_Device->IIC_Restart(p_AT24Dev->IIC_Device);   //发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 1);    //发送地址 + 读命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    temp = p_AT24Dev->IIC_Device->IIC_ReadByte(p_AT24Dev->IIC_Device,0);//读取一个字节
    p_AT24Dev->IIC_Device->IIC_Nack(p_AT24Dev->IIC_Device);//发送nACK
    p_AT24Dev->IIC_Device->IIC_Stop(p_AT24Dev->IIC_Device);//产生一个停止条件
	return temp;
}

/**
 * @brief   在AT24CXX存储器中指定地址写入一个字节的数据
 *
 * @param   p_AT24Dev       AT24CXX存储器设备结构体指针
 * @param   WriteAddr       写入数据的目的地址
 * @param   DataToWrite     要写入的数据
 */
static void AT24CXX_WriteOneByte(struct AT24CXX_Device *p_AT24Dev,uint16_t WriteAddr,uint8_t DataToWrite)
{                                
    p_AT24Dev->IIC_Device->IIC_Start(p_AT24Dev->IIC_Device);   //发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)//AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, WriteAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    }
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, WriteAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device,DataToWrite);     //发送字节 
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
  	p_AT24Dev->IIC_Device->IIC_Stop(p_AT24Dev->IIC_Device);//产生一个停止条件
    p_AT24Dev->IIC_Device->delay_us(10000); //写完后需要一定的时间
}

/**
 * @brief   初始化IIC接口
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 */
static void AT24CXX_Init(struct AT24CXX_Device *p_AT24Dev)
{
    printf("AT24CXX_Init\r\n");
 	p_AT24Dev->IIC_Device->IIC_Init(p_AT24Dev->IIC_Device);//IIC初始化
}


/**
 * @brief   检查AT24CXX存储器设备
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 * @return  返回0表示检测成功，返回1表示检测失败
 */
// static uint8_t AT24CXX_Check(struct AT24CXX_Device *p_AT24Dev)   
// {
// 	uint8_t temp;
//  	temp = p_AT24Dev->AT24CXX_ReadOneByte(p_AT24Dev,p_AT24Dev->EEP_SIZE);//避免每次开机都写AT24CXX      
//  	if(temp == 0X33)return 0;     
//  	else//排除第一次初始化的情况
//  	{
//    		 p_AT24Dev->AT24CXX_WriteOneByte(p_AT24Dev,p_AT24Dev->EEP_SIZE,0X33);
//      		temp = p_AT24Dev->AT24CXX_ReadOneByte(p_AT24Dev,p_AT24Dev->EEP_SIZE);
//     		if(temp==0X33)return 0;
// 	 }
// 	 return 1;  
// }



static void AT24CXX_Write(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{                                
    uint16_t PageSize = p_AT24Dev->EEP_TYPE->EEP_PAGESIZE;
    uint16_t PageNum = p_AT24Dev->EEP_TYPE->EEP_PAGENUM;
    uint16_t CurrentPage = WriteAddr / PageSize; // 计算当前写入的数据所在的页数

    if (CurrentPage >= PageNum)
    {
        // 超过了总页数，无法写入
        return;
    }

    // 计算当前页的起始地址
    uint16_t PageStartAddr = WriteAddr - (WriteAddr % PageSize);

    // 计算当前页剩余可写入的字节数
    uint16_t RemainingBytes = PageSize - (WriteAddr % PageSize);

    // 如果要写入的数据量大于当前页剩余可写入的字节数，则需要分页写入
    while (NumToWrite > RemainingBytes)
    {
        // 先写入当前页剩余的字节
        p_AT24Dev->IIC_Device->IIC_Start(p_AT24Dev->IIC_Device);   // 发送起始信号
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 0);    // 发送地址 + 写命令
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
        if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
        {
            p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, PageStartAddr / 256);   // 发送高地址
            p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
        }
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, PageStartAddr % 256);   // 发送低地址
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);

        // 循环写入数据到当前页
        for(uint16_t i = 0; i < RemainingBytes; i++)
        {
            p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, pBuffer[i]);  // 发送字节
            p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
        }

        p_AT24Dev->IIC_Device->IIC_Stop(p_AT24Dev->IIC_Device);  // 产生一个停止条件
        p_AT24Dev->IIC_Device->delay_us(10000); // 写完后需要一定的时间

        // 更新要写入的数据和写入地址
        NumToWrite -= RemainingBytes;
        pBuffer += RemainingBytes;
        WriteAddr += RemainingBytes;

        // 计算下一页的起始地址和剩余可写入的字节数
        PageStartAddr += PageSize;
        RemainingBytes = PageSize;
    }

    // 写入剩余的数据（不足一页的情况）
    p_AT24Dev->IIC_Device->IIC_Start(p_AT24Dev->IIC_Device);   // 发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 0);    // 发送地址 + 写命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, WriteAddr / 256);   // 发送高地址
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    }
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, WriteAddr % 256);   // 发送低地址
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);

    // 循环写入剩余的数据
    for(uint16_t i = 0; i < NumToWrite; i++)
    {
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, pBuffer[i]);  // 发送字节
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    }

    p_AT24Dev->IIC_Device->IIC_Stop(p_AT24Dev->IIC_Device);  // 产生一个停止条件
    p_AT24Dev->IIC_Device->delay_us(10000); // 写完后需要一定的时间
}


void AT24CXX_Read(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    p_AT24Dev->IIC_Device->IIC_Start(p_AT24Dev->IIC_Device);   //发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048) //AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, ReadAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);
    }
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, ReadAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);

    p_AT24Dev->IIC_Device->IIC_Restart(p_AT24Dev->IIC_Device);   //发送起始信号
    p_AT24Dev->IIC_Device->IIC_SendByte(p_AT24Dev->IIC_Device, AT24_Address | 1);    //发送地址 + 读命令
    p_AT24Dev->IIC_Device->IIC_WaitAck(p_AT24Dev->IIC_Device);

    // 读取数据
    for(uint16_t i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = p_AT24Dev->IIC_Device->IIC_ReadByte(p_AT24Dev->IIC_Device, i == NumToRead - 1 ? 0 : 1); //读取一个字节
    }

    p_AT24Dev->IIC_Device->IIC_Stop(p_AT24Dev->IIC_Device);  // 产生一个停止条件
}



//实例化AT24CXX对象
extern struct IIC_Device IIC2;//IIC.c定义接口
struct AT24CXX_Device AT24C64_device = {
    .name = "AT24C64",
    .EEP_TYPE =  &AT24C64,
    .IIC_Device = &IIC2,
    .AT24CXX_Init = AT24CXX_Init,
    .AT24CXX_ReadOneByte = AT24CXX_ReadOneByte, //done
    .AT24CXX_WriteOneByte = AT24CXX_WriteOneByte,   //done
    .AT24CXX_Read = AT24CXX_Read,
    .AT24CXX_Write = AT24CXX_Write,
    .AT24CXX_Check = NULL,
};


/**
  *@brief   AT24设备数组
 */
struct AT24CXX_Device *AT24CXX_Devices[] = {
    &AT24C64_device  //AT24C64
};

/**
  *@brief   根据设备名称获取对应的EEPROM设备指针
 *
  *@param   name    设备名称
  *@return  返回对应设备名称的EEPROM设备指针，如果找不到则返回NULL
 */
struct AT24CXX_Device *AT24CXX_GetDevice(char *name)
{
    int i = 0;
        for (i = 0; i < sizeof(AT24CXX_Devices)/sizeof(AT24CXX_Devices[0]); i++)
    {
        if (0 == strcmp(name, AT24CXX_Devices[i]->name))
            return AT24CXX_Devices[i];
    }
    return NULL;
}



