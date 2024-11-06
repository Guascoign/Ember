/********************************************************************************
    * 文件名称 ：SH36730X.h
    * 作     者：ZM
    * 版     本：V1.1
    * 编写日期 ：2024-04-15
    * 功     能：SH36730X 驱动代码
*********************************************************************************/
#ifndef _SH367303_H
#define _SH367303_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h> //包含标准类型定义头文件
#include <stdio.h>
#include "BMS.h"
#include <stdbool.h>    


#define LOG_OUTPUT
extern void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData);//通过I2C向设备寄存器写一个字节
extern uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress);//通过I2C从设备寄存器读一个字节
extern void SoftwareDelay(uint8_t ms);//软件延时毫秒
extern void I2C_WriteRegByte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData); //通过I2C向设备寄存器写一个字节(CRC校验)
extern uint16_t I2C_ReadReg2Byte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress); //通过I2C从设备寄存器读两个字节
extern uint8_t I2C_ReadRegByte_CRC8(uint8_t SlaveAddress, uint8_t RegAddress); //通过I2C从设备寄存器读一个字节


#define ON 1
#define OFF 0

// 定义SH状态枚举类型
typedef enum {
    SH367303_OK,         // 数据读取成功
    SH367303_TimeOut,    // 数据读取超时
} SH367303_StatusTypeDef;

#define SH367303_Rsens 0.005
//SH367303 寄存器地址定义
//7-bit address: 0x1B; 
//8-bit address: 0x36 for write command; 0x37 for read command
#define SH_ADDR             0x1B        //SH367303 I2C 设备地址 0011 011x
#define SH_ADDR_WRITE       (SH_ADDR<<1)        //SH367303 I2C 设备地址 0011 0110  0x36
#define SH_ADDR_READ        (SH_ADDR<<1)+1        //SH367303 I2C 设备地址 0011 0111  0x37
#define SH_FLAG1            0x00        //系统标志寄存器1
#define SH_FLAG2            0x01        //系统标志寄存器2
#define SH_BSTATUS          0x02        //系统标志寄存器3
#define SH_INT_EN           0x03        //中断请求标志位
#define SH_SCONF1           0x04        //系统设置寄存器
#define SH_SCONF2           0x05        //系统设置寄存器
#define SH_SCONF3           0x06        //系统设置寄存器
#define SH_SCONF4           0x07        //系统设置寄存器
#define SH_SCONF5           0x08        //系统设置寄存器
#define SH_SCONF6           0x09        //系统设置寄存器
#define SH_SCONF7           0x0A        //系统设置寄存器
#define SH_SCONF8           0x0B        //系统设置寄存器
#define SH_SCONF9           0x0C        //系统设置寄存器
#define SH_SCONF10          0x0D        //系统设置寄存器
#define SH_CELL1H           0x0E        //电池1电压(高位)
#define SH_CELL1L           0x0F        //电池1电压(低位)
#define SH_CELL2H           0x10        //电池2电压(高位)
#define SH_CELL2L           0x11        //电池2电压(低位)
#define SH_CELL3H           0x12        //电池3电压(高位)
#define SH_CELL3L           0x13        //电池3电压(低位)
#define SH_CELL4H           0x14        //电池4电压(高位)
#define SH_CELL4L           0x15        //电池4电压(低位)
#define SH_CELL5H           0x16        //电池5电压(高位)
#define SH_CELL5L           0x17        //电池5电压(低位)
#define SH_CELL6H           0x18        //电池6电压(高位)
#define SH_CELL6L           0x19        //电池6电压(低位)
#define SH_CELL7H           0x1A        //电池7电压(高位)
#define SH_CELL7L           0x1B        //电池7电压(低位)
#define SH_CELL8H           0x1C        //电池8电压(高位)
#define SH_CELL8L           0x1D        //电池8电压(低位)
#define SH_CELL9H           0x1E        //电池9电压(高位)
#define SH_CELL9L           0x1F        //电池9电压(低位)
#define SH_CELL10H          0x20        //电池10电压(高位)
#define SH_CELL10L          0x21        //电池10电压(低位)
#define SH_TS1H             0x22        //电池电流(高位)
#define SH_TS1L             0x23        //电池电流(高位)
#define SH_TS2H             0x24        //电池电流(高位)
#define SH_TS2L             0x25        //电池电流(高位)
#define SH_TEMP1H           0x26        //电池电流(高位)
#define SH_TEMP1L           0x27        //电池电流(高位)
#define SH_TEMP2H           0x28        //电池电流(高位)
#define SH_TEMP2L           0x29        //电池电流(高位)
#define SH_CURH             0x2A        //电池电流(高位)
#define SH_CURL             0x2B        //电池电流(低位)


