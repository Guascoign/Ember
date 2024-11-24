/********************************************************************************
    * 文件名称 ：uartpack.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：UART串口封装
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL

*********************************************************************************/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"
#include "uart_pack.h"
#include "string.h"


/**
 * @brief   HAL串口初始化
 *
 * @param   p_UARTDev    UART设备结构体指针
 * @param   baud    波特率
 * @param   datas   数据位数
 * @param   parity  奇偶校验类型
 * @param   stop    停止位数
 * @return  返回值为0表示初始化成功，否则表示失败
 */
static int STM32_Bare_HAL_Init(struct UART_Device *p_UARTDev, int baud, int datas, char parity, int stop)
{   
    // 解引用 p_UARTDev 指针并设置串口初始化参数
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Instance = p_UARTDev->Channel;
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = baud;
    // 根据数据位参数设置 WordLength
    switch (datas) {
        case 8:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_8B;
            break;
        case 9:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_9B;
            break;
        default:
            // 默认设置为 8 位
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_8B;
            break;
    }
    // 根据停止位参数设置 StopBits
    switch (stop) {
        case 1:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_1;
            break;
        case 2:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_2;
            break;
        default:
            // 默认设置为 1 位停止位
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_1;
            break;
    }
    // 根据奇偶校验参数设置 Parity
    switch (parity) {
        case 'N':
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_NONE;
            break;
        case 'E':
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_EVEN;
            break;
        case 'O':
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_ODD;
            break;
        default:
            // 默认设置为无校验
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_NONE;
            break;
    }
    // 其他参数保持不变
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Mode = UART_MODE_TX_RX;
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.OverSampling = UART_OVERSAMPLING_16;

    // 使用 HAL 库初始化串口
    HAL_UART_Init((UART_HandleTypeDef *)p_UARTDev->priv_data);
    
    return 0;	//HAL_OK = 0x00U
}

/**
 * @brief   从UART设备发送数据
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int STM32_Bare_HAL_Send(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
{
    return HAL_UART_Transmit((UART_HandleTypeDef *)p_UARTDev->priv_data, datas, len, timeout_ms);
}

/**
 * @brief   从UART设备接收数据
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   data        接收数据的缓冲区指针
 * @param   timeout_ms  接收超时时间（毫秒）
 * @return  返回值为HAL库函数的返回值，表示接收操作的状态
 */
static int STM32_Bare_HAL_Recv(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1, timeout_ms);
}


/**
 * @brief   UART1设备结构体
 */
extern UART_HandleTypeDef huart1;
static struct UART_Device STM32_Bare_HAL_UART1 = {
    "STM32_Bare_HAL_UART1",        // 设备名称
    STM32_Bare_HAL_Init,            // 初始化函数
    STM32_Bare_HAL_Send,            // 发送数据函数
    STM32_Bare_HAL_Recv,            // 接收数据函数
    &huart1,                        // UART_HandleTypeDef 结构体指针
    USART1,                         // USART1 通道
};

/**
 * @brief   使用中断发送数据到UART设备
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int STM32_Bare_HAL_Send_IT(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
{
    return HAL_UART_Transmit_IT((UART_HandleTypeDef *)p_UARTDev->priv_data, datas, len);
}

/**
 * @brief   使用中断从UART设备接收数据
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   data        接收数据的缓冲区指针
 * @param   timeout_ms  接收超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示接收操作的状态
 */
static int STM32_Bare_HAL_Recv_IT(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive_IT((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1);
}

/**
 * @brief   UART1中断驱动模式设备结构体
 */
extern UART_HandleTypeDef huart1;
static struct UART_Device STM32_Bare_HAL_UART1_IT = {
    "STM32_Bare_HAL_UART1_IT",     // 设备名称
    STM32_Bare_HAL_Init,            // 初始化函数
    STM32_Bare_HAL_Send_IT,         // 使用中断发送数据函数
    STM32_Bare_HAL_Recv_IT,         // 使用中断接收数据函数
    &huart1,                        // UART_HandleTypeDef 结构体指针
    USART1,                         // USART1 通道
};

/**
 * @brief   使用中断发送数据到UART设备
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int STM32_Bare_HAL_Send_DMA(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
{
    return HAL_UART_Transmit_DMA((UART_HandleTypeDef *)p_UARTDev->priv_data, datas, len);
}

/**
 * @brief   使用中断从UART设备接收数据
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   data        接收数据的缓冲区指针
 * @param   timeout_ms  接收超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示接收操作的状态
 */
static int STM32_Bare_HAL_Recv_DMA(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive_DMA((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1);
}

/**
 * @brief   UART1中断驱动模式设备结构体
 */
extern UART_HandleTypeDef huart1;
static struct UART_Device STM32_Bare_HAL_UART1_DMA = {
    .name = "STM32_Bare_HAL_UART1_DMA",     // 设备名称
    .Init = STM32_Bare_HAL_Init,            // 初始化函数
    .Send = STM32_Bare_HAL_Send_DMA,         // 使用中断发送数据函数
    .Recv = STM32_Bare_HAL_Recv_DMA,         // 使用中断接收数据函数
    &huart1,                        // UART_HandleTypeDef 结构体指针
    USART1,                         // USART1 通道
};


/**
 * @brief   UART设备数组，包含普通模式和中断驱动模式的UART设备
 */
struct UART_Device *uart_devices[] = {
    &STM32_Bare_HAL_UART1,      // 普通模式UART1设备
    &STM32_Bare_HAL_UART1_IT,    // 中断驱动模式UART1设备
	&STM32_Bare_HAL_UART1_DMA    // DMA驱动模式UART1设备
};

/**
 * @brief   根据设备名称获取对应的UART设备指针
 *
 * @param   name    设备名称
 * @return  返回对应设备名称的UART设备指针，如果找不到则返回NULL
 */
struct UART_Device *GetUARTDevice(char *name)
{
    int i = 0;
    for (i = 0; i < sizeof(uart_devices)/sizeof(uart_devices[0]); i++)
    {
        if (0 == strcmp(name, uart_devices[i]->name))
            return uart_devices[i];
    }

    return NULL;
}

