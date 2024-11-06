/********************************************************************************
    * 文件名称 ：BMS.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：BMS功能函数
*********************************************************************************/
#include "BMS.h"

/**
 * @brief   读取电池电压
 *
 * @param   BMS_Status  电池管理系统信息结构体指针，用于保存读取到的电压信息
 *
 * @return  BMS_ReturnTypeDef 返回读取状态，BMS_OK 表示读取成功，BMS_Error 表示读取失败
 */
BMS_ReturnTypeDef BMS_Read_Voltage(BMSTypeDef *BMS) {
    uint16_t totalVoltage = 0;
    for (uint8_t i = 0; i < Battery_Count; i++) {
        uint16_t voltage = AFE_Read_BATT_Voltage(i);
        if (voltage == 0xFFFF) 
        {
            // 读取失败
            return BMS_Error;
        }
        BMS->Status.BAT_Voltage[i] = voltage;
        totalVoltage += voltage;
    }
    // 计算总电压
    BMS->Status.Pack_Voltage = totalVoltage;

    // 读取成功
    return BMS_OK;
}

/**
 * @brief   读取电池电流
 *
 * @param   Bms_info  电池管理系统信息结构体指针，用于保存读取到的电流信息
 *
 * @return  BMS_ReturnTypeDef 返回读取状态，BMS_OK 表示读取成功，BMS_Error 表示读取失败
 */
BMS_ReturnTypeDef BMS_Read_Current(BMSTypeDef *BMS) {
    float current = AFE_Read_Current();
    if (current == 0.0f) {
        // 读取失败
        return BMS_Error;
    }
    // 将电流存储在 bms_info 结构体的 pack_i 成员中
    BMS->Status.Current = (uint16_t)(current * 100); // 将电流放大 100 倍并转换成 uint16_t 类型
    // 读取成功
    return BMS_OK;
}

/**
 * @brief   读取电池温度
 *
 * @param   BMS_Status  电池管理系统信息结构体指针，用于保存读取到的温度信息
 *
 * @return  BMS_ReturnTypeDef 返回读取状态，BMS_OK 表示读取成功，BMS_Error 表示读取失败
 */
BMS_ReturnTypeDef BMS_Read_Temp(BMSTypeDef *BMS) {
    for (uint8_t i = 0; i < Temp_Count; i++) {
        float temperature = AFE_Read_TEMP(i);
        // 如果温度为 0xFF，表示读取失败
        if (temperature == 0xFF) {
            return BMS_Error;
        }
        // 将读取的温度存储在 bms_info 结构体的 pack_t 数组中
        BMS->Status.Temperature[i] = temperature ;
    }
    // 读取成功
    return BMS_OK;
}

/**
 * @brief   读取开关量
 *
 * @param   BMS_Status  电池管理系统状态结构体指针，用于保存读取到的开关量信息
 *
 * @return  BMS_ReturnTypeDef 返回读取状态，BMS_OK 表示读取成功，BMS_Error 表示读取失败
 */
BMS_ReturnTypeDef BMS_Read_SwitchStatus(BMSTypeDef *BMS)
{
    // 读取寄存器状态字节
    uint8_t status_byte = SH36730X_READ_BSTATUS();
    
    // 检查是否成功读取了状态字节
    if (status_byte == 0xFF) 
    {
        return BMS_Error; // 读取失败
    }
    // 解析状态字节并更新 BMS_Status 结构体中的每个 SwitchStatus 变量
    BMS->Status.SwitchStatus[DSG] = (status_byte >> 5) & 0x01;    // DSG
    BMS->Status.SwitchStatus[CHG] = (status_byte >> 4) & 0x01;    // CHG
    BMS->Status.SwitchStatus[DSGING] = (status_byte >> 3) & 0x01; // DSGING
    BMS->Status.SwitchStatus[CHGING] = (status_byte >> 2) & 0x01; // CHGING
    BMS->Status.SwitchStatus[LOAD] = (status_byte >> 1) & 0x01;   // LOAD
    BMS->Status.SwitchStatus[CHGR] = status_byte & 0x01;          // CHGR
    return BMS_OK; // 读取成功
}


