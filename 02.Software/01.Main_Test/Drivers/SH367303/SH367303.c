/********************************************************************************
    * 文件名称 ：SH36730X.h
    * 作     者：ZM
    * 版     本：V1.1
    * 编写日期 ：2024-04-15
    * 功     能：SH36730X 驱动代码
*********************************************************************************/
#include "SH367303.h"

/**
 * @brief   初始化 SH36730X
 *
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_Init(void)
{

SH367303_SCONF_TypeDef SH367303_SCONF_Struct = {0};
SH367303_INT_EN_TypeDef SH367303_INT_EN_Struct = {0};
    /****启动 SH36730X...****/
    //->设置 CHGD 为低  SH退出低功耗状态 输出V33
    //->设置 DSGD 为高  SH放电PWM驱动启动
//	HAL_GPIO_WritePin(SH_CHGD_GPIO_Port,SH_CHGD_Pin,GPIO_PIN_RESET);  
//	HAL_GPIO_WritePin(SH_DSGD_GPIO_Port,SH_DSGDP_Pin,GPIO_PIN_SET);

    //配置 SH36730X 硬件参数选项
    //SCONF1
    SH367303_SCONF_Struct.LTCLR = SH_LTCLR_EN;
    SH367303_SCONF_Struct.CTLD_EN = SH_CTLD_EN;
    SH367303_SCONF_Struct.PD_EN = SH_PD_DIS;
    SH367303_SCONF_Struct.WDT_EN = SH_WDT_DIS;
    SH367303_SCONF_Struct.SC_EN = SH_SC_EN ;
    SH367303_SCONF_Struct.OV_EN = SH_OV_EN;
    SH367303_SCONF_Struct.LOAD_EN = SH_LOAD_EN;
    SH367303_SCONF_Struct.CHGR_EN = SH_CHGR_EN;
    //SCONF2
    SH367303_SCONF_Struct.RESET_PF = SH_RESET_PF_EN;
    SH367303_SCONF_Struct.ALARM_C = SH_ALARM_EN;
    SH367303_SCONF_Struct.DSG_C = SH_DSG_DIS;
    SH367303_SCONF_Struct.CHG_C = SH_CHG_DIS;
    //SCONF3
    SH367303_SCONF_Struct.CADC_EN = SH_CADC_EN;
    SH367303_SCONF_Struct.CADC_M = SH_CADC_M_SINGLE;
    SH367303_SCONF_Struct.CBIT_C = SH_CBIT_13;
    SH367303_SCONF_Struct.VADC_EN = SH_VADC_EN;
    SH367303_SCONF_Struct.VADC_C = SH_VADC_C_VT;
    SH367303_SCONF_Struct.SCAN_C = SH_SCAN_C_200mS;
    //SCONF4
    SH367303_SCONF_Struct.CB10 = SH_CB10_DIS;
    SH367303_SCONF_Struct.CB9 = SH_CB9_DIS;
    SH367303_SCONF_Struct.CB8 = SH_CB8_DIS;
    SH367303_SCONF_Struct.CB7 = SH_CB7_DIS;
    SH367303_SCONF_Struct.CB6 = SH_CB6_DIS;
    //SCONF5
    SH367303_SCONF_Struct.CB5 = SH_CB5_DIS;
    SH367303_SCONF_Struct.CB4 = SH_CB4_DIS;
    SH367303_SCONF_Struct.CB3 = SH_CB3_DIS;
    SH367303_SCONF_Struct.CB2 = SH_CB2_DIS;
    SH367303_SCONF_Struct.CB1 = SH_CB1_DIS;
    //SCONF6
    SH367303_SCONF_Struct.RSNS = SH_RSNS_50mV;
    SH367303_SCONF_Struct.RST = SH_RST_32mS;
    SH367303_SCONF_Struct.SCV = SH_SCV_400mV;
    SH367303_SCONF_Struct.SCT = SH_SCT_50uS;
    //SCONF7
    SH367303_SCONF_Struct.OVT = SH_OVT_32;
    SH367303_SCONF_Struct.CHS = SH_CHS_12mV;
    SH367303_SCONF_Struct.WDTT = SH_WDTT_30S;
    //SCONF8-9
    SH367303_SCONF_Struct.OVD = 0x02DD;  //  733/5.86 = 4.3V

    if(SH36730X_HardwareInit(&SH367303_SCONF_Struct) != SH367303_OK)
    {
        #ifdef LOG_OUTPUT
        printf("Failed to SH36730X_HardwareInit\n");
        #endif
        return SH367303_TimeOut;
    }

    //配置 SH36730X 中断屏蔽选项
    SH367303_INT_EN_Struct.SC_INT = SH_SC_INT_DIS;
    SH367303_INT_EN_Struct.OV_INT = SH_OV_INT_DIS;
    SH367303_INT_EN_Struct.CD_INT = SH_CD_INT_DIS;
    SH367303_INT_EN_Struct.CADC_INT = SH_CADC_INT_DIS;
    SH367303_INT_EN_Struct.VADC_INT = SH_VADC_INT_DIS;
    SH367303_INT_EN_Struct.WDT_INT = SH_WDT_INT_DIS;
    SH367303_INT_EN_Struct.TWI_INT = SH_TWI_INT_DIS;
    if(SH36730X_ConfigInterruptMask(&SH367303_INT_EN_Struct) != SH367303_OK)
    {
        #ifdef LOG_OUTPUT
        printf("Failed to SH36730X_HardwareInit\n");
        #endif
        return SH367303_TimeOut;
    }
    return SH367303_OK;
}

