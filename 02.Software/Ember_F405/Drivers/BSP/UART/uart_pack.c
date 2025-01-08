/********************************************************************************
    * 文件名称 ：uartpack.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：UART串口封装
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL
V1.1 2025-01-08 更换为UART设备结构体 暂只支持阻塞
*********************************************************************************/

#include "BSP/UART/uart_pack.h"

#ifdef USE_HAL
/**
 * @brief   从UART设备发送数据
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int Send(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
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
static int Recv(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1, timeout_ms);
}

/**
 * @brief   使用中断发送数据到UART设备
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int Send_IT(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
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
static int Recv_IT(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive_IT((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1);
}

/**
 * @brief   使用中断发送数据到UART设备
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   datas       待发送数据的缓冲区指针
 * @param   len         待发送数据的长度
 * @param   timeout_ms  发送超时时间（毫秒）（此参数暂时未使用）
 * @return  返回值为HAL库函数的返回值，表示发送操作的状态
 */
static int Send_DMA(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms)
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
static int Recv_DMA(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms)
{
   return HAL_UART_Receive_DMA((UART_HandleTypeDef *)p_UARTDev->priv_data, data, 1);
}

/**
 * @brief   初始化UART设备
 *
 * @param   p_UARTDev        UART设备结构体指针
 * @param   Instance        UART外设实例基地址
 * @param   rate            波特率
 * @param   wordlength      数据位数
 * @param   parity          奇偶校验类型
 * @param   stop            停止位数
 * @return  返回值为0表示初始化成功，其他值表示初始化失败
 */
static int Init(struct UART_Device *p_UARTDev, void *Instance, UART_RateTypeDef rate, UART_WordLengthTypeDef wordlength, UART_ParityTypeDef parity, UART_StopBitsTypeDef stop) {
    // 检查参数是否有效
    if (p_UARTDev == NULL || Instance == NULL) {
        return 2; // 参数错误
    }

    // 分配或清零内存
    if (p_UARTDev->priv_data == NULL) {
        p_UARTDev->priv_data = (void *)malloc(sizeof(UART_HandleTypeDef));
        if (p_UARTDev->priv_data == NULL) {
            return 3; // 内存分配失败
        }
    } else {
        memset(p_UARTDev->priv_data, 0, sizeof(UART_HandleTypeDef));
    }

    // 设置 UART 初始化参数
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Instance = (USART_TypeDef *)Instance;
    switch (rate) {
        case Rate_115200:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 115200;
            break;
        case Rate_9600:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 9600;
            break;
        case Rate_4800:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 4800;
            break;
        case Rate_2400:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 2400;
            break;
        case Rate_1200:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 1200;
            break;
        default:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.BaudRate = 115200;
            break;
    }

    switch (wordlength) {
        case Word_length_8B:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_8B;
            break;
        case Word_length_9B:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_9B;
            break;
        default:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.WordLength = UART_WORDLENGTH_8B;
            break;
    }

    switch (stop) {
        case Stop_bits_1:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_1;
            break;
        case Stop_bits_2:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_2;
            break;
        default:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.StopBits = UART_STOPBITS_1;
            break;
    }

    switch (parity) {
        case PARITY_NONE:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_NONE;
            break;
        case PARITY_EVEN:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_EVEN;
            break;
        case PARITY_ODD:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_ODD;
            break;
        default:
            ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Parity = UART_PARITY_NONE;
            break;
    }

    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.Mode = UART_MODE_TX_RX;
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    ((UART_HandleTypeDef *)(p_UARTDev->priv_data))->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init((UART_HandleTypeDef *)p_UARTDev->priv_data) != HAL_OK) {
        free(p_UARTDev->priv_data); // 初始化失败，释放内存
        return 3; // 初始化失败
    }

    return 0; // 初始化成功
}
#endif  // #ifdef USE_HAL

#ifdef USE_LL
#endif // #ifdef USE_LL

#ifdef USE_REG
#endif // #ifdef USE_REG

/**
 * @brief   初始化UART设备
 *
 * @param   name    设备名称
 * @param   Instance    UART外设实例基地址
 * @param   Status  工作模式，'I'表示中断模式，'DMA'表示DMA模式，其他值表示普通模式
 * @param   baud    波特率
 * @param   datas   数据位数
 * @param   parity  奇偶校验类型
 * @param   stop    停止位数
 * @return  返回值为0表示初始化成功，其他值表示初始化失败
 */
int8_t UART_Init(
    UART_DeviceTypeDef *p_UARTDev,
    char *name, 
    void *Instance, 
    UART_ModeTypeDef Mode ,
    UART_RateTypeDef Rate, 
    UART_WordLengthTypeDef WordLength, 
    UART_ParityTypeDef Parity, 
    UART_StopBitsTypeDef StopBits){
    if(p_UARTDev == NULL)
        return 1; //参数错误
    p_UARTDev->name = name;
    p_UARTDev->Init = Init;
    switch (Mode){
        case UART_MODE_IT:
            p_UARTDev->Send = Send_IT;
            p_UARTDev->Recv = Recv_IT;
            break;
        case UART_MODE_DMA:
            p_UARTDev->Send = Send_DMA;
            p_UARTDev->Recv = Recv_DMA;
            break;
        default:
            p_UARTDev->Send = Send;
            p_UARTDev->Recv = Recv;
            break;
    }
    if(p_UARTDev->Init(p_UARTDev,Instance,Rate,WordLength,Parity,StopBits) != 0)
        return 2; //初始化失败
    else
    {
        Add_Device(name,p_UARTDev->priv_data,UART_DEVICE);//添加到外设列表
        return 0; //初始化成功
    }
    }