BMS_ReturnTypeDef BMS_Read_CBStatus(BMSTypeDef *BMS) {
    // 调用读取均衡通道状态的函数
    if (SH36730X_Read_CBStatus(BMS->Status.CBxStatus) != SH367303_OK) {
        return BMS_Error; // 如果读取失败，直接返回错误状态
    }

    return BMS_OK; // 读取成功
}

/**
 * @brief   读取电池状态信息
 *
 * @param   BMS_Status  电池管理系统信息结构体指针，用于保存读取到的状态信息
 *
 * @return  BMS_ReturnTypeDef 返回读取状态，BMS_OK 表示读取成功，BMS_Error 表示读取失败
 */
BMS_ReturnTypeDef BMS_Read_Status(BMSTypeDef *BMS) {

    // 读取电池电压并传入数组中
    if (BMS_Read_Voltage(BMS) != BMS_OK) {
        return BMS_Error; // 如果读取电压失败，直接返回错误状态
    }

    // 读取电池电流并传入数组中
    if (BMS_Read_Current(BMS) != BMS_OK) {
        return BMS_Error; // 如果读取电流失败，直接返回错误状态
    }

    // 读取温度并传入数组中
    if (BMS_Read_Temp(BMS) != BMS_OK) {
        return BMS_Error; // 如果读取温度失败，直接返回错误状态
    }

    // 读取开关量状态并传入数组中
    if (BMS_Read_SwitchStatus(BMS) != BMS_OK) {
        return BMS_Error; // 如果读取温度失败，直接返回错误状态
    }

    // 读取开关量状态并传入数组中
    if (BMS_Read_CBStatus(BMS) != BMS_OK) {
        return BMS_Error; // 如果读取温度失败，直接返回错误状态
    }

    // 读取所有信息成功，返回 OK 状态
    return BMS_OK;
}

/**
 * @brief   计算电池状态
 *
 * @param   BMS_Status  电池管理系统信息结构体指针，用于计算状态信息
 *
 * @return  BMS_ReturnTypeDef 返回状态计算结果，BMS_OK 表示无报警
 */