/**
 * @brief   初始化 SH36730X 硬件参数选项
 *
 * @param   SH367303_SCONF_Struct    SH367303_SCONF_TypeDef 结构体指针，包含要设置的硬件参数选项
 *
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_HardwareInit(SH367303_SCONF_TypeDef *SH367303_SCONF_Struct)
{
    uint8_t tmp;

    //SH_SCONF1
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF1) & 0x00; //读取寄存器中的保留位 XXXX XXXX
    tmp |= SH367303_SCONF_Struct->LTCLR;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->CTLD_EN;
    tmp |= SH367303_SCONF_Struct->PD_EN;
    tmp |= SH367303_SCONF_Struct->WDT_EN;
    tmp |= SH367303_SCONF_Struct->SC_EN;
    tmp |= SH367303_SCONF_Struct->OV_EN;
    tmp |= SH367303_SCONF_Struct->LOAD_EN;
    tmp |= SH367303_SCONF_Struct->CHGR_EN;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF1, tmp);    //写回寄存器    
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF1) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF1\n");
         #endif
        return SH367303_TimeOut;
    }

    //SH_SCONF2
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) & 0xF0; //读取寄存器中的保留位 1111 XXXX
    tmp |= SH367303_SCONF_Struct->RESET_PF;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->ALARM_C;
    tmp |= SH367303_SCONF_Struct->DSG_C;
    tmp |= SH367303_SCONF_Struct->CHG_C;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF2, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF2\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF3
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) & 0x00; //读取寄存器中的保留位 XXXX XXXX
    tmp |= SH367303_SCONF_Struct->CADC_EN;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->CADC_M;
    tmp |= SH367303_SCONF_Struct->CBIT_C;
    tmp |= SH367303_SCONF_Struct->VADC_EN;
    tmp |= SH367303_SCONF_Struct->VADC_C;
    tmp |= SH367303_SCONF_Struct->SCAN_C;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF3, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF3\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF4
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF4) & 0xE0; //读取寄存器中的保留位 111X XXXX
    tmp |= SH367303_SCONF_Struct->CB10;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->CB9;
    tmp |= SH367303_SCONF_Struct->CB8;
    tmp |= SH367303_SCONF_Struct->CB7;
    tmp |= SH367303_SCONF_Struct->CB6;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF4, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF4) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF4\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF5
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF5) & 0xE0; //读取寄存器中的保留位 111X XXXX
    tmp |= SH367303_SCONF_Struct->CB5;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->CB4;
    tmp |= SH367303_SCONF_Struct->CB3;
    tmp |= SH367303_SCONF_Struct->CB2;
    tmp |= SH367303_SCONF_Struct->CB1;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF5, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF5) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF5\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF6
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF6) & 0x00; //读取寄存器中的保留位 XXXX XXXX
    tmp |= SH367303_SCONF_Struct->RSNS;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->RST;
    tmp |= SH367303_SCONF_Struct->SCV;
    tmp |= SH367303_SCONF_Struct->SCT;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF6, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF6) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF6\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF7
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF7) & 0x80; //读取寄存器中的保留位 1XXX XXXX
    tmp |= SH367303_SCONF_Struct->OVT;           //装填配置参数
    tmp |= SH367303_SCONF_Struct->CHS;
    tmp |= SH367303_SCONF_Struct->WDTT;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF7, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF7) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF7\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF8
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF8) & 0xFC; //读取寄存器中的保留位 1111 11XX
    tmp |= (uint8_t)(SH367303_SCONF_Struct->OVD >> 8);           //装填配置参数 取高2位
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF8, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF8) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF8\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF9
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF9) & 0x00; //读取寄存器中的保留位 XXXX XXXX
    tmp |= (uint8_t)(SH367303_SCONF_Struct->OVD & 0xFF);           //装填配置参数 取低8位
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF9, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF9) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF9\n");
         #endif
        return SH367303_TimeOut;
    }
        
    //SH_SCONF10
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF10) & 0x00; //读取寄存器中的保留位 XXXX XXXX
    tmp |= SH367303_SCONF_Struct->PIN;           //装填配置参数
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF10, tmp);    //写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF10) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF10\n");
         #endif
        return SH367303_TimeOut;
    }

    return SH367303_OK;
}

/**
 * @brief   配置 SH36730X 中断屏蔽选项
 *
 * @param   SH367303_INT_EN_Struct    SH367303_INT_EN_TypeDef 结构体指针，包含要设置的中断屏蔽选项
 *
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_ConfigInterruptMask(SH367303_INT_EN_TypeDef *SH367303_INT_EN_Struct)
{
    uint8_t tmp;

    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_INT_EN) & 0x80; //读取寄存器中的保留位 1XXX XXXX
    tmp |= SH367303_INT_EN_Struct->SC_INT;           //装填配置参数
    tmp |= SH367303_INT_EN_Struct->OV_INT;
    tmp |= SH367303_INT_EN_Struct->CD_INT;
    tmp |= SH367303_INT_EN_Struct->CADC_INT;
    tmp |= SH367303_INT_EN_Struct->VADC_INT;
    tmp |= SH367303_INT_EN_Struct->WDT_INT;
    tmp |= SH367303_INT_EN_Struct->TWI_INT;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_INT_EN, tmp);    //写回寄存器
        if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_INT_EN) != tmp) 
    {
        #ifdef LOG_OUTPUT
        printf("Failed to write to register SH_INT_EN\n");
         #endif
        return SH367303_TimeOut;
    }
    return SH367303_OK;
}

/**
 * @brief   读取 SH36730X 中断状态标志
 *
 * @param   SH367303_FLAG_Struct   SH367303_FLAG_TypeDef 结构体指针，用于存储读取到的中断状态标志
 *
 * @return  void
 */