//SH367303 系统标志 结构体定义 R对应FLAG1 FLAG2 BSTATUS 
typedef struct
{
    uint8_t SC;              //硬件短路保护标志位.
    uint8_t OV;              //硬件过充电保护标志位.
    uint8_t WDT;             //看门狗标志位.
    uint8_t TWI;             //TWI Timeout标志位.
    uint8_t RST;             //RST标志位.
    uint8_t CADC;            //CADC中断标志位.
    uint8_t VADC;            //VADC中断标志位.
    uint8_t DSG;             //VADC中断标志位.
    uint8_t CHG;             //VADC中断标志位.
    uint8_t DSGING;          //放电状态位.
    uint8_t CHGING;          //充电状态位.
    uint8_t LOAD;            //负载连接状态位.
    uint8_t CHGR;            //充电器连接状态位.
}SH367303_FLAG_TypeDef;

//SH367303 中断请求标志位 R/W结构体定义 对应INT_EN
typedef struct
{
    uint8_t SC_INT;             //硬件短路保护中断使能位.
    uint8_t OV_INT;             //硬件过充电保护中断使能位.
    uint8_t CD_INT;             //充放电状态中断使能位.
    uint8_t CADC_INT;           //CADC中断使能位.
    uint8_t VADC_INT;           //VADC中断使能位.
    uint8_t WDT_INT;            //WDT中断使能位.
    uint8_t TWI_INT;            //TWI Timeout中断使能位.
}SH367303_INT_EN_TypeDef;

    //6 SC_INT 硬件短路保护中断使能位
    #define SH_SC_INT_EN          0x00    //0：发生硬件短路保护后，ALARM管脚不输出报警信号
    #define SH_SC_INT_DIS         0x40    //1：发生硬件短路保护后，ALARM管脚输出报警信号
    //5 OV_INT 硬件过充电保护中断使能位
    #define SH_OV_INT_EN          0x00    //0：硬件过充电保护后，ALARM管脚不输出报警信号
    #define SH_OV_INT_DIS         0x40    //1：硬件过充电保护后，ALARM管脚输出报警信号
    //4 CD_INT 充放电状态中断使能位
    #define SH_CD_INT_EN          0x00    //0：检测到充放电状态后，ALARM管脚不输出报警信号
    #define SH_CD_INT_DIS         0x40    //1：检测到充放电状态后，ALARM管脚输出报警信号
    //3 CADC_INT CADC中断使能位
    #define SH_CADC_INT_DIS       0x00    //0：CADC转换完成后，ALARM管脚不输出报警信号
    #define SH_CADC_INT_EN     		0x40    //1：CADC转换完成后，ALARM管脚仅输出低电平脉冲
    //2 VADC_INT VADC中断使能位
    #define SH_VADC_INT_EN        0x00    //0：VADC转换完成后，ALARM管脚不输出报警信号
    #define SH_VADC_INT_DIS       0x40    //1：VADC转换完成后，ALARM管脚仅输出低电平脉冲
    //1 WDT_INT WDT中断使能位
    #define SH_WDT_INT_EN         0x00    //0：发生WDT后，ALARM管脚不输出报警信号
    #define SH_WDT_INT_DIS        0x40    //1：发生WDT后，ALARM管脚输出报警信号
    //0 TWI_INT TWI Timeout中断使能位
    #define SH_TWI_INT_EN         0x00    //0：发生Timeout后，ALARM管脚不输出报警信号
    #define SH_TWI_INT_DIS        0x40    //1：发生Timeout 后，ALARM管脚输出报警信号

