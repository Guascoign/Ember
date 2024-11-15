/********************************************************************************
    * 文件名称 ：peripherals_int.h
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：外设初始化
*********************************************************************************
V1.0 2024-11-15 First release @ZM
*********************************************************************************/
#ifndef __PERIPHERALS_INT_H
#define __PERIPHERALS_INT_H

#include "main.h"
#include "iic.h"
#include "uart_pack.h"
#include "uart_printf.h"
#include "at24cxx.h"


struct UART_Device *pUARTDev;
struct AT24CXX_Device *pAT24Dev;

void peripherals_int(void);




#endif