void SH36730X_ReadInterrupStatus(SH367303_FLAG_TypeDef *SH367303_FLAG_Struct)
{

}



/**
 * @brief   读取电流值
 *
 * @return  float   电流值（单位：安培）
 */
float SH36730X_Read_Current(void)
{
    //SH367303_SCONF_TypeDef *SH367303_SCONF_Struct;
	  uint16_t data;
    uint8_t polar;
    float Current;
	//SH367303_SCONF_Struct->RSNS = SH_RSNS_50mV; //读取全局结构体 待修改
    data=I2C_ReadReg2Byte_CRC8(SH_ADDR , SH_CURH);

    if (data & 0x1000)  //判断极性  12byte 1  ===  polar0 = 放电
    {
        data = data | 0xe000;
        polar = 0;
    }
    else
    {
        data = data;
        polar = 1;
    }
    
//    switch (SH367303_SCONF_Struct->RSNS)    //计算电流 单A
//    {
//        case SH_RSNS_400mV :
//            {
//                Current = (1.0f * (float)data) / (8192.0f * SH367303_Rsens);
//            }
//        case SH_RSNS_200mV :
//            {
//                Current = (1.0f * (float)data) / (16384.0f * SH367303_Rsens);
//            }
//        case SH_RSNS_100mV :
//            {
//                Current = (1.0f * (float)data) / (32768.0f * SH367303_Rsens);
//            }
//        case SH_RSNS_50mV :
//            {
                Current = (1.0f * (float)data) / (65536.0f * SH367303_Rsens);
//            }
//            break;
//        default:
//            break;
//    }
    
    switch (polar)  //计算极性
    {
        case 0:
            Current = -Current;
            break;
        case 1:
            Current = Current;
            break;
        default:
            break;
    }
	return Current;
}

/**
 * @brief   读取电池电压值
 *
 * @param   num     电池通道号
 *
 * @return  uint16_t   电压值（单位：毫伏）
 */
uint16_t SH36730X_Read_BATT_Voltage(uint8_t num)
{
    // 读取原始寄存器值
    uint16_t rawVoltage = I2C_ReadReg2Byte_CRC8(SH_ADDR , SH_CELL1H + num * 2);
    
    // 将原始寄存器值转换为电压（单位：毫伏）
    uint16_t Voltage = (uint16_t)(((float)rawVoltage * 6000.0f) / 4096.0f);

    return Voltage;
}


