/********************************************************************************
    * 文件名称 ：BMS.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：BMS功能函数
*********************************************************************************/
#ifndef _BMS_H_
#define _BMS_H_

#include "sm.h"
#include "main.h"
#include <stdbool.h>    

#define	Battery_Count	5	// 电池数量
#define	CELL_BALANCE_Count	(Battery_Count - 1)	// 均衡回路
#define Temp_Count 4  // 温度传感器数量
#define BATTERY_CAPACITY 10000  // 电池出场容量，假设为 10000 mAh
#define UPDATE_INTERVAL 1000    // SOC最小更新间隔，假设为 1000 毫秒
#define SYSTEM_POWER_CONSUMPTION 0 // 系统功耗（单位：mAh）

// 数据获取源
#define USING_SH367303
//#define USING_SH367309
//#define USING_UART

#define ON 1
#define OFF 0

#define BAT_OverChargeWarn            4200  // 过充预警电压 4.2V
#define BAT_OverChargeAlarm           4500  // 过充报警电压 4.5V
#define BAT_OverChargeRecovery        4100  // 过充恢复电压 4.1V

#define BAT_OverDischargeWarn         3300  // 过放预警电压 3.3V
#define BAT_OverDischargeAlarm        3000  // 过放报警电压 3.0V
#define BAT_OverDischargeRecovery     3400  // 过放恢复电压 3.4V

#define BAT_OverChargeCurrentWarn     500  // 充电过流电流 5A
#define BAT_OverChargeCurrentAlarm    300  // 充电过流预警电流 3A

#define BAT_OverDischargeCurrentWarn  500  // 放电过流电流 5A
#define BAT_OverDischargeCurrentAlarm 300  // 放电过流预警电流 3A

#define BAT_HighChargeTemperatureWarn     600   // 过温预警温度 60℃
#define BAT_HighChargeTemperatureAlarm    600   // 过温预警温度 60℃
#define BAT_HighChargeTemperatureRecovery 500   // 过温恢复温度 50℃

#define BAT_LowChargeTemperatureWarn     (-100) // 低温预警温度 -10℃
#define BAT_LowChargeTemperatureAlarm    (-100) // 低温预警温度 -10℃
#define BAT_LowChargeTemperatureRecovery  0     // 低温恢复温度 0℃

#define BAT_HighDischargeTemperatureWarn     600   // 过温预警温度 60℃
#define BAT_HighDischargeTemperatureAlarm    600   // 过温预警温度 60℃
#define BAT_HighDischargeTemperatureRecovery 500   // 过温恢复温度 50℃

#define BAT_LowDischargeTemperatureWarn     (-100) // 低温预警温度 -10℃
#define BAT_LowDischargeTemperatureAlarm    (-100) // 低温预警温度 -10℃
#define BAT_LowDischargeTemperatureRecovery  0     // 低温恢复温度 0℃

#ifdef USING_UART
	#include "UART_BMS.h" //函数重映射 SH03/06
  #define AFE_Read_BATT_Voltage UART_Read_BATT_Voltage
  #define AFE_Read_Current UART_Read_Current
  #define AFE_Read_TEMP UART_Read_TEMP
#endif

#ifdef USING_SH367303
	#include "SH367303.h" //函数重映射 SH03/06
  #define AFE_Read_BATT_Voltage SH36730X_Read_BATT_Voltage
  #define AFE_Read_Current SH36730X_Read_Current
  #define AFE_Read_TEMP SH36730X_Read_TEMP
  #define AFE_Init SH36730X_Init
  #define AFE_Sleep SH36730X_Sleep
  #define AFE_CHG SH36730X_CHG
  #define AFE_DSG SH36730X_DSG
  #define AFE_Write_CB SH36730X_Write_CB
  #define AFE_Write_CTLD SH36730XWrite_CTLD //待编写 感觉没啥用 不写了
  #define AFE_Read_CBStatus SH36730X_Read_CBStatus //待编写
#endif

#ifdef USING_SH367309
	#include "SH367309.h" //函数重映射
  #define AFE_Read_BATT_Voltage SH367309_Read_BATT_Voltage
  #define AFE_Read_Current SH367309_Read_Current
  #define AFE_Read_TEMP SH367309_Read_TEMP
#endif

// 定义电池管理系统状态枚举类型
typedef enum {
    BMS_OK,         // BMS 正常
    BMS_Warning,    // BMS 警告
    BMS_Alarm,      // BMS 报警
    BMS_Error       // BMS 错误（数据获取失败）
} BMS_ReturnTypeDef;

enum SwitchStatus {
  CHGR,      //充电器连接状态位 
  LOAD,      //负载连接状态位
  CHGING,    //充电状态位 
  DSGING,    //放电状态位 
  CHG,       //充电MOS开关状态位
  DSG       //放电MOS开关状态位
};

// 定义报警类型的枚举
enum V_AlarmType{
    OverChargeWarn ,           //过充警告
    OverChargeAlarm,           //过充报警

    OverDischargeWarn,          //过放警告
    OverDischargeAlarm,         //过放报警
};

enum I_AlarmType{
    OverChargeCurrentAlarm ,    //充电 过流报警
    OverChargeCurrentWarn,      //充电 过流警告

    OverDischargeCurrentWarn,   //放电 过流警告
    OverDischargeCurrentAlarm,  //放电 过流报警
};

enum T_AlarmType{
    ChargeHighTemperatureWarn,        //充电 高温警告
    ChargeHighTemperatureAlarm,       //充电 高温报警

