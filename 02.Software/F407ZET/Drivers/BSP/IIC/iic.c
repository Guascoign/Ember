/********************************************************************************
    * 文件名称 ：iic.c
    * 版     本：V1.1
    * 编写日期 ：2024-05-09
    * 功     能：IIC驱动
*********************************************************************************
V1.0 2024-01-30 First release @ZM
V1.1 2024-05-09 Add IIC_TypeDef @ZM
*********************************************************************************/
#include "iic.h"
#include "main.h"
#include "gpio.h"
#include "string.h"

/**
 * @brief   SCL引脚操作
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   state   引脚状态 0 = 低电平, 1 = 高电平
 *
 */
static void IIC_SCL(const struct IIC_Device *p_IICDev, uint8_t state)
{
    if(state == 1)
    HAL_GPIO_WritePin(p_IICDev->SCL_Port, p_IICDev->SCL_Pin, GPIO_PIN_SET);
    if(state == 0)
    HAL_GPIO_WritePin(p_IICDev->SCL_Port, p_IICDev->SCL_Pin, GPIO_PIN_RESET);

}

/**
 * @brief   SDA引脚操作
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   state   引脚状态 0 = 低电平, 1 = 高电平
 *
 */
static void IIC_SDA(const struct IIC_Device *p_IICDev, uint8_t state)
{
    if(state == 1)
    HAL_GPIO_WritePin(p_IICDev->SDA_Port, p_IICDev->SDA_Pin, GPIO_PIN_SET);
    if(state == 0)
    HAL_GPIO_WritePin(p_IICDev->SDA_Port, p_IICDev->SDA_Pin, GPIO_PIN_RESET);
}

/**
 * @brief   读取SDA引脚状态
 *
 * @param   p_IICDev    IIC设备结构体
 *
 */
static uint8_t IIC_READ_SDA(const struct IIC_Device *p_IICDev)
{
   return HAL_GPIO_ReadPin(p_IICDev->SDA_Port, p_IICDev->SDA_Pin);
}


/**
 * @brief   IIC初始化
 *
 * @param   IIC_Device  IIC设备结构体
 *
 */
void iic_init(const struct IIC_Device *p_IICDev)
{
    GPIO_InitTypeDef GPIO_Initure;

    //根据GPIO组初始化GPIO时钟
    if(p_IICDev->SCL_Port == GPIOA || p_IICDev->SDA_Port == GPIOA)
    {
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOA时钟
    }
    if(p_IICDev->SCL_Port == GPIOB || p_IICDev->SDA_Port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    }
    if(p_IICDev->SCL_Port == GPIOC || p_IICDev->SDA_Port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    }
    if(p_IICDev->SCL_Port == GPIOD || p_IICDev->SDA_Port == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    }
    if(p_IICDev->SCL_Port == GPIOE || p_IICDev->SDA_Port == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();   //使能GPIOE时钟
    } 
    if(p_IICDev->SCL_Port == GPIOH || p_IICDev->SDA_Port == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();   //使能GPIOH时钟
    }     

    //GPIO_SCL初始化设置
    GPIO_Initure.Pin=p_IICDev->SCL_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速
    HAL_GPIO_Init(p_IICDev->SCL_Port,&GPIO_Initure);
    //GPIO_SDA初始化设置
    GPIO_Initure.Pin=p_IICDev->SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速
    HAL_GPIO_Init(p_IICDev->SDA_Port,&GPIO_Initure);

    //SCL与SDA的初始化均为高电平
    IIC_SCL(p_IICDev, 1);
    IIC_SDA(p_IICDev, 1);
}

/**
 * @brief   IIC反初始化
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_deinit(const struct IIC_Device *p_IICDev)
{
    // 关闭与 GPIO 相关的时钟
    if(p_IICDev->SCL_Port == GPIOA || p_IICDev->SDA_Port == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    }
    if(p_IICDev->SCL_Port == GPIOB || p_IICDev->SDA_Port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_DISABLE();
    }
    if(p_IICDev->SCL_Port == GPIOC || p_IICDev->SDA_Port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_DISABLE();
    }
    if(p_IICDev->SCL_Port == GPIOD || p_IICDev->SDA_Port == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_DISABLE();
    }
    if(p_IICDev->SCL_Port == GPIOE || p_IICDev->SDA_Port == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_DISABLE();
    } 
    if(p_IICDev->SCL_Port == GPIOH || p_IICDev->SDA_Port == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_DISABLE();
    }     
}

/**
 * @brief   IIC发送开始信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_start(const struct IIC_Device *p_IICDev)
{
    IIC_SDA(p_IICDev, 1);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SDA(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
}

/**
 * @brief   IIC发送重新开始信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_restart(const struct IIC_Device *p_IICDev)
{
    IIC_SDA(p_IICDev, 1);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay+2);
    IIC_SDA(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay+2);
    IIC_SCL(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
}

/**
 * @brief   IIC发送停止信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_stop(const struct IIC_Device *p_IICDev)
{
    IIC_SDA(p_IICDev, 0);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SDA(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
}

/**
 * @brief   IIC等待应答信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 * @retval  1，接收应答失败
 *          0，接收应答成功
 */