BMS_ReturnTypeDef BMS_Status_Cacluate(BMSTypeDef *BMS) {
//-----------------------单电池过压检测------------------------------------------*/
	for (int i = 0; i < Battery_Count; ++i) 
    {
        if (BMS->Status.BAT_Voltage[i] > BAT_OverChargeWarn) //过压预警判断
		{
            if (BMS->Status.BAT_Voltage[i] > BAT_OverChargeAlarm) //过压警告判断
            {
                BMS->State.V_Alarm[OverChargeAlarm][i] = true;
                BMS->State.V_Alarm[OverChargeWarn][i] = true;
            } 
            else 
            {
                BMS->State.V_Alarm[OverChargeWarn][i] = true;
            }
        } 
        if (BMS->State.V_Alarm[OverChargeWarn][i] || BMS->State.V_Alarm[OverChargeAlarm][i])//是否已产生标志
        {
            if (BMS->Status.BAT_Voltage[i] < BAT_OverChargeRecovery) //电池达到恢复阈值，清除报警
            { // 如果该节电池都低于恢复电压
                BMS->State.V_Alarm[OverChargeWarn][i] = false;
                BMS->State.V_Alarm[OverChargeAlarm][i] = false;
            }
        }
    }
//-----------------------单电池欠压检测------------------------------------------*/
	for (int i = 0; i < Battery_Count; ++i) 
    {
        if (BMS->Status.BAT_Voltage[i] < BAT_OverDischargeWarn) //欠压预警判断
		{
            if (BMS->Status.BAT_Voltage[i] < BAT_OverDischargeAlarm) //欠压警告判断
            {
                BMS->State.V_Alarm[OverDischargeAlarm][i] = true;
                BMS->State.V_Alarm[OverDischargeWarn][i] = true;
            } 
            else 
            {
                BMS->State.V_Alarm[OverDischargeWarn][i] = true;
            }
        } 
        if (BMS->State.V_Alarm[OverDischargeWarn][i] || BMS->State.V_Alarm[OverChargeAlarm][i])//是否已产生标志
        {
            if (BMS->Status.BAT_Voltage[i] > BAT_OverDischargeRecovery) //电池达到恢复阈值，清除报警
            { // 如果该节电池都高于恢复电压
                BMS->State.V_Alarm[OverDischargeWarn][i] = false;
                BMS->State.V_Alarm[OverDischargeAlarm][i] = false;
            }
        }
    }
//-----------------------充电 高温、低温监测------------------------------------------*/
if (BMS->Status.SwitchStatus[CHGING])//充电状态 CADC检测到Sense电阻两端(RS2-RS1)电压≤-VCD；
{
	for (int i = 0; i < Temp_Count; ++i) //高温检测
    {
        if (BMS->Status.Temperature[i] > BAT_HighChargeTemperatureWarn) //充电 高温预警判断
		{
            if (BMS->Status.Temperature[i] > BAT_HighChargeTemperatureAlarm) //充电 高温警告判断
            {
                BMS->State.I_Alarm[ChargeHighTemperatureAlarm][i] = true;
                BMS->State.I_Alarm[ChargeHighTemperatureWarn][i] = true;
            } 
            else 
            {
                BMS->State.I_Alarm[ChargeHighTemperatureWarn][i] = true;
            }
        } 
        if (BMS->State.I_Alarm[ChargeHighTemperatureWarn][i] || BMS->State.I_Alarm[ChargeHighTemperatureAlarm][i])//是否已产生标志
        {
            if (BMS->Status.Temperature[i] < BAT_HighChargeTemperatureRecovery) 
            { // 如果该通道低于恢复阈值
                BMS->State.I_Alarm[ChargeHighTemperatureWarn][i] = false;
                BMS->State.I_Alarm[ChargeHighTemperatureAlarm][i] = false;
            }
        }
    }
	for (int i = 0; i < Temp_Count; ++i) //低温检测
    {
        if (BMS->Status.Temperature[i] < BAT_LowChargeTemperatureWarn) //充电 低温预警判断
		{
            if (BMS->Status.Temperature[i] < BAT_LowChargeTemperatureAlarm) //充电 低温警告判断
            {
                BMS->State.I_Alarm[ChargeLowTemperatureAlarm][i] = true;
                BMS->State.I_Alarm[ChargeLowTemperatureWarn][i] = true;
            } 
            else 
            {
                BMS->State.I_Alarm[ChargeLowTemperatureWarn][i] = true;
            }
        } 
        if (BMS->State.I_Alarm[ChargeLowTemperatureWarn][i] || BMS->State.I_Alarm[ChargeLowTemperatureAlarm][i])//是否已产生标志
        {
            if (BMS->Status.Temperature[i] > BAT_LowChargeTemperatureRecovery)
            { // 如果所有电池都低于恢复电压
                BMS->State.I_Alarm[ChargeLowTemperatureWarn][i] = false;
                BMS->State.I_Alarm[ChargeLowTemperatureAlarm][i] = false;
            }
        }
    }
}
//-----------------------放电 高温、低温监测------------------------------------------*/
if (BMS->Status.SwitchStatus[DSGING])//放电状态 CADC检测到Sense电阻两端(RS2-RS1)电压≥VCD；
{
	for (int i = 0; i < Temp_Count; ++i) //高温检测
    {
        if (BMS->Status.Temperature[i] > BAT_HighDischargeTemperatureWarn) //放电 高温预警判断
		{
            if (BMS->Status.Temperature[i] > BAT_HighDischargeTemperatureAlarm) //放电 高温警告判断
            {
                BMS->State.I_Alarm[DischargeHighTemperatureAlarm][i] = true;
                BMS->State.I_Alarm[DischargeHighTemperatureWarn][i] = true;
            } 
            else 
            {
                BMS->State.I_Alarm[DischargeHighTemperatureWarn][i] = true;
            }
        } 
        if (BMS->State.I_Alarm[DischargeHighTemperatureWarn][i] || BMS->State.I_Alarm[DischargeHighTemperatureAlarm][i])//是否已产生标志
        {
            if (BMS->Status.Temperature[i] > BAT_HighDischargeTemperatureRecovery) 
            { // 如果所有电池都低于恢复电压
                BMS->State.I_Alarm[DischargeHighTemperatureWarn][i] = false;
                BMS->State.I_Alarm[DischargeHighTemperatureAlarm][i] = false;
            }
        }
    }
	for (int i = 0; i < Temp_Count; ++i) //低温检测
    {
        if (BMS->Status.Temperature[i] < BAT_LowDischargeTemperatureWarn) //放电 低温预警判断
		{
            if (BMS->Status.Temperature[i] < BAT_LowDischargeTemperatureAlarm) //放电 低温警告判断
            {
                BMS->State.I_Alarm[DischargeLowTemperatureAlarm][i] = true;
                BMS->State.I_Alarm[DischargeLowTemperatureWarn][i] = true;
            } 
            else 
            {
                BMS->State.I_Alarm[DischargeLowTemperatureWarn][i] = true;
            }
        } 
        if (BMS->State.I_Alarm[DischargeLowTemperatureWarn][i] || BMS->State.I_Alarm[DischargeLowTemperatureAlarm][i])//是否已产生标志
        {
            if (BMS->Status.Temperature[i] < BAT_LowDischargeTemperatureRecovery) 
            { // 如果所有电池都低于恢复电压
                BMS->State.I_Alarm[DischargeLowTemperatureWarn][i] = false;
                BMS->State.I_Alarm[DischargeLowTemperatureAlarm][i] = false;
            }
        }
    }
}
    // 如果没有任何警告或报警，返回 BMS_OK
    return BMS_CheckAlarm(BMS);
}