/**
 * @brief   读取温度值
 *
 * @param   num     温度传感器通道号
 *
 * @return  float   温度值（单位：摄氏度）
 */
int16_t SH36730X_Read_TEMP(uint8_t num)
{
    uint16_t data;
    float NTC;
    data = I2C_ReadReg2Byte_CRC8(SH_ADDR, SH_TS1H + num * 2);
    switch (num)
    {
    case 0:
        // 计算NTC值
        NTC = ((float)data * 10.0f) / (4096.0f - (float)data);
        // 使用NTC值调用lookup_temperature函数进行温度转换
        return (lookup_temperature(NTC)*10);
    case 1:
        // 计算NTC值
        NTC = ((float)data * 10.0f) / (4096.0f - (float)data);
        // 使用NTC值调用lookup_temperature函数进行温度转换
        return (lookup_temperature(NTC)*10);
    case 2:
        // 计算温度值
        return (uint16_t)(0.17f * (float)data - 270.0f)*10;
    case 3:
        // 计算温度值
        return (uint16_t)(0.17f * (float)data - 270.0f)*10;
    default:
        // 默认值
        return 0;
    }
}

/**
 * @brief   SH36730X 进入睡眠模式
 *
 * @return  void
 */
SH367303_StatusTypeDef SH36730X_Sleep(bool enable)
{
    //关闭所有MOS
    //SCONF10 0x33
    //04H PD_EN = 1
    return SH367303_OK;
}




/**
 * @brief   配置 SH36730X 连续模式下的电流 ADC 中断
 *
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_CADC_int(void)
{
    uint8_t tmp;
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_INT_EN) & 0xF7; // 读取寄存器中的保留位 1111 X111
    tmp |= SH_CADC_INT_DIS ; // 不输出报警
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_INT_EN, tmp); // 写回寄存器

    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) & 0xE0; // 读取寄存器中的保留位 111X XXXX
    tmp |= SH_CADC_EN ; // 开启转换
    tmp |= SH_CADC_M_CONTINUOUS ; // 连续采集模式
    tmp |= SH_CBIT_13 ; // 13位
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF3, tmp); // 写回寄存器

    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF6) & 0xC0; // 读取寄存器中的保留位 11XX XXXX
    tmp |= SH_RSNS_50mV ; // 采集范围
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF6, tmp); // 写回寄存器

    // 检查写入是否成功
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) != tmp || I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF6) != tmp) 
    {
#ifdef LOG_OUTPUT
        printf("Failed to write to register SH_SCONF3 or SH_SCONF6\n");
#endif
        return SH367303_TimeOut;
    }

    return SH367303_OK;
}

/**
 * @brief   开启/关闭VADC电压检测
 *
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_VADC_int(void)
{
    uint8_t tmp;
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_INT_EN) & 0xFB; // 读取寄存器中的保留位 1111 1X11
    tmp |= SH_VADC_INT_EN ;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_INT_EN, tmp); // 写回寄存器

    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) & 0x1F; // 读取寄存器中的保留位 XXX1 1111
    tmp |= SH_VADC_EN ;
    tmp |= SH_VADC_C_VT ;
    tmp |= SH_SCAN_C_50mS ;
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF3, tmp); // 写回寄存器

    // 检查写入是否成功
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_INT_EN) != tmp || I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF3) != tmp) 
    {
#ifdef LOG_OUTPUT
        printf("Failed to write to register SH_INT_EN or SH_SCONF3\n");
#endif
        return SH367303_TimeOut;
    }

    return SH367303_OK;
}


/**
 * @brief   读取开关量状态
 *
 * @return  电池状态字节
 */
uint8_t SH36730X_READ_BSTATUS()
{
    uint8_t tmp;
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_BSTATUS); //读取寄存器中的保留位 1111 1X11
	return tmp;
}

