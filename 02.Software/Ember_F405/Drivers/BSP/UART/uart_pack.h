/********************************************************************************
    * 文件名称 ：uartpack.h
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-21
    * 功     能：UART串口封装
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL

*********************************************************************************/
/********************************************************************************
struct UART_Device *pUARTDev = GetUARTDevice("STM32_Bare_HAL_UART1");//获取外设地址指针
pUARTDev->Init(pUARTDev, 115200, 8, 'N', 1);//初始化串口
pUARTDev->Send(pUARTDev, "TEST\r\n", 8, 100);//发送数据
pUARTDev->Recv(pUARTDev, &c, 100)//接受数据
*********************************************************************************/

#ifndef _UART_PACK_H
#define _UART_PACK_H

#include <stdint.h>

typedef struct UART_Device {
    char *name; //设备名称
    int (*Init)(struct UART_Device *p_UARTDev, int baud, int datas, char parity, int stop);//初始化
    int (*Send)(struct UART_Device *p_UARTDev, uint8_t *datas, int len, int timeout_ms);//发送
    int (*Recv)(struct UART_Device *p_UARTDev, uint8_t *data, int timeout_ms);//接收 
    void *priv_data;//私有数据
	void *Channel;//串口通道
}UART_DeviceTypeDef;




struct UART_Device *GetUARTDevice(char *name);//获取外设地址指针


#endif