/**
 * @brief   计算报警
 *
 * @param   BMS_Status  电池管理系统信息结构体指针，用于计算状态信息
 *
 * @return  BMS_ReturnTypeDef 返回状态计算结果，BMS_OK 表示无报警
 */
BMS_ReturnTypeDef BMS_CheckAlarm(BMSTypeDef *BMS) {
    bool hasAlarm = false;
    bool hasWarning = false;

    // 检查电压报警
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < Battery_Count; ++j) {
            if (BMS->State.V_Alarm[i][j]) {
                if (i == OverChargeAlarm || i == OverDischargeAlarm) {
                    hasAlarm = true;
                    #ifdef LOG_OUTPUT
                    printf("Cell %d %s Alarm!\n", j, (i == OverChargeAlarm) ? "Overcharge" : "Overdischarge");
                    #endif
                } else if (i == OverChargeWarn || i == OverDischargeWarn) {
                    hasWarning = true;
                    #ifdef LOG_OUTPUT
                    printf("Cell %d %s Warning!\n", j, (i == OverChargeWarn) ? "Overcharge" : "Overdischarge");
                    #endif
                }
            }
        }
    }

    // 检查电流报警
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < Temp_Count; ++j) {
            if (BMS->State.I_Alarm[i][j]) {
                if (i == OverChargeCurrentAlarm || i == OverDischargeCurrentAlarm) {
                    hasAlarm = true;
                    #ifdef LOG_OUTPUT
                    printf("Channel %d %s Alarm!\n", j, (i == OverChargeCurrentAlarm) ? "Overcharge current" : "Overdischarge current");
                    #endif
                } else if (i == OverChargeCurrentWarn || i == OverDischargeCurrentWarn) {
                    hasWarning = true;
                    #ifdef LOG_OUTPUT
                    printf("Channel %d %s Warning!\n", j, (i == OverChargeCurrentWarn) ? "Overcharge current" : "Overdischarge current");
                    #endif
                }
            }
        }
    }

    // 检查温度报警
    for (int i = 0; i < 4; ++i) {
        if (BMS->State.T_Alarm[i]) {
            if (i == ChargeHighTemperatureAlarm || i == DischargeHighTemperatureAlarm ||
                i == ChargeLowTemperatureAlarm || i == DischargeLowTemperatureAlarm) {
                hasAlarm = true;
                #ifdef LOG_OUTPUT
                printf("Temperature %s Alarm!\n", (i == ChargeHighTemperatureAlarm || i == DischargeHighTemperatureAlarm) ? "High" : "Low");
                #endif
            } else if (i == ChargeHighTemperatureWarn || i == DischargeHighTemperatureWarn ||
                       i == ChargeLowTemperatureWarn || i == DischargeLowTemperatureWarn) {
                hasWarning = true;
                #ifdef LOG_OUTPUT
                printf("Temperature %s Warning!\n", (i == ChargeHighTemperatureWarn || i == DischargeHighTemperatureWarn) ? "High" : "Low");
                #endif
            }
        }
    }

    return (hasAlarm ? BMS_Alarm : (hasWarning ? BMS_Warning : BMS_OK));
}




