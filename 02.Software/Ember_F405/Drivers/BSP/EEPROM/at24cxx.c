/********************************************************************************
     *文件名称 ：at24cxx.c
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动
*********************************************************************************/
#include "EEPROM/at24cxx.h"
#include "string.h"

/**
 * @brief   读取AT24CXX存储器中指定地址的数据
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 * @param   ReadAddr    开始读取数据的地址
 *
 * @return  读取到的数据
 */
static uint8_t AT24CXX_ReadReg(struct AT24CXX_Device *p_AT24Dev,uint16_t ReadAddr)
{
	uint8_t temp=0;                          
    p_AT24Dev->IIC_Bus->IIC_Start(p_AT24Dev->IIC_Bus);   //发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048) //AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, ReadAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    }
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, ReadAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    p_AT24Dev->IIC_Bus->IIC_Restart(p_AT24Dev->IIC_Bus);   //发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 1);    //发送地址 + 读命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    temp = p_AT24Dev->IIC_Bus->IIC_ReceiveByte(p_AT24Dev->IIC_Bus,0);//读取一个字节
    p_AT24Dev->IIC_Bus->IIC_Nack(p_AT24Dev->IIC_Bus);//发送nACK
    p_AT24Dev->IIC_Bus->IIC_Stop(p_AT24Dev->IIC_Bus);//产生一个停止条件
	return temp;
}

/**
 * @brief   在AT24CXX存储器中指定地址写入一个字节的数据
 *
 * @param   p_AT24Dev       AT24CXX存储器设备结构体指针
 * @param   WriteAddr       写入数据的目的地址
 * @param   DataToWrite     要写入的数据
 */
static void AT24CXX_WriteReg(struct AT24CXX_Device *p_AT24Dev,uint16_t WriteAddr,uint8_t DataToWrite)
{                                
    p_AT24Dev->IIC_Bus->IIC_Start(p_AT24Dev->IIC_Bus);   //发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)//AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, WriteAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    }
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, WriteAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus,DataToWrite);     //发送字节 
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
  	p_AT24Dev->IIC_Bus->IIC_Stop(p_AT24Dev->IIC_Bus);//产生一个停止条件
    p_AT24Dev->IIC_Bus->delay_us(10000); //写完后需要一定的时间
}

/**
 * @brief   检查AT24CXX存储器设备
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 * @return  返回0表示检测成功，返回1表示检测失败
 */
static uint8_t AT24CXX_Check(struct AT24CXX_Device *p_AT24Dev)   
{
	uint8_t temp;
 	temp = p_AT24Dev->AT24CXX_ReadReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_SIZE);//避免每次开机都写AT24CXX      
 	if(temp == 0X67)return 0;     
 	else//排除第一次初始化的情况
 	{
   		 p_AT24Dev->AT24CXX_WriteReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_SIZE,0X67);
     		temp = p_AT24Dev->AT24CXX_ReadReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_SIZE);
    		if(temp==0X67)return 0;
	 }
     #ifdef Debug_Logs
        printf("EEprom Check ERROR");
     #endif
	 return 1;  
}



static void AT24CXX_WriteRegs(struct AT24CXX_Device *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{                                
    uint16_t PageSize = p_AT24Dev->EEP_TYPE->EEP_PAGESIZE;
    uint16_t PageNum = p_AT24Dev->EEP_TYPE->EEP_PAGENUM;
    uint16_t CurrentPage = WriteAddr / PageSize; // 计算当前写入的数据所在的页数

    if (CurrentPage >= PageNum)
    {
        // 超过了总页数，无法写入
        #ifdef Debug_Logs
            printf("EEPROM Write REG Overflow");
        #endif
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
        p_AT24Dev->IIC_Bus->IIC_Start(p_AT24Dev->IIC_Bus);   // 发送起始信号
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 0);    // 发送地址 + 写命令
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
        if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
        {
            p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, PageStartAddr / 256);   // 发送高地址
            p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
        }
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, PageStartAddr % 256);   // 发送低地址
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);

        // 循环写入数据到当前页
        for(uint16_t i = 0; i < RemainingBytes; i++)
        {
            p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, pBuffer[i]);  // 发送字节
            p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
        }

        p_AT24Dev->IIC_Bus->IIC_Stop(p_AT24Dev->IIC_Bus);  // 产生一个停止条件
        p_AT24Dev->IIC_Bus->delay_us(10000); // 写完后需要一定的时间

        // 更新要写入的数据和写入地址
        NumToWrite -= RemainingBytes;
        pBuffer += RemainingBytes;
        WriteAddr += RemainingBytes;

        // 计算下一页的起始地址和剩余可写入的字节数
        PageStartAddr += PageSize;
        RemainingBytes = PageSize;
    }

    // 写入剩余的数据（不足一页的情况）
    p_AT24Dev->IIC_Bus->IIC_Start(p_AT24Dev->IIC_Bus);   // 发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 0);    // 发送地址 + 写命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, WriteAddr / 256);   // 发送高地址
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    }
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, WriteAddr % 256);   // 发送低地址
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);

    // 循环写入剩余的数据
    for(uint16_t i = 0; i < NumToWrite; i++)
    {
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, pBuffer[i]);  // 发送字节
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    }

    p_AT24Dev->IIC_Bus->IIC_Stop(p_AT24Dev->IIC_Bus);  // 产生一个停止条件
    p_AT24Dev->IIC_Bus->delay_us(10000); // 写完后需要一定的时间
}


