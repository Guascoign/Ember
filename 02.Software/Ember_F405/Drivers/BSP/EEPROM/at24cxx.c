/********************************************************************************
    * 文件名称 ：at24cxx.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-16
    * 功     能：EEPROM驱动
*********************************************************************************/
#include "BSP/EEPROM/at24cxx.h"

static uint8_t ReadReg(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t ReadAddr)
{
    uint8_t temp = 0;
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->start((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 0);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, ReadAddr);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->restart((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 1);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    return temp;
}

static void WriteReg(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t WriteAddr, uint8_t DataToWrite)
{
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->start((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 0);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, WriteAddr);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, DataToWrite);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->stop((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
}

static uint8_t Check(AT24CXX_DeviceTyepeDef *p_AT24Dev)
{
    uint8_t temp = 0;
    temp = ReadReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_PAGESIZE - 1);
    if (temp == 0X66){return 0;}
    else 
    {
        p_AT24Dev->WriteReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_PAGESIZE - 1,0X66);
        temp = ReadReg(p_AT24Dev,p_AT24Dev->EEP_TYPE->EEP_PAGESIZE - 1);
        if (temp == 0X66){return 0;}
    }
    return 1;
}

static void WriteRegs(AT24CXX_DeviceTyepeDef *p_AT24Dev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
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
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->start((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);   // 发送起始信号
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 0);    // 发送地址 + 写命令
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
        if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
        {
            ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, PageStartAddr / 256);   // 发送高地址
            ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
        }
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, PageStartAddr % 256);   // 发送低地址
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);

        // 循环写入数据到当前页
        for(uint16_t i = 0; i < RemainingBytes; i++)
        {
            ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, pBuffer[i]);  // 发送字节
            ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
        }

        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->stop((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);  // 产生一个停止条件
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->delay_us(10000); // 写完后需要一定的时间

        // 更新要写入的数据和写入地址
        NumToWrite -= RemainingBytes;
        pBuffer += RemainingBytes;
        WriteAddr += RemainingBytes;

        // 计算下一页的起始地址和剩余可写入的字节数
        PageStartAddr += PageSize;
        RemainingBytes = PageSize;
    }

    // 写入剩余的数据（不足一页的情况）
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->start((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);   // 发送起始信号
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 0);    // 发送地址 + 写命令
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048)  // AT24C32/64/128/256地址是2个字节的
    {
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, WriteAddr / 256);   // 发送高地址
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    }
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, WriteAddr % 256);   // 发送低地址
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);

    // 循环写入剩余的数据
    for(uint16_t i = 0; i < NumToWrite; i++)
    {
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, pBuffer[i]);  // 发送字节
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    }

    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->stop((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);  // 产生一个停止条件
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->delay_us(10000); // 写完后需要一定的时间
}

void ReadRegs(struct AT24CXX_Device *p_AT24Dev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->start((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);   //发送起始信号
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 0);    //发送地址 + 写命令
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    if(p_AT24Dev->EEP_TYPE->EEP_SIZE > 2048) //AT24C32/64/128/256地址是2个字节的
    {
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, ReadAddr / 256);   //发送高地址
        ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);
    }
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, ReadAddr % 256);   //发送低地址
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);

    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->restart((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);   //发送起始信号
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->sendByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, (p_AT24Dev->Address<<1) | 1);    //发送地址 + 读命令
    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->waitAck((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);

    // 读取数据
    for(uint16_t i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->readByte((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, i == NumToRead - 1 ? 0 : 1); //读取一个字节
    }

    ((IIC_DeviceTypeDef *)p_AT24Dev->priv_data)->stop((IIC_DeviceTypeDef *)p_AT24Dev->priv_data);  // 产生一个停止条件
}

void AT24CXX_Init(AT24CXX_DeviceTyepeDef *p_AT24Dev, char *name, 
                    AT24CXX_EEP_TYPE EEP_TYPE, uint8_t Address ,
                    void *SCL_Instance ,uint16_t SCL_Pin ,
                    void *SDA_Instance ,uint16_t SDA_Pin)
{
    p_AT24Dev->name = name;
    p_AT24Dev->Address = Address;
    p_AT24Dev->priv_data = (void *)malloc(sizeof(IIC_DeviceTypeDef));
    IIC_Init((IIC_DeviceTypeDef *)p_AT24Dev->priv_data, "AT24CXX", SCL_Instance, SCL_Pin, SDA_Instance, SDA_Pin);
    // 根据 EEP_TYPE 填充数据
    switch (EEP_TYPE) {
        case AT24C01:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){128, 16, 8};
            break;
        case AT24C02:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){256, 32, 8};
            break;
        case AT24C04:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){512, 32, 16};
            break;
        case AT24C08:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){1024, 64, 16};
            break;
        case AT24C16:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){2048, 128, 16};
            break;
        case AT24C32:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){4096, 128, 32};
            break;
        case AT24C64:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){8192, 256, 32};
            break;
        case AT24C128:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){16384, 512, 64};
            break;
        case AT24C256:
            *(p_AT24Dev->EEP_TYPE) = (struct AT24CXX_Type){32768, 512, 128};
            break;
        default:
//            free(AT24CXX.EEP_TYPE);
//            free(AT24CXX.IIC_Bus);
//            AT24CXX.EEP_TYPE = NULL;
//            AT24CXX.IIC_Bus = NULL;
                        p_AT24Dev->priv_data = (void *)1;	//AT24CXX.EEP_TYPE无此型号
    }
    p_AT24Dev->ReadReg = ReadReg;
    p_AT24Dev->WriteReg = WriteReg;
    p_AT24Dev->ReadRegs = ReadRegs;
    p_AT24Dev->WriteRegs = WriteRegs;
    p_AT24Dev->Check = Check;
}