/**
 * @brief   计算电池荷电状态SOC
 *
 * @param   BMS_Status  指向电池管理系统信息结构体的指针，用于获取当前状态信息
 * @param   BMS_State   指向动力电池状态信息结构体的指针，用于更新SOC值
 *
 * @return  BMS_ReturnTypeDef 返回状态计算结果，BMS_OK 表示更新成功，BMS_Error 表示更新失败
 */
BMS_ReturnTypeDef BMS_SOC_Update(BMSTypeDef *BMS) {
    // 获取当前时间
    uint32_t current_time = HAL_GetTick();

    // 计算时间间隔
    uint32_t time_diff = current_time - BMS->State.Last_Update;

    // 如果时间间隔小于 SOC 最小更新间隔，则不进行更新
    if (time_diff < UPDATE_INTERVAL) {
        return BMS_Error; // 尚未到达更新间隔，不进行更新
    }

    // 计算充电/放电的能量变化（单位：mAh）
    float delta_energy = ((BMS->Status.Current >= 0) ? (BMS->Status.Current - SYSTEM_POWER_CONSUMPTION) : (BMS->Status.Current + SYSTEM_POWER_CONSUMPTION)) * time_diff / 3600.0; // 电流减去系统功耗（充电时），加上系统功耗（放电时），乘以时间得到电量，除以3600转换为mAh

    // 更新充电/放电累计能量
    if (BMS->Status.Current >= 0) {
        // 充电状态，累加充电能量
        BMS->State.ChargedPower += delta_energy;
    } else {
        // 放电状态，累加放电能量
        BMS->State.DischargedPower -= delta_energy; // 电流为负数，取其绝对值，同时考虑电池自衰减系数
    }

    // 更新 SOC
    float remaining_capacity = BATTERY_CAPACITY - BMS->State.DischargedPower + BMS->State.ChargedPower; // 计算剩余容量（mAh）
    BMS->State.SOC = (remaining_capacity / BATTERY_CAPACITY) * 100; // 计算 SOC

    // 限制 SOC 在 0 到 100 之间
    if (BMS->State.SOC > 100) {
        BMS->State.SOC = 100;
    }

    // 更新上次更新时间
    BMS->State.Last_Update = current_time;

    return BMS_OK; // 更新成功
}