uint8_t iic_wait_ack(const struct IIC_Device *p_IICDev)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);

    while (IIC_READ_SDA(p_IICDev))    /* 等待应答 */
    {
        waittime++;

        if (waittime > 250)
        {
            iic_stop(p_IICDev);
            rack = 1;
            break;
        }
    }

    IIC_SCL(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
    return rack;
}

/**
 * @brief   IIC发送应答信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_ack(const struct IIC_Device *p_IICDev)
{
    IIC_SDA(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SDA(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
}

/**
 * @brief   IIC发送不应答信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_nack(const struct IIC_Device *p_IICDev)
{
    IIC_SDA(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 1);
    p_IICDev->delay_us(IIC_Delay);
    IIC_SCL(p_IICDev, 0);
    p_IICDev->delay_us(IIC_Delay);
}

/**
 * @brief   IIC发送一个字节
 *
 * @param   p_IICDev  IIC设备结构体
 * @param   Send_data   发送的数据
 *
 */
void iic_send_byte(const struct IIC_Device *p_IICDev, uint8_t Send_data)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        IIC_SDA(p_IICDev, (Send_data & 0x80) >> 7);
        Send_data <<= 1;
        p_IICDev->delay_us(IIC_Delay);
        IIC_SCL(p_IICDev, 1);
        p_IICDev->delay_us(IIC_Delay);
        IIC_SCL(p_IICDev, 0);
        p_IICDev->delay_us(IIC_Delay);
    }
}

/**
 * @brief   IIC读取一个字节
 *
 * @param   p_IICDev  IIC设备结构体
 *
 * @retval  读取到的数据
 */
uint8_t iic_read_byte(const struct IIC_Device *p_IICDev , uint8_t ack)
{
    uint8_t i;
    uint8_t receive = 0;

    IIC_SDA(p_IICDev, 1);

    for (i = 0; i < 8; i++)
    {
        IIC_SCL(p_IICDev, 1);
        p_IICDev->delay_us(IIC_Delay);
        receive <<= 1;

        if (IIC_READ_SDA(p_IICDev))
        {
            receive++;
        }

        IIC_SCL(p_IICDev, 0);
        p_IICDev->delay_us(IIC_Delay);
    }

    if (ack)
    {
        iic_ack(p_IICDev);
    }
    else
    {
        iic_nack(p_IICDev);
    }

    return receive;
}

/**
 * @brief   IIC写入一个字节
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 * @param   ByteData        写入的数据
 *
 */
void iic_WriteRegByte(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
{
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, ByteData);
    iic_wait_ack(p_IICDev);
    iic_stop(p_IICDev);
}

/**
 * @brief   IIC读取一个字节
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 *
 * @retval  读取到的数据
 */
uint8_t iic_ReadRegByte(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress)
{
    uint8_t data;
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_restart(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress + 1);
    iic_wait_ack(p_IICDev);
    data = iic_read_byte(p_IICDev , 0);
    iic_nack(p_IICDev);
    iic_stop(p_IICDev);
    return data;
}

/**
 * @brief   IIC写入两个字节
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 * @param   ByteData        写入的数据
 *
 */
void iic_WriteReg2Byte(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint16_t ByteData)
{
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, ByteData >> 8);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, ByteData);
    iic_wait_ack(p_IICDev);
    iic_stop(p_IICDev);
}

/**
 * @brief   IIC读取两个字节
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 *
 * @retval  读取到的数据
 */