//SH367303  系统设置寄存器 结构体定义 对应SCONF1-10
typedef struct
{
    uint8_t LTCLR;             //清除SC/OV/TWI/WDT标志位.
    uint8_t CTLD_EN;           //CTLD功能控制位.
    uint8_t PD_EN;             //低功耗状态控制位.
    uint8_t WDT_EN;            //看门狗使能控制位.
    uint8_t SC_EN;             //硬件短路保护使能控制位.
    uint8_t OV_EN;             //硬件过充电保护使能控制位.
    uint8_t LOAD_EN;           //负载检测使能控制位.
    uint8_t CHGR_EN;           //充电器检测使能控制位.
    uint8_t RESET_PF;          //复位外部MCU/二级保护选择位.
    uint8_t ALARM_C;           //ALARM输出选择位.
    uint8_t DSG_C;             //放电MOS控制位.
    uint8_t CHG_C;             //充电MOS控制位.
    uint8_t CADC_EN;           //CADC使能控制位.
    uint8_t CADC_M;            //CADC采样方式选择位.
    uint8_t CBIT_C;            //CADC精度选择位.
    uint8_t VADC_EN;           //VADC使能控制位.
    uint8_t VADC_C;            //VADC采样方式选择位.
    uint8_t SCAN_C;            //VADC转换周期选择位.
    uint8_t CB10;              //平衡回路10控制位.
    uint8_t CB9;               //平衡回路9控制位.
    uint8_t CB8;               //平衡回路8控制位.
    uint8_t CB7;               //平衡回路7控制位.
    uint8_t CB6;               //平衡回路6控制位.
    uint8_t CB5;               //平衡回路5控制位.
    uint8_t CB4;               //平衡回路4控制位.
    uint8_t CB3;               //平衡回路3控制位.
    uint8_t CB2;               //平衡回路2控制位.
    uint8_t CB1;               //平衡回路1控制位.
    uint8_t RSNS;              //CADC采集范围选择位.
    uint8_t RST;               //复位外部MCU脉冲宽度选择位.
    uint8_t SCV;               //硬件短路保护电压设置选择位.
    uint8_t SCT;               //硬件短路保护延时设置选择位.
    uint8_t OVT;               //硬件过充电保护延时选择位.
    uint8_t CHS;               //充放电状态检测阈值选择位.
    uint8_t WDTT;              //看门狗溢出时间选择位.
    uint16_t OVD;              //硬件过充电保护阈值设置低8位，计算方式：寄存器值×5.86mV. MAX 6V
    uint8_t PIN;               //进低功耗控制位.
}SH367303_SCONF_TypeDef;

    //SCONF1
    //7 LTCLR 清除SC/OV/TWI/WDT标志位
    #define SH_LTCLR_DIS          0x00    //0：正常
    #define SH_LTCLR_EN           0x80    //1->0：清除标志寄存器(Flag1)各保护标志位
    //6 CTLD_EN CTLD功能控制位
    #define SH_CTLD_DIS           0x00    //0：关闭CTLD功能
    #define SH_CTLD_EN            0x40    //1：开启CTLD功能
    //5 PD_EN 低功耗状态控制位
    #define SH_PD_DIS             0x00    //0：正常状态
    #define SH_PD_EN              0x20    //1：低功耗状态
    //4 WDT_EN 看门狗使能控制位
    #define SH_WDT_DIS            0x00    //0：关闭看门狗模块
    #define SH_WDT_EN             0x10    //1：开启看门狗模块
    //3 SC_EN 硬件短路保护使能控制位
    #define SH_SC_DIS             0x00    //0：关闭硬件短路保护功能
    #define SH_SC_EN              0x08    //1：开启硬件短路保护功能
    //2 OV_EN 硬件过充电保护使能控制位
    #define SH_OV_DIS             0x00    //0：关闭硬件过充电保护功能
    #define SH_OV_EN              0x04    //1：开启硬件过充电保护功能
    //1 LOAD_EN 负载检测使能控制位
    #define SH_LOAD_DIS           0x00    //0：关闭负载检测功能
    #define SH_LOAD_EN            0x02    //1：开启负载检测功能
    //0 CHGR_EN 充电器检测使能控制位
    #define SH_CHGR_DIS           0x00    //0：关闭充电器检测
    #define SH_CHGR_EN            0x01    //1：开启充电器检测

    //SCONF2
    //3 RESET/PF 复位外部MCU/二级保护选择位
    #define SH_RESET_PF_DIS       0x00    //0：复位外部MCU功能
    #define SH_RESET_PF_EN        0x08    //1：二级保护功能
    //2 ALARM_C ALARM输出选择位
    #define SH_ALARM_DIS          0x00    //0：输出低电平脉冲
    #define SH_ALARM_EN           0x04    //1：输出持续低电平
    //1 DSG_C 放电MOS控制位
    #define SH_DSG_DIS            0x00    //0：放电MOS关闭
    #define SH_DSG_EN             0x02    //1：放电MOS由硬件保护模块决定
    //0 CHG_C 充电MOS控制位
    #define SH_CHG_DIS            0x00    //0：充电MOS关闭
    #define SH_CHG_EN             0x01    //1：充电MOS由硬件保护模块决定

    //SCONF3
    //7 CADC_EN CADC使能控制位
    #define SH_CADC_DIS           0x00    //0：关闭CADC模块
    #define SH_CADC_EN            0x80    //1：开启CADC模块
    //6 CADC_M CADC采样方式选择位
    #define SH_CADC_M_SINGLE      0x00    //0：单次采集，完成后CADC_EN自动为0
    #define SH_CADC_M_CONTINUOUS  0x40    //1：连续采集
    //5 CBIT_C CADC精度选择位
    #define SH_CBIT_10           	0x00    //0：10-bit CADC
    #define SH_CBIT_13            0x20    //1：13-bit CADC
    //4 VADC_EN VADC使能控制位
    #define SH_VADC_DIS           0x00    //0：关闭VADC模块
    #define SH_VADC_EN            0x10    //1：开启VADC模块
    //3 VADC_C VADC采样方式选择位
    #define SH_VADC_C_V        	  0x00    //0：开启电压采集
    #define SH_VADC_C_VT          0x08    //1：开启电压和温度采集
    //2-0 SCAN_C VADC转换周期选择位
    #define SH_SCAN_C_50mS        0x00    //000：50mS
    #define SH_SCAN_C_100mS       0x01    //001：100mS
    #define SH_SCAN_C_200mS       0x02    //010：200mS
    #define SH_SCAN_C_500mS       0x03    //011：500mS
    #define SH_SCAN_C_1S          0x04    //100：1S
    #define SH_SCAN_C_2S          0x05    //101：2S
    #define SH_SCAN_C_4S          0x06    //110：4S
    #define SH_SCAN_C_8S          0x07    //111：8S

    //SCONF4
    //4 CB10 平衡回路10控制位
    #define SH_CB10_DIS           0x00    //0：关闭VCN10平衡回路
    #define SH_CB10_EN            0x10    //1：开启VCN10平衡回路
    //3 CB9 平衡回路9控制位
    #define SH_CB9_DIS            0x00    //0：关闭VCN9平衡回路
    #define SH_CB9_EN             0x08    //1：开启VCN9平衡回路
    //2 CB8 平衡回路8控制位
    #define SH_CB8_DIS           0x00    //0：关闭VCN8平衡回路
    #define SH_CB8_EN            0x04    //1：开启VCN8平衡回路
    //1 CB7 平衡回路7控制位
    #define SH_CB7_DIS           0x00    //0：关闭VCN7平衡回路
    #define SH_CB7_EN            0x02    //1：开启VCN7平衡回路
    //0 CB6 平衡回路6控制位
    #define SH_CB6_DIS           0x00    //0：关闭VCN6平衡回路
    #define SH_CB6_EN            0x01    //1：开启VCN6平衡回路

    //SCONF5
    //4 CB5 平衡回路5控制位
    #define SH_CB5_DIS           0x00    //0：关闭VCN5平衡回路
    #define SH_CB5_EN            0x10    //1：开启VCN5平衡回路
    //3 CB4 平衡回路4控制位
    #define SH_CB4_DIS           0x00    //0：关闭VCN4平衡回路
    #define SH_CB4_EN            0x08    //1：开启VCN4平衡回路
    //2 CB3 平衡回路3控制位
    #define SH_CB3_DIS           0x00    //0：关闭VCN3平衡回路
    #define SH_CB3_EN            0x04    //1：开启VCN3平衡回路
    //1 CB2 平衡回路2控制位
    #define SH_CB2_DIS           0x00    //0：关闭VCN2平衡回路
    #define SH_CB2_EN            0x02    //1：开启VCN2平衡回路
    //0 CB1 平衡回路1控制位
    #define SH_CB1_DIS           0x00    //0：关闭VCN1平衡回路
    #define SH_CB1_EN            0x01    //1：开启VCN1平衡回路

    //SCONF6
    //7-6 RSNS 参数定义
    #define SH_RSNS_400mV        0x00    //CADC采集范围选择00：400mV
    #define SH_RSNS_200mV        0x40    //CADC采集范围选择01：200mV
    #define SH_RSNS_100mV        0x80    //CADC采集范围选择10：100mV
    #define SH_RSNS_50mV         0xC0    //CADC采集范围选择11：50mV
    //5-4 RST 参数定义
    #define SH_RST_16mS          0x00    //复位外部MCU脉冲宽度00：16mS
    #define SH_RST_32mS          0x10    //复位外部MCU脉冲宽度01：32mS
    #define SH_RST_128mS         0x20    //复位外部MCU脉冲宽度10：128mS
    #define SH_RST_1S            0x30    //复位外部MCU脉冲宽度=11：1S
    //3-2 SCV 参数定义
    #define SH_SCV_100mV         0x00    //硬件短路保护电压00：100mV
    #define SH_SCV_200mV         0x04    //硬件短路保护电压01：200mV
    #define SH_SCV_300mV         0x08    //硬件短路保护电压10：300mV
    #define SH_SCV_400mV         0x0C    //硬件短路保护电压11：400mV
    //5-4 SCT 参数定义
    #define SH_SCT_50uS          0x00    //硬件短路保护延时00：50uS 
    #define SH_SCT_100uS         0x10    //硬件短路保护延时01：100uS
    #define SH_SCT_300uS         0x20    //硬件短路保护延时10：300uS
    #define SH_SCT_500uS         0x30    //硬件短路保护延时11：500uS
 
    //SCONF7
    //6-4 OVT 硬件过充电保护延时选择位
    #define SH_OVT_1             0x00    //000：1个转换周期
    #define SH_OVT_2             0x10    //001：2个转换周期
    #define SH_OVT_4             0x20    //010：4个转换周期
    #define SH_OVT_8             0x30    //011：8个转换周期
    #define SH_OVT_16            0x40    //100：16个转换周期
    #define SH_OVT_32            0x50    //101：32个转换周期
    #define SH_OVT_64            0x60    //110：64个转换周期
    #define SH_OVT_128           0x70    //111：128个转换周期
    //3-2 CHS 充放电状态检测阈值选择位
    #define SH_CHS_14mV          0x00    //00：1.4mV
    #define SH_CHS_3mV           0x04    //01：3mV
    #define SH_CHS_6mV           0x08    //10：6mV
    #define SH_CHS_12mV          0x0C    //11：12mV
    //1-0 WDTT 看门狗溢出时间选择位
    #define SH_WDTT_30S          0x00    //00：看门狗溢出时间为30S
    #define SH_WDTT_10S          0x01    //01：看门狗溢出时间为10S
    #define SH_WDTT_2S           0x10    //10：看门狗溢出时间为2S
    #define SH_WDTT_500mS        0x11    //11：看门狗溢出时间为500mS

    //SCONF8-9
    //OVD 硬件过充电保护阈值设置 10bit

    //SCONF10
    // PIN 进低功耗控制位
    #define SH_PIN_Reserved     0x00    //其它：Reserved
    #define SH_PIN_EN           0x33    //允许PD_EN使能位写1，关闭LDO

