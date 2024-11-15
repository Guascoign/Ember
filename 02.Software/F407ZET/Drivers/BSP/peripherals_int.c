/********************************************************************************
    * 文件名称 ：peripherals_int.c
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：外设初始化
*********************************************************************************
V1.0 2024-11-15 First release @ZM
*********************************************************************************/
#include "peripherals_int.h"

void peripherals_int()
{

    //串口初始化
    pUARTDev = GetUARTDevice("STM32_Bare_HAL_UART1_IT");//获取外设地址指针
    pUARTDev->Init(pUARTDev, 115200, 8, 'N', 1);



    //AT24C02初始化
    pAT24Dev = AT24CXX_GetDevice("AT24C02");//获取外设地址指针
    pAT24Dev->AT24CXX_Init(pAT24Dev);//初始化AT24CXX


}