uint16_t iic_ReadReg2Byte(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress)
{
    uint16_t data;
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_restart(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress + 1);
    iic_wait_ack(p_IICDev);
    data = iic_read_byte(p_IICDev , 1) << 8;
    iic_ack(p_IICDev);
    data |= iic_read_byte(p_IICDev , 0);
    iic_nack(p_IICDev);
    iic_stop(p_IICDev);
    return data;
}

/**
 * @brief   IIC写入一个字节(CRC8)
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 * @param   ByteData        写入的数据
 *
 */
void iic_WriteRegByte_CRC8(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
{
    uint8_t CRC_buf[3];
    CRC_buf[0] = SlaveAddress<<1 | 0;
    CRC_buf[1] = RegAddress;
    CRC_buf[2] = ByteData;
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress<<1 | 0);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, ByteData);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, CRC8(CRC_buf,3) );
    iic_wait_ack(p_IICDev);
    iic_stop(p_IICDev);
}

/**
 * @brief   IIC读取一个字节(CRC8)   SH367303 读2字节，丢1字节
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 *
 * @retval  读取到的数据
 */
uint8_t iic_ReadRegByte_CRC8(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress)
{
    uint8_t data;
    uint8_t CRC_buf[6];
    CRC_buf[0] = SlaveAddress<<1 | 0;
    CRC_buf[1] = RegAddress;
    CRC_buf[2] = SlaveAddress<<1 | 1;

    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress<<1 | 0);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress<<1 | 1);
    iic_wait_ack(p_IICDev);
    CRC_buf[3] = iic_read_byte(p_IICDev , 1);   //高8位
    CRC_buf[4] = iic_read_byte(p_IICDev , 1);   //低8位
    CRC_buf[5] = iic_read_byte(p_IICDev , 0);   //CRC
    iic_stop(p_IICDev);

    if(CRC8(CRC_buf, 5) == CRC_buf[5])
    {
        data = CRC_buf[3];
    }
    else
    {
        data = 0xff;
        #ifdef LOG_OUTPUT
        printf("IIC CRC Failed\n");
        #endif
    }
    return data;
}

/**
 * @brief   IIC读取两个字节(CRC8)
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 *
 * @retval  读取到的数据
 */
uint16_t iic_ReadReg2Byte_CRC8(const struct IIC_Device *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress)
{
    uint16_t data;
    uint8_t CRC_buf[6];
    CRC_buf[0] = SlaveAddress<<1 | 0;
    CRC_buf[1] = RegAddress;
    CRC_buf[2] = SlaveAddress<<1 | 1;

    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress<<1 | 0);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress<<1 | 1);
    iic_wait_ack(p_IICDev);
    CRC_buf[3] = iic_read_byte(p_IICDev , 1);   //高8位
    CRC_buf[4] = iic_read_byte(p_IICDev , 1);   //低8位
    CRC_buf[5] = iic_read_byte(p_IICDev , 0);   //CRC
    iic_stop(p_IICDev);

    if(CRC8(CRC_buf, 5) == CRC_buf[5])
    {
        data = (CRC_buf[3] << 8) | CRC_buf[4];
    }
    else
    {
        data = 0xffff;
        #ifdef LOG_OUTPUT
        printf("IIC CRC Failed\n");
        #endif
    }
    return data;
}


/**
 * @brief   us延时函数
 *
 * @param   nus 延时时间
 *
 */
void delay_us(uint32_t nus)
{
    uint32_t Delay = nus * 168/4;
    do
    {
        __NOP();
    }
    while (Delay --);
}

/**
 * @brief   IIC延时函数,用于控制IIC读写速度
 *
 * @param   delay   延时时间
 *
 */
void iic_delay(int delay)
{
    delay_us(delay);
}