/**
 * @brief   开启/关闭充电MOS
 *
 * @param   enable  true表示开启，false表示关闭
 *
 * @return  SH367303_StatusTypeDef   返回操作状态，如果成功则返回SH367303_OK，否则返回SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_CHG(bool enable)
{
    uint8_t tmp;
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) & 0xFC; // 保留位 XXXX XX00
    tmp |= enable ? SH_CHG_EN : SH_CHG_DIS; // 根据输入的值设置充电MOS控制位
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF2, tmp);    // 写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) != tmp) {
#ifdef LOG_OUTPUT
        printf("Failed to set CHG MOS state\n");
#endif
        return SH367303_TimeOut;
    }
    return SH367303_OK;
}

/**
 * @brief   开启/关闭放电MOS
 *
 * @param   enable  true表示开启，false表示关闭
 *
 * @return  SH367303_StatusTypeDef   返回操作状态，如果成功则返回SH367303_OK，否则返回SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_DSG(bool enable)
{
    uint8_t tmp;
    tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) & 0xFC; // 保留位 XXXX XX00
    tmp |= enable ? SH_DSG_EN : SH_DSG_DIS; // 根据输入的值设置放电MOS控制位
    I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF2, tmp);    // 写回寄存器
    if (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF2) != tmp) {
#ifdef LOG_OUTPUT
        printf("Failed to set DSG MOS state\n");
#endif
        return SH367303_TimeOut;
    }
    return SH367303_OK;
}



/**
 * @brief   查找温度
 *
 * @param   resistance  电阻值
 * @return  温度值（摄氏度）
 */
int16_t lookup_temperature(float resistance) {
    for (int i = 0; i < sizeof(ntc_10k_table) / sizeof(ntc_10k_table[0]); ++i) {
        if (resistance >= ntc_10k_table[i].resistance) {
            if (i == 0) {
                return ntc_10k_table[i].temperature;
            }
            // 进行线性插值
            float temp_range = ntc_10k_table[i - 1].temperature - ntc_10k_table[i].temperature;
            float res_range = ntc_10k_table[i - 1].resistance - ntc_10k_table[i].resistance;
            float temp_diff = resistance - ntc_10k_table[i].resistance;
            float temp_interpolated = ntc_10k_table[i].temperature + (temp_diff / res_range) * temp_range;
            return (int16_t)temp_interpolated;
        }
    }
    // 如果电阻值高于表格中的最高值，则返回0作为错误值
    return 0;
}

/**
 * @brief   读取均衡通道状态并写入外设
 *
 * @param   channel  均衡通道编号，范围为 0 到 CELL_BALANCE_Count - 1
 * @param   enable   true 表示使能，false 表示禁用
 * @return  SH367303_StatusTypeDef   返回初始化结果，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_Write_CB(int channel, bool enable) {
    uint8_t tmp = 0;

    // 装填配置参数
    if (channel < 5) {
        tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF4);
        tmp &= ~(1 << (4 - channel)); // 清除指定位
        tmp |= (enable ? 1 : 0) << (4 - channel); // 设置指定位
        I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF4, tmp);
    } else if (channel < CELL_BALANCE_Count) {
        tmp = I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF5);
        tmp &= ~(1 << (CELL_BALANCE_Count - channel)); // 清除指定位
        tmp |= (enable ? 1 : 0) << (CELL_BALANCE_Count - channel); // 设置指定位
        I2C_WriteRegByte_CRC8(SH_ADDR, SH_SCONF5, tmp);
    } else {
#ifdef LOG_OUTPUT
        printf("Invalid channel number\n");
#endif
        return SH367303_TimeOut;
    }

    // 检查写入是否成功
    if (channel < 5 && (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF4) & (1 << (4 - channel))) != (enable ? 1 : 0)) {
#ifdef LOG_OUTPUT
    printf("Failed to write CB channel channel %d\n", channel);
#endif

        return SH367303_TimeOut;
    } else if (channel >= 5 && (I2C_ReadRegByte_CRC8(SH_ADDR, SH_SCONF5) & (1 << (CELL_BALANCE_Count - channel))) != (enable ? 1 : 0)) {
#ifdef LOG_OUTPUT
    printf("Failed to write CB channel %d\n", channel);
#endif

        return SH367303_TimeOut;
    }

    return SH367303_OK;
}

/**
 * @brief   从SH36730X读取均衡通道状态并更新到指定的数组中
 *
 * @param   CBxStatus   指向存储均衡通道状态的数组的指针，数组长度应为 Battery_Count
 *
 * @return  SH367303_StatusTypeDef 返回读取状态，如果成功则返回 SH367303_OK，否则返回 SH367303_TimeOut
 */
SH367303_StatusTypeDef SH36730X_Read_CBStatus(bool CBxStatus[Battery_Count]) {
    // 读取寄存器状态字节
    uint16_t sconf_data = I2C_ReadReg2Byte_CRC8(SH_ADDR, SH_SCONF4);

    // 检查是否成功读取了状态字节
    if (sconf_data == 0xFFFF) {
        return SH367303_TimeOut; // 读取失败
    }

    // 更新 CBxStatus 数组
    for (int i = 0; i < Battery_Count; ++i) {
        CBxStatus[i] = ((sconf_data >> (4 - i)) & 0x01) ? true : false;
    }

    return SH367303_OK; // 读取成功
}

