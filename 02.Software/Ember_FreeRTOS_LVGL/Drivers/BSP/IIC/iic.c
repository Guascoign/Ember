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
static void IIC_SCL(const struct IIC_Bus *p_IICDev, uint8_t state)
{
    if(state == 1)
    HAL_GPIO_WritePin(p_IICDev->IIC_SCL_Port, p_IICDev->IIC_SCL_Pin, GPIO_PIN_SET);
    if(state == 0)
    HAL_GPIO_WritePin(p_IICDev->IIC_SCL_Port, p_IICDev->IIC_SCL_Pin, GPIO_PIN_RESET);

}

/**
 * @brief   SDA引脚操作
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   state   引脚状态 0 = 低电平, 1 = 高电平
 *
 */
static void IIC_SDA(const struct IIC_Bus *p_IICDev, uint8_t state)
{
    if(state == 1)
    HAL_GPIO_WritePin(p_IICDev->IIC_SDA_Port, p_IICDev->IIC_SDA_Pin, GPIO_PIN_SET);
    if(state == 0)
    HAL_GPIO_WritePin(p_IICDev->IIC_SDA_Port, p_IICDev->IIC_SDA_Pin, GPIO_PIN_RESET);
}

/**
 * @brief   读取SDA引脚状态
 *
 * @param   p_IICDev    IIC设备结构体
 *
 */
static uint8_t IIC_READ_SDA(const struct IIC_Bus *p_IICDev)
{
   return HAL_GPIO_ReadPin(p_IICDev->IIC_SDA_Port, p_IICDev->IIC_SDA_Pin);
}

/**
 * @brief   IIC发送开始信号
 *
 * @param   p_IICDev  IIC设备结构体
 *
 */
void iic_start(const struct IIC_Bus *p_IICDev)
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
void iic_restart(const struct IIC_Bus *p_IICDev)
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
void iic_stop(const struct IIC_Bus *p_IICDev)
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
uint8_t iic_wait_ack(const struct IIC_Bus *p_IICDev)
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
void iic_ack(const struct IIC_Bus *p_IICDev)
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
void iic_nack(const struct IIC_Bus *p_IICDev)
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
void iic_send_byte(const struct IIC_Bus *p_IICDev, uint8_t Send_data)
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
uint8_t iic_receiveByte_byte(const struct IIC_Bus *p_IICDev , uint8_t ack)
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
void iic_WriteReg(const struct IIC_Bus *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
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
uint8_t iic_ReadReg(const struct IIC_Bus *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress)
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
    data = iic_receiveByte_byte(p_IICDev , 0);
    iic_nack(p_IICDev);
    iic_stop(p_IICDev);
    return data;
}

/**
 * @brief   向寄存器连续写
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 * @param   len             读取长度
 * @param   buf             读取数据存储区
 *
 */
void iic_WriteRegs(const struct IIC_Bus *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress, uint8_t len , uint8_t *pbuf)
{
    uint8_t i;
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    for(i = 0; i < len; i++)
    {
        iic_send_byte(p_IICDev, pbuf[i]);
        iic_wait_ack(p_IICDev);
    }
    iic_stop(p_IICDev);
}

/**
 * @brief   向寄存器连续读
 *
 * @param   p_IICDev    IIC设备结构体
 * @param   SlaveAddress    从设备地址
 * @param   RegAddress      寄存器地址
 * @param   len             读取长度
 * @param   buf             读取数据存储区
 *
 */
void iic_ReadRegs(const struct IIC_Bus *p_IICDev, uint8_t SlaveAddress, uint8_t RegAddress , uint8_t len , uint8_t *pbuf)
{
    uint8_t i;
    iic_start(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress);
    iic_wait_ack(p_IICDev);
    iic_send_byte(p_IICDev, RegAddress);
    iic_wait_ack(p_IICDev);
    iic_restart(p_IICDev);
    iic_send_byte(p_IICDev, SlaveAddress + 1);
    iic_wait_ack(p_IICDev);
    for(i = 0; i < len; i++)
    {
        pbuf[i] = iic_receiveByte_byte(p_IICDev , 1);
    }
    iic_nack(p_IICDev);
    iic_stop(p_IICDev);
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

/**
 * @brief   IIC初始化
 *
 * @param   IIC_Bus  IIC设备结构体
 *
 */
IIC_BusTypeDef Create_IIC(char *name , GPIO_TypeDef  *IIC_SCL_Port , GPIO_TypeDef  *IIC_SDA_Port , uint16_t IIC_SCL_Pin , uint16_t IIC_SDA_Pin)
{
    IIC_BusTypeDef IIC_Bus;
    GPIO_InitTypeDef GPIO_Initure;

    IIC_Bus.name = name; // 设备名称
    IIC_Bus.IIC_SCL_Port = IIC_SCL_Port; // SCL引脚所在的GPIO组
    IIC_Bus.IIC_SCL_Pin = IIC_SCL_Pin; // SCL引脚编号
    IIC_Bus.IIC_SDA_Port = IIC_SDA_Port; // SDA引脚所在的GPIO组
    IIC_Bus.IIC_SDA_Pin = IIC_SDA_Pin; // SDA引脚编号
    IIC_Bus.IIC_Start = iic_start; // 发送IIC开始信号
    IIC_Bus.IIC_Restart = iic_restart; // 发送IIC重新开始信号
    IIC_Bus.IIC_Stop = iic_stop; // 发送IIC停止信号
    IIC_Bus.IIC_Ack = iic_ack; // IIC发送ACK信号
    IIC_Bus.IIC_Nack = iic_nack; // IIC不发送ACK信号
    IIC_Bus.IIC_WaitAck = iic_wait_ack; // IIC等待ACK信号
    IIC_Bus.IIC_SendByte = iic_send_byte; // IIC发送一个字节
    IIC_Bus.IIC_ReceiveByte = iic_receiveByte_byte; // IIC读取一个字节
    IIC_Bus.IIC_WriteReg = iic_WriteReg;
    IIC_Bus.IIC_ReadReg = iic_ReadReg;
    IIC_Bus.IIC_WriteRegs = iic_WriteRegs;
    IIC_Bus.IIC_ReadRegs = iic_ReadRegs;
    IIC_Bus.delay_us = delay_us; // 延时函数
    IIC_Bus.priv_data = NULL; // 私有数据

    //根据GPIO组初始化GPIO时钟
    if(IIC_SCL_Port == GPIOA || IIC_SDA_Port == GPIOA)
    {
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOA时钟
    }
    if(IIC_SCL_Port == GPIOB || IIC_SDA_Port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    }
    if(IIC_SCL_Port == GPIOC || IIC_SDA_Port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    }
    if(IIC_SCL_Port == GPIOD || IIC_SDA_Port == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    }
    if(IIC_SCL_Port == GPIOE || IIC_SDA_Port == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();   //使能GPIOE时钟
    } 
    if(IIC_SCL_Port == GPIOH || IIC_SDA_Port == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();   //使能GPIOH时钟
    }     

    //GPIO_SCL初始化设置
    GPIO_Initure.Pin=IIC_SCL_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速
    HAL_GPIO_Init(IIC_SCL_Port,&GPIO_Initure);
    //GPIO_SDA初始化设置
    GPIO_Initure.Pin=IIC_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速
    HAL_GPIO_Init(IIC_SDA_Port,&GPIO_Initure);

    //SCL与SDA的初始化均为高电平
    IIC_SCL(&IIC_Bus, 1);
    IIC_SDA(&IIC_Bus, 1);
    return IIC_Bus;
}