//IIC1初始化
struct IIC_Device IIC1 = {
    .name = "SH367303_IIC", // 设备名称
    .SCL_Port = IIC2_SCL_GPIO_Port, // SCL引脚所在的GPIO组
    .SCL_Pin = IIC2_SCL_Pin, // SCL引脚编号
    .SDA_Port = IIC2_SDA_GPIO_Port, // SDA引脚所在的GPIO组
    .SDA_Pin = IIC2_SDA_Pin, // SDA引脚编号
    .IIC_Init = iic_init, // 初始化IIC
    .IIC_Deint = iic_deinit, // 反初始化IIC
    .IIC_Start = iic_start, // 发送IIC开始信号
    .IIC_Restart = iic_restart, // 发送IIC重新开始信号
    .IIC_Stop = iic_stop, // 发送IIC停止信号
    .IIC_Ack = iic_ack, // IIC发送ACK信号
    .IIC_Nack = iic_nack, // IIC不发送ACK信号
    .IIC_WaitAck = iic_wait_ack, // IIC等待ACK信号
    .IIC_SendByte = iic_send_byte, // IIC发送一个字节
    .IIC_ReadByte = iic_read_byte, // IIC读取一个字节
    .IIC_ReadRegByte = iic_ReadRegByte_CRC8, // 从寄存器读取一个字节
    .IIC_WriteRegByte = iic_WriteRegByte_CRC8, // 向寄存器写入一个字节
    .IIC_ReadReg2Byte = iic_ReadReg2Byte_CRC8, // 从寄存器读取两个字节
    .IIC_WriteReg2Byte = NULL, // 向寄存器写入两个字节
    .delay_us = delay_us, // 延时函数
    .priv_data = NULL // 私有数据
};

//IIC2初始化
struct IIC_Device IIC2 = {
    .name = "AT24C64_IIC", // 设备名称
    .SCL_Port = IIC1_SCL_GPIO_Port, // SCL引脚所在的GPIO组
    .SCL_Pin = IIC1_SCL_Pin, // SCL引脚编号
    .SDA_Port = IIC1_SDA_GPIO_Port, // SDA引脚所在的GPIO组
    .SDA_Pin = IIC1_SDA_Pin, // SDA引脚编号
    .IIC_Init = iic_init, // 初始化IIC
    .IIC_Deint = iic_deinit, // 反初始化IIC
    .IIC_Start = iic_start, // 发送IIC开始信号
    .IIC_Restart = iic_restart, // 发送IIC重新开始信号
    .IIC_Stop = iic_stop, // 发送IIC停止信号
    .IIC_Ack = iic_ack, // IIC发送ACK信号
    .IIC_Nack = iic_nack, // IIC不发送ACK信号
    .IIC_WaitAck = iic_wait_ack, // IIC等待ACK信号
    .IIC_SendByte = iic_send_byte, // IIC发送一个字节
    .IIC_ReadByte = iic_read_byte, // IIC读取一个字节
    .IIC_ReadRegByte = iic_ReadRegByte, // 从寄存器读取一个字节
    .IIC_WriteRegByte = iic_WriteRegByte, // 向寄存器写入一个字节
    .IIC_ReadReg2Byte = iic_ReadReg2Byte, // 从寄存器读取两个字节
    .IIC_WriteReg2Byte = iic_WriteReg2Byte, // 向寄存器写入两个字节
    .delay_us = delay_us, // 延时函数
    .priv_data = NULL // 私有数据
};

/**
 * @brief   IIC设备数组
 */
struct IIC_Device *iic_devices[] = {
     &IIC1,  //SH367303
     &IIC2   //AT24C64
};

/**
 * @brief   获取IIC设备结构体
 *
 * @param   name    设备名称
 *
 * @retval  IIC设备结构体
 */
struct IIC_Device *GetIICDevice(char *name)
{
    int i;
    for (i = 0; i < sizeof(iic_devices)/sizeof(iic_devices[0]); i++)
    {
        if (strcmp(name, iic_devices[i]->name) == 0)
        {
            return iic_devices[i];
        }
    }
    return NULL;
}





// /************************ V1.0代码*************************/
// /**
//  * @brief       IIC写入一个字节
//  * @param       无
//  * @retval      无
//  */
// void iic_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
// {
// 	iic_start();
// 	iic_send_byte(SlaveAddress);
// 	iic_wait_ack();
// 	iic_send_byte(RegAddress);
// 	iic_wait_ack();
// 	iic_send_byte(ByteData);
// 	iic_wait_ack();
// 	iic_stop();
// }
// /**
//  * @brief       IIC读出一个字节
//  * @param       无
//  * @retval      无
//  */
// uint8_t iic_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress)
// {
// 	uint8_t data;
// 	iic_start();
// 	iic_send_byte(SlaveAddress);
// 	iic_wait_ack();
// 	iic_send_byte(RegAddress);
// 	iic_wait_ack();
// 	iic_start();
// 	iic_send_byte(SlaveAddress + 1);
// 	iic_wait_ack();
// 	data = iic_read_byte(0);
// 	iic_nack();
// 	iic_stop();
// 	return data;
// }