/**
 * @brief   计算电池健康状态SOH
 *
 * @param   BMS_Status  指向电池管理系统信息结构体的指针，用于获取当前状态信息
 * @param   BMS_State   指向动力电池状态信息结构体的指针，用于更新SOC值
 *
 * @return  BMS_ReturnTypeDef 返回状态计算结果，BMS_OK 表示更新成功，BMS_Error 表示更新失败
 */
BMS_ReturnTypeDef BMS_SOH_Update(BMSTypeDef *BMS) {
    // 计算总循环次数（充电和放电）
    uint32_t total_cycle_count = BMS->State.ChargedPower + BMS->State.DischargedPower;

    // 查找循环次数对应的 SOH 值
    uint16_t calculated_soh = 100; // 默认为 100%
    for (int i = 0; i < 9; i++) {
        if (total_cycle_count >= SOH_Table[i].cycle_count && total_cycle_count < SOH_Table[i + 1].cycle_count) {
            // 使用线性插值计算 SOH
            calculated_soh = SOH_Table[i].soh + ((SOH_Table[i + 1].soh - SOH_Table[i].soh) * (total_cycle_count - SOH_Table[i].cycle_count)) / (SOH_Table[i + 1].cycle_count - SOH_Table[i].cycle_count);
            break;
        }
    }

    // 更新 SOH
    BMS->State.SOH = calculated_soh;

    return BMS_OK; // 更新成功
}

/**
 * @brief   将 BMS 状态信息转换为 MCU 使用的数据格式
 *
 * @param   BMS_Status  指向 BMS 状态信息结构体的指针，包含要转换的状态信息
 * @param   BMS_State   指向 BMS 状态信息结构体的指针，包含要转换的状态信息
 * @param   Bms_info    指向 MCU 使用的数据结构体的指针，用于存储转换后的数据
 *
 * @return  BMS_ReturnTypeDef 返回转换结果，BMS_OK 表示转换成功，BMS_Error 表示转换失败
 */
BMS_ReturnTypeDef BMS_to_MCU(BMSTypeDef *BMS , bms_info *Bms_info) {
    // 检查指针是否为空
    if (BMS == NULL || Bms_info == NULL) {
        return BMS_Error; // 指针为空，转换失败
    }

    // 复制电池电压
    for (int i = 0; i < Battery_Count; ++i) {
        Bms_info->batter_v[i] = BMS->Status.BAT_Voltage[i];
    }

    // 复制电池组电压、电流、温度
    Bms_info->pack_v = BMS->Status.Pack_Voltage;
    Bms_info->pack_i = BMS->Status.Current;
    for (int i = 0; i < Temp_Count; ++i) {
        Bms_info->pack_t[i] = BMS->Status.Temperature[i];
    }

    // 计算并保存充电功率和总放电功率到 EEPROM
    Bms_info->all_cha_power = BMS->Status.Pack_Voltage * BMS->Status.Current; // 充电功率 = 电池组电压 * 电流
    Bms_info->all_dis_power = BMS->Status.Pack_Voltage * abs(BMS->Status.Current); // 总放电功率 = 电池组电压 * 电流的绝对值

    // 复制剩余电量、当日累计充电量、当日累计放电量、放电 MOS 开关状态、BMS 报警
    Bms_info->soc = BMS->State.SOC;
    Bms_info->day_cha_power = BMS->State.ChargedPower; 
    Bms_info->day_dis_power = BMS->State.DischargedPower;
    Bms_info->bms_st = SH36730X_READ_BSTATUS(); 
    Bms_info->bms_alarm = BMS_CheckAlarm(BMS); 

    return BMS_OK; // 转换成功
}

/**
 * @brief   将 BMS 状态信息存储至EEPROM
 *
 * @param   BMS_State   指向 BMS 状态信息结构体的指针，包含要转换的状态信息
 *
 * @return  BMS_ReturnTypeDef 返回转换结果，BMS_OK 表示转换成功，BMS_Error 表示转换失败
 */