//SH367303  系统设置寄存器 结构体定义 对应CELL1-10 TS1-2 TMEP1-2 CUR1
typedef struct
{
    float CELL_1;             //Cell1电芯电压寄存器.12bit
    float CELL_2;             //Cell2电芯电压寄存器.12bit
    float CELL_3;             //Cell3电芯电压寄存器.12bit
    float CELL_4;             //Cell4电芯电压寄存器.12bit
    float CELL_5;             //Cell5电芯电压寄存器.12bit
    float CELL_6;             //Cell6电芯电压寄存器.12bit
    float CELL_7;             //Cell7电芯电压寄存器.12bit
    float CELL_8;             //Cell8电芯电压寄存器.12bit
    float CELL_9;             //Cell9电芯电压寄存器.12bit
    float CELL_10;            //Cell10电芯电压寄存器.12bit
    float TS_1;               //TS1温度寄存器.12bit
    float TS_2;               //TS2温度寄存器.12bit
    float TEMP_1;             //TEMP1内部温度寄存器.12bit
    float TEMP_2;             //TEMP2内部温度寄存器.12bit
    float CUR;                //电流寄存器.1bit符号(“1”表示放电；“0”表示充电) + 12bit
}SH367303_STATUS_TypeDef;


typedef struct{
   int16_t    temperature;//单位：°C
   float  resistance;//单位：Ω
}NTC_10KTYPE;