// /**
//  * @brief       us延时函数
//  * @param       无
//  * @retval      无
//  */
// void delay_us(uint32_t nus)
// {
//     uint32_t Delay = nus * 168/4;
//     do
//     {
//         __NOP();
//     }
//     while (Delay --);
// }

// /**
//  * @brief       IIC延时函数,用于控制IIC读写速度
//  * @param       无
//  * @retval      无
//  */
// void iic_delay(int delay)
// {
// 	//AL_Delay(1);    /* 2us的延时, 读写速度在250Khz以内 */
// 	//delay_us(delay);
// 	delay_us(delay);
// }

// /**
//  * @brief       产生IIC起始信号
//  * @param       无
//  * @retval      无
//  */
// void iic_start(void)
// {
//     IIC_SDA(1);
//     IIC_SCL(1);
//     iic_delay(IIC_Delay);
//     IIC_SDA(0);     /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
//     iic_delay(IIC_Delay);
//     IIC_SCL(0);     /* 钳住iic总线，准备发送或接收数据 */
//     iic_delay(IIC_Delay);
// }

// /**
//  * @brief       产生IIC起始信号
//  * @param       无
//  * @retval      无
//  */
// void iic_restart(void)
// {
//     IIC_SDA(1);
//     IIC_SCL(1);
//     iic_delay(IIC_Delay+2);
//     IIC_SDA(0);     /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
//     iic_delay(IIC_Delay+2);
//     IIC_SCL(0);     /* 钳住iic总线，准备发送或接收数据 */
//     iic_delay(IIC_Delay);
// }

// /**
//  * @brief       产生IIC停止信号
//  * @param       无
//  * @retval      无
//  */
// void iic_stop(void)
// {
// 	IIC_SDA(0);
//     IIC_SCL(1);
// 		iic_delay(IIC_Delay);
//      /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
//     iic_delay(IIC_Delay);
//     IIC_SDA(1);     /* 发送iic总线结束信号 */
//     iic_delay(IIC_Delay);
// }

// void iic_init(void)
// {
// 	IIC_SDA(1);
//     IIC_SCL(1);
// }

// /**
//  * @brief       等待应答信号到来
//  * @param       无
//  * @retval      1，接收应答失败
//  *              0，接收应答成功
//  */
// uint8_t iic_wait_ack(void)
// {
//     uint8_t waittime = 0;
//     uint8_t rack = 0;

//     IIC_SDA(1);     /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
//     iic_delay(IIC_Delay);
//     IIC_SCL(1);     /* SCL=1, 此时从机可以返回ACK */
//     iic_delay(IIC_Delay);

//     while (IIC_READ_SDA)    /* 等待应答 */
//     {
//         waittime++;

//         if (waittime > 250)
//         {
//             iic_stop();
//             rack = 1;
//             break;
//         }
//     }

//     IIC_SCL(0);     /* SCL=0, 结束ACK检查 */
//     iic_delay(IIC_Delay);
//     return rack;
// }

// /**
//  * @brief       产生ACK应答
//  * @param       无
//  * @retval      无
//  */
// void iic_ack(void)
// {
//     IIC_SDA(0);     /* SCL 0 -> 1  时 SDA = 0,表示应答 */
//     iic_delay(IIC_Delay);
//     IIC_SCL(1);     /* 产生一个时钟 */
//     iic_delay(IIC_Delay);
//     IIC_SCL(0);
//     iic_delay(IIC_Delay);
//     IIC_SDA(1);     /* 主机释放SDA线 */
//     iic_delay(IIC_Delay);
// }

// /**
//  * @brief       不产生ACK应答
//  * @param       无
//  * @retval      无
//  */
// void iic_nack(void)
// {
//     IIC_SDA(1);     /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
//     iic_delay(IIC_Delay);
//     IIC_SCL(1);     /* 产生一个时钟 */
//     iic_delay(IIC_Delay);
//     IIC_SCL(0);
//     iic_delay(IIC_Delay);
// }

// /**
//  * @brief       IIC发送一个字节
//  * @param       data: 要发送的数据
//  * @retval      无
//  */
// void iic_send_byte(uint8_t data)
// {
//     uint8_t t;
    
//     for (t = 0; t < 8; t++)
//     {
//         IIC_SDA((data & 0x80) >> 7);    /* 高位先发送 */
//         iic_delay(IIC_Delay);
//         IIC_SCL(1);
//         iic_delay(IIC_Delay);
//         IIC_SCL(0);
//         data <<= 1;     /* 左移1位,用于下一次发送 */
//     }
//     IIC_SDA(1);         /* 发送完成, 主机释放SDA线 */
// }