    ChargeLowTemperatureWarn,         //充电 低温警告
    ChargeLowTemperatureAlarm,         //充电 低温报警

    DischargeHighTemperatureWarn,        //放电 高温警告
    DischargeHighTemperatureAlarm,       //放电 高温报警

    DischargeLowTemperatureWarn,         //放电 低温警告
    DischargeLowTemperatureAlarm         //放电 低温报警
};

typedef struct {
    uint16_t BAT_Voltage[Battery_Count];//单电池电压 3300 = 3.300V
    uint16_t Pack_Voltage;         //单电池电压 58000 = 58.000V
    int16_t Current;               //电池组电流  100 = 1.00A +为充电 -为放电
    int16_t Temperature[Temp_Count]; //温度 3850 = 38.5°C
    bool SwitchStatus[6];          // 开关状态（负载检测、充电检测、MOS 状态等）1开启 0关闭
    bool CBxStatus[CELL_BALANCE_Count];//均衡通道状态 1开启 0关闭
} BMS_StatusTypeDef; //存储电池初始采集状态

typedef struct {
    uint32_t Last_Update;//上次更新时间
    uint32_t ChargedPower;  //累计充电能量Wh（除电池容量计算出冲电循环次数）
    uint32_t DischargedPower;  //累计放电容量Wh（除电池容量计算出放电循环次数）
    uint16_t SOC; //动力电池荷电状态，State of Charge
    uint16_t SOH; //动力电池健康状态，State of Health
    uint16_t DOD; //电池包放电深度，Depth of discharge
    uint16_t SOE; //电池剩余能量，Stete of Energy
    bool V_Alarm[4][Battery_Count]; // BMS电压报警 报警类型为AlarmType
    bool T_Alarm[4];             // BMS温度报警 报警类型为AlarmType
    bool I_Alarm[8][Temp_Count];// BMS电流报警 报警类型为AlarmType
} BMS_StateTypeDef; //存储电池计算后状态 需保存至EEPROM

typedef struct {
  bool DSG; //放电MOS控制
  bool CHG; //充电MOS控制
  bool CTLD; //放电PWM控制
  bool CBxControl[CELL_BALANCE_Count]; //均衡通道控制
  bool Sleep;//进入低功耗（只能进）
} BMS_ControlTypeDef; //存储控制状态

typedef struct {
  BMS_StatusTypeDef Status;   //存储电池初始采集状态
  BMS_StateTypeDef State;     //存储电池计算后状态
  BMS_ControlTypeDef Control; //存储控制状态
} BMSTypeDef; //BMS总结构体
extern BMSTypeDef BMS;

typedef struct {
    uint32_t cycle_count;//总充放电循环次数
    uint16_t soh;//对于循环SOH数值
} SOHTableEntryTypeDef;

// 初始化 SOH 表，包含 10 个点
static const SOHTableEntryTypeDef SOH_Table[10] = {// 初始 SOH 为 100% 每循环100次SOH衰减10%
    {0, 100},{100, 90},{200, 80},{300, 70},{400, 60},
    {500, 50},{600, 40},{700, 30},{800, 20},{900, 10}
};

BMS_ReturnTypeDef BMS_Read_Voltage(BMSTypeDef *BMS);		//读出电池电压并且传入数组中
BMS_ReturnTypeDef BMS_Read_Current(BMSTypeDef *BMS);		//读出电池电流并且传入数组中
BMS_ReturnTypeDef BMS_Read_Temp(BMSTypeDef *BMS);			//读出温度并且传入数组中
BMS_ReturnTypeDef BMS_Read_SwitchStatus(BMSTypeDef *BMS);		//读出电池电流并且传入数组中
BMS_ReturnTypeDef BMS_Read_Status(BMSTypeDef *BMS);		//读出所有状态并且传入数组中（待编写开关量）
BMS_ReturnTypeDef BMS_Status_Cacluate(BMSTypeDef *BMS);//根据事实量判断告警
// BMS_ReturnTypeDef BMS_BOOM_Handel(BMSTypeDef *BMS);		//根据BMS中断信息控制外设（紧急程度高）瞬时过流、主动熔断
// BMS_ReturnTypeDef BMS_Alarm_Handel(BMSTypeDef *BMS);		//根据告警信息控制外设（紧急程度中）
// BMS_ReturnTypeDef BMS_Warn_Handel(BMSTypeDef *BMS);		//根据预警信息控制外设（紧急程度低）
BMS_ReturnTypeDef BMS_Param_Check(BMSTypeDef *BMS);		//初始化过压过流参数等检测
BMS_ReturnTypeDef BMS_SOC_Update(BMSTypeDef *BMS);		//计算SOC
BMS_ReturnTypeDef BMS_SOH_Update(BMSTypeDef *BMS);   //计算SOH
BMS_ReturnTypeDef BMS_CheckAlarm(BMSTypeDef *BMS);
BMS_ReturnTypeDef BMS_to_MCU(BMSTypeDef *BMS , bms_info *Bms_info);	//采集数据转换
BMS_ReturnTypeDef BMS_StateSave(BMSTypeDef *BMS);
BMS_ReturnTypeDef BMS_Control_Update(BMSTypeDef *BMS);//控制数据写入
BMS_ReturnTypeDef BMS_CHG(BMSTypeDef *BMS, bool *enable);
BMS_ReturnTypeDef BMS_DSG(BMSTypeDef *BMS, bool *enable);
#endif