void AT24CXX_ReadRegs(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    p_AT24Dev->IIC_Bus->IIC_Start(p_AT24Dev->IIC_Bus);   //发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 0);    //发送地址 + 写命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048) //AT24C32/64/128/256地址是2个字节的
    {
        p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, ReadAddr / 256);   //发送高地址
        p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);
    }
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, ReadAddr % 256);   //发送低地址
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);

    p_AT24Dev->IIC_Bus->IIC_Restart(p_AT24Dev->IIC_Bus);   //发送起始信号
    p_AT24Dev->IIC_Bus->IIC_SendByte(p_AT24Dev->IIC_Bus, (p_AT24Dev->Address<<1) | 1);    //发送地址 + 读命令
    p_AT24Dev->IIC_Bus->IIC_WaitAck(p_AT24Dev->IIC_Bus);

    // 读取数据
    for(uint16_t i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = p_AT24Dev->IIC_Bus->IIC_ReceiveByte(p_AT24Dev->IIC_Bus, i == NumToRead - 1 ? 0 : 1); //读取一个字节
    }

    p_AT24Dev->IIC_Bus->IIC_Stop(p_AT24Dev->IIC_Bus);  // 产生一个停止条件
}

/**
 * @brief   创建AT24CXX设备
 *
 * @param   p_AT24Dev   AT24CXX存储器设备结构体指针
 */
AT24CXX_DeviceDef Create_AT24CXX(char *name, uint8_t Address, AT24CXX_EEP_TYPE EEP_TYPE, GPIO_TypeDef *IIC_SCL_Port, GPIO_TypeDef *IIC_SDA_Port, uint16_t IIC_SCL_Pin, uint16_t IIC_SDA_Pin) {
    AT24CXX_DeviceDef AT24CXX = {0};
//    IIC_BusTypeDef IIC_Bus = {0};

    // 初始化 IIC 总线
   // IIC_Bus = *Create_IIC(name, IIC_SCL_Port, IIC_SDA_Port, IIC_SCL_Pin, IIC_SDA_Pin);
    AT24CXX.IIC_Bus = Create_IIC(name, IIC_SCL_Port, IIC_SDA_Port, IIC_SCL_Pin, IIC_SDA_Pin);
//    if (AT24CXX.IIC_Bus == NULL) {
//        // 内存分配失败
//				AT24CXX.priv_data = (void *)1;	//AT24CXX.IIC_Bus内存分配失败
//        return AT24CXX;
//    }
//    *(AT24CXX.IIC_Bus) = IIC_Bus;

    AT24CXX.name = name;

    // 动态分配 EEP_TYPE
    AT24CXX.EEP_TYPE = malloc(sizeof(struct AT24CXX_Type));
    if (AT24CXX.EEP_TYPE == NULL) {
        // 内存分配失败
//        free(AT24CXX.IIC_Bus);
//        AT24CXX.IIC_Bus = NULL;
				AT24CXX.priv_data = (void *)1;	//AT24CXX.EEP_TYPE内存分配失败
        return AT24CXX;
    }

    // 根据 EEP_TYPE 填充数据
    switch (EEP_TYPE) {
        case AT24C01:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){128, 16, 8};
            break;
        case AT24C02:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){256, 32, 8};
            break;
        case AT24C04:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){512, 32, 16};
            break;
        case AT24C08:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){1024, 64, 16};
            break;
        case AT24C16:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){2048, 128, 16};
            break;
        case AT24C32:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){4096, 128, 32};
            break;
        case AT24C64:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){8192, 256, 32};
            break;
        case AT24C128:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){16384, 512, 64};
            break;
        case AT24C256:
            *(AT24CXX.EEP_TYPE) = (struct AT24CXX_Type){32768, 512, 128};
            break;
        default:
//            free(AT24CXX.EEP_TYPE);
//            free(AT24CXX.IIC_Bus);
//            AT24CXX.EEP_TYPE = NULL;
//            AT24CXX.IIC_Bus = NULL;
						AT24CXX.priv_data = (void *)1;	//AT24CXX.EEP_TYPE无此型号
            return AT24CXX;  // 错误类型
    }

    AT24CXX.Address = Address;
    AT24CXX.AT24CXX_ReadReg = AT24CXX_ReadReg;
    AT24CXX.AT24CXX_WriteReg = AT24CXX_WriteReg;
    AT24CXX.AT24CXX_ReadRegs = AT24CXX_ReadRegs;
    AT24CXX.AT24CXX_WriteRegs = AT24CXX_WriteRegs;
    AT24CXX.AT24CXX_Check = AT24CXX_Check;

    // 检查设备
    if (AT24CXX_Check(&AT24CXX) != 0) {
//        free(AT24CXX.EEP_TYPE);
//        free(AT24CXX.IIC_Bus);
//        AT24CXX.EEP_TYPE = NULL;
//        AT24CXX.IIC_Bus = NULL;
				AT24CXX.priv_data = (void *)1;	//AT24CXX检查失败
    }
	
    return AT24CXX;
}