BMS_ReturnTypeDef BMS_StateSave(BMSTypeDef *BMS)
{
    // if(EEPROM_Write(BMS_State) != EEPROM_OK)
    // {
    //     return BMS_Error;
    // }
    return BMS_OK; 
}

BMS_ReturnTypeDef BMS_CHG(BMSTypeDef *BMS, bool *enable) {
    // 设置CHG
    AFE_CHG(enable);
    BMS_Read_SwitchStatus(BMS);
    int retry_count = 3; // 重试次数
    while (retry_count > 0) {
        if (BMS->Status.SwitchStatus[CHG] == ON) {
            // 如果开关状态匹配预期，返回BMS_OK
            return BMS_OK;
        } else {
            HAL_Delay(10); // 如果开关状态不匹配，等待一段时间后重新读取开关状态
            BMS_Read_SwitchStatus(BMS);
            retry_count--;
        }
    }
    // 重试次数用完仍然无法成功设置开关状态，返回BMS_Error
    return BMS_Error;
}

/**
 * @brief   控制放电状态
 *
 * @param   BMS  电池管理系统状态结构体指针，用于设置放电状态
 * @param   enable  是否开启放电，true表示开启，false表示关闭
 *
 * @return  BMS_ReturnTypeDef 返回操作状态，BMS_OK 表示成功，BMS_Error 表示失败
 */
BMS_ReturnTypeDef BMS_DSG(BMSTypeDef *BMS, bool *enable) {
    // 设置DSG
    AFE_DSG(enable);
    // 更新开关量
    BMS_Read_SwitchStatus(BMS);
    int retry_count = 3; // 重试次数
    while (retry_count > 0) {
        if (BMS->Status.SwitchStatus[DSG] == ON) {
            // 如果开关状态匹配预期，返回BMS_OK
            return BMS_OK;
        } else {
            HAL_Delay(10);  // 如果开关状态不匹配，等待一段时间后重新读取开关状态
            BMS_Read_SwitchStatus(BMS);
            retry_count--;
        }
    }
    // 重试次数用完仍然无法成功设置开关状态，返回BMS_Error
    return BMS_Error;
}


/**
 * @brief   将 MCU 控制数据写入 BMS 控制结构体
 *
 * @param   BMS  MCU 控制数据结构体，包含要写入的控制信息
 *
 * @return  BMS_ReturnTypeDef 返回写入结果，BMS_OK 表示写入成功，BMS_Error 表示写入失败
 */
BMS_ReturnTypeDef BMS_Control_Update(BMSTypeDef *BMS) {
    bool Error_flag = false;
    if (BMS_Read_SwitchStatus(BMS) != BMS_OK) {
        return BMS_Error; // 无法读取SH 直接退出
    }

    if(BMS->Control.CHG != BMS->Status.SwitchStatus[CHG])
    {
         if (AFE_CHG( BMS->Control.CHG) != BMS_OK)
         {
            Error_flag = true; //继续执行 但是有错误
         }
    }
    if(BMS->Control.DSG != BMS->Status.SwitchStatus[DSG])
    {
         if (AFE_DSG( BMS->Control.DSG) != BMS_OK)
         {
            Error_flag = true; //继续执行 但是有错误
         }
    }
    for (int i = 0; i < CELL_BALANCE_Count; ++i) 
        {
        if (AFE_Write_CB(i , BMS->Control.CBxControl[i]) != BMS_OK)
        {
            Error_flag = true; //继续执行 但是有错误
        }
    }
   // AFE_CTLD(BMS->Control.CTLD);
    //判断是否休眠
    if(BMS->Control.Sleep == ON)
        if(AFE_Sleep(BMS->Control.Sleep) != BMS_OK)
            return BMS_Error;//休眠失败

    if(Error_flag)
        return BMS_Error;//休眠失败
    return BMS_OK;
}