// /**
//  * @brief       IIC读取一个字节
//  * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
//  * @retval      接收到的数据
//  */
// uint8_t iic_read_byte(uint8_t ack)
// {
//     uint8_t i, receive = 0;

//     for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
//     {
//         receive <<= 1;  /* 高位先输出,所以先收到的数据位要左移 */
//         IIC_SCL(1);
//         iic_delay(IIC_Delay);

//         if (IIC_READ_SDA)
//         {
//             receive++;
//         }
        
//         IIC_SCL(0);
//         iic_delay(IIC_Delay);
//     }

//     if (!ack)
//     {
//         iic_nack();     /* 发送nACK */
//     }
//     else
//     {
//         iic_ack();      /* 发送ACK */
//     }

//     return receive;
// }

// void iic_WriteRegByte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData) //通过iic向设备寄存器写一个字节(CRC校验)
// {
//     uint8_t CRC_buf[3];
//     CRC_buf[0] = (SlaveAddress<<1 | 0);
//     CRC_buf[1] = RegAddress;
//     CRC_buf[2] = ByteData;
// 	iic_start();
// 	iic_send_byte((SlaveAddress<<1) | 0);
// 	iic_wait_ack();
// 	iic_send_byte(RegAddress);
// 	iic_wait_ack();
// 	iic_send_byte(ByteData);
// 	iic_wait_ack();
// 	iic_send_byte(CRC8(CRC_buf,3));
// 	iic_wait_ack();
// 	iic_stop();
// }


// uint8_t CRC8(uint8_t *data , uint8_t len)
// {
    
// 	uint8_t crc8 = 0;

//     while (len --)
//     {
//         crc8 = crc8 ^ (*data++);
//         crc8 = crc8Table[crc8];
//     }

//     return crc8;
// }



// uint16_t iic_ReadReg2Byte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress) //通过iic从设备寄存器读两个字节(CRC校验)
// {
//     uint8_t CRC_buf[6];
//     uint16_t data;
//     CRC_buf[0] = (SlaveAddress<<1);
//     CRC_buf[1] = RegAddress;
//     CRC_buf[2] = (SlaveAddress<<1)+1;
	
//     iic_start();
// 	iic_send_byte((SlaveAddress<<1));
// 	iic_wait_ack();
// 	iic_send_byte(RegAddress);
// 	iic_wait_ack();	
// 	iic_start();		
//     iic_send_byte((SlaveAddress<<1)+1);
// 	iic_wait_ack();
//     CRC_buf[3] = iic_read_byte(1);      //高8位
//     CRC_buf[4] = iic_read_byte(1);      //低八位
// 	CRC_buf[5] = iic_read_byte(0);      //CRC
//     iic_stop();

// if( CRC8(CRC_buf,5) == CRC_buf[5])
// {
// data = (CRC_buf[3] <<8 )| CRC_buf[4];
// }
// else 
//    {
//        data = 0xffff;
//         #ifdef LOG_OUTPUT
//         printf("IIC CRC Failed\n");
//          #endif
//    }
// return data;
// }


// uint8_t iic_ReadRegByte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress) //通过iic从设备寄存器读一个字节
// {
//     uint8_t CRC_buf[6];
//     uint8_t data;
//     CRC_buf[0] = (SlaveAddress<<1);
//     CRC_buf[1] = RegAddress;
//     CRC_buf[2] = (SlaveAddress<<1)+1;
//     iic_start();
// 	iic_send_byte((SlaveAddress<<1));
// 	iic_wait_ack();
// 	iic_send_byte(RegAddress);
// 	iic_wait_ack();	
// 	iic_start();
//   iic_send_byte((SlaveAddress<<1)+1);
// 	iic_wait_ack();
//    CRC_buf[3] = iic_read_byte(1);      //高8位
//    CRC_buf[4] = iic_read_byte(1);      //低八位
// 	CRC_buf[5] = iic_read_byte(0);      //CRC
//     iic_stop();
//    if( CRC8(CRC_buf,5) == CRC_buf[5])
//    {
//        data = CRC_buf[3];
//    }
//    else 
//    {
//        data = 0xff;
//         #ifdef LOG_OUTPUT
//         printf("IIC CRC Failed\n");
//          #endif
//    }
//     return data;
// }