#define NTC_LIST_MAX_SIZE             (sizeof(ntc_10k_table) / sizeof(ntc_10k_table[0]))/*表的长度*/
	
// NTC 10K 温度-电阻表格
static const NTC_10KTYPE ntc_10k_table[] = {
    {-40, 235.8307559}, {-39, 221.6724098}, {-38, 208.473826}, {-37, 196.1630569}, {-36, 184.6740349},
    {-35, 173.9460536}, {-34, 163.9232991}, {-33, 154.5544238}, {-32, 145.7921607}, {-31, 137.5929735},
    {-30, 129.9167384}, {-29, 122.7264551}, {-28, 115.9879839}, {-27, 109.6698071}, {-26, 103.7428109},
    {-25, 98.18008736}, {-24, 92.95675344}, {-23, 88.04978631}, {-22, 83.43787284}, {-21, 79.1012721},
    {-20, 75.0216899}, {-19, 71.18216392}, {-18, 67.56695872}, {-17, 64.16146957}, {-16, 60.95213444},
    {-15, 57.92635333}, {-14, 55.07241424}, {-13, 52.37942535}, {-12, 49.83725271}, {-11, 47.43646304},
    {-10, 45.16827118}, {-9, 43.02449173}, {-8, 40.99749462}, {-7, 39.08016422}, {-6, 37.26586165},
    {-5, 35.54839008}, {-4, 33.92196277}, {-3, 32.38117357}, {-2, 30.92096971}, {-1, 29.53662669},
    {0, 28.22372509}, {1, 26.97812912}, {2, 25.79596688}, {3, 24.67361196}, {4, 23.60766657},
    {5, 22.59494578}, {6, 21.63246309}, {7, 20.71741687}, {8, 19.84717797}, {9, 19.01927811},
    {10, 18.23139919}, {11, 17.48136327}, {12, 16.76712341}, {13, 16.08675502}, {14, 15.4384479},
    {15, 14.82049884}, {16, 14.23130468}, {17, 13.66935597}, {18, 13.1332309}, {19, 12.62158981},
    {20, 12.13317001}, {21, 11.66678088}, {22, 11.22129948}, {23, 10.79566624}, {24, 10.38888114},
    {25, 10}, {26, 9.628131096}, {27, 9.272431959}, {28, 8.932106406}, {29, 8.606401759},
    {30, 8.294606244}, {31, 7.996046556}, {32, 7.710085586}, {33, 7.436120291}, {34, 7.173579697},
    {35, 6.921923035}, {36, 6.680637987}, {37, 6.449239052}, {38, 6.227265999}, {39, 6.014282436},
    {40, 5.809874448}, {41, 5.613649332}, {42, 5.425234404}, {43, 5.244275879}, {44, 5.070437823},
    {45, 4.90340116}, {46, 4.742862746}, {47, 4.588534498}, {48, 4.440142569}, {49, 4.297426576},
    {50, 4.160138877}, {51, 4.028043881}, {52, 3.900917407}, {53, 3.778546077}, {54, 3.660726742},
    {55, 3.547265944}, {56, 3.437979407}, {57, 3.332691561}, {58, 3.231235085}, {59, 3.133450484},
    {60, 3.039185685}, {61, 2.948295658}, {62, 2.860642056}, {63, 2.776092875}, {64, 2.694522137},
    {65, 2.615809586}, {66, 2.539840398}, {67, 2.466504917}, {68, 2.395698395}, {69, 2.327320749},
    {70, 2.261276335}, {71, 2.19747373}, {72, 2.135825526}, {73, 2.076248134}, {74, 2.018661605},
    {75, 1.962989451}, {76, 1.909158479}, {77, 1.857098639}, {78, 1.80674287}, {79, 1.758026963},
    {80, 1.710889424}, {81, 1.665271351}, {82, 1.62111631}, {83, 1.578370221}, {84, 1.536981253},
    {85, 1.496899717}, {86, 1.458077968}, {87, 1.420470316}, {88, 1.384032933}, {89, 1.348723772},
    {90, 1.314502486}, {91, 1.281330351}, {92, 1.249170196}, {93, 1.217986331}, {94, 1.187744486},
    {95, 1.158411744}, {96, 1.129956484}, {97, 1.102348327}, {98, 1.075558075}, {99, 1.049557669},
    {100, 1.024320132}, {101, 0.999819529}, {102, 0.976030918}, {103, 0.952930309}, {104, 0.930494626},
    {105, 0.908701665}, {106, 0.88753006}, {107, 0.866959246}, {108, 0.846969428}, {109, 0.827541548},
    {110, 0.808657252}, {111, 0.790298866}, {112, 0.772449364}, {113, 0.755092345}, {114, 0.738212003},
    {115, 0.721793108}, {116, 0.70582098}, {117, 0.690281467}, {118, 0.675160924}, {119, 0.660446194},
    {120, 0.646124587}, {121, 0.632183863}, {122, 0.618612213}, {123, 0.605398243}, {124, 0.592530958},
    {125, 0.579999745}, {126, 0.56779436}, {127, 0.555904913}
};


 
SH367303_StatusTypeDef SH36730X_Init(void);           //SH初始化
SH367303_StatusTypeDef SH36730X_HardwareInit(SH367303_SCONF_TypeDef *SH367303_SCONF_Struct); //电池保护设置
SH367303_StatusTypeDef SH36730X_ConfigInterruptMask(SH367303_INT_EN_TypeDef *SH367303_INT_EN_Struct);     //中断屏蔽
void SH36730X_ReadInterrupStatus(SH367303_FLAG_TypeDef *SH367303_FLAG_Struct);      //读取中断状态
float SH36730X_Read_Current(void);         //读取的电池电流
uint16_t SH36730X_Read_BATT_Voltage(uint8_t num);    //读取电池电压
int16_t SH36730X_Read_TEMP(uint8_t num);        //读内外部温度
SH367303_StatusTypeDef SH36730X_Sleep(bool enable);       //进入低功耗模式
SH367303_StatusTypeDef SH36730X_CADC_int(void);
SH367303_StatusTypeDef SH36730X_VADC_int(void);
void SH36730X_CTLD(uint8_t status);
uint8_t SH36730X_READ_BSTATUS(void);
void SH36730X_MOS_EN(void);
int16_t lookup_temperature(float resistance) ;
SH367303_StatusTypeDef SH36730X_Write_CB(int channel, bool enable);
SH367303_StatusTypeDef SH36730X_CHG(bool enable);
SH367303_StatusTypeDef SH36730X_DSG(bool enable);
SH367303_StatusTypeDef SH36730X_Read_CBStatus(bool CBxStatus[Battery_Count]);



#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !SC8815_H
