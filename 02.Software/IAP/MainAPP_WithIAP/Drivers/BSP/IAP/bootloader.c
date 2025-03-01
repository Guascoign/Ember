/********************************************************************************
    * 文件名称 ：bootloader.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-03-01
    * 功     能：bootloader固件升级
*********************************************************************************
V1.0 2024-01-30 add STM32 HAL
*********************************************************************************/
#include "BSP/IAP/bootloader.h"

boot_t boot_parameter = {START_PROGRAM, 0, 0, 0};

//跳转到APP
uint8_t jump_app(uint32_t app_addr) 
{
    uint32_t jump_addr;
    jump_callback cb;
    if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000) 
	{  
        jump_addr = *(__IO uint32_t*) (app_addr + 4);  
        cb = (jump_callback)jump_addr;  
        __set_MSP(*(__IO uint32_t*)app_addr);  
        cb();
        return 1;
    } 
    return 0;
}

//获取扇区
uint16_t Get_MCU_Flash_Sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)
        return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)
        return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)
        return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)
        return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)
        return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)
        return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)
        return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)
        return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)
        return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)
        return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)
        return FLASH_SECTOR_10; 
    else
	return FLASH_SECTOR_11;	
}

//擦除扇区
uint8_t MCU_Flash_Erase(uint32_t addr, uint8_t sector_num) 
{
    uint32_t sector_error;
    uint32_t sector_addr;

    HAL_FLASH_Unlock();

    // 获取要擦除的起始扇区
    sector_addr = Get_MCU_Flash_Sector(addr);

    // 配置擦除结构体
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 根据你的实际情况选择电压范围
    EraseInitStruct.Sector = sector_addr;//要擦除的扇区
    EraseInitStruct.NbSectors = sector_num;//要擦除的扇区数量

    // 擦除指定的扇区
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &sector_error) != HAL_OK)
    {
        // 擦除失败，处理错误
        HAL_FLASH_Lock();
        return 0;
    }

    HAL_FLASH_Lock();
    return 1;
}

//写入数据
uint8_t Mcu_Flash_Write(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
    uint32_t i;
    uint16_t data;

    HAL_FLASH_Unlock();

    for (i = 0; i < length; i += 2) 
    {
        // 将两个字节合并为一个16位数据
        data = (buffer[i + 1] << 8) | buffer[i];
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, data) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return 0; // 写入失败
        }
    }
    HAL_FLASH_Lock();
    return 1; // 写入成功
}

//读取数据
void Mcu_Flash_Read(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
	memcpy(buffer, (void *)addr, length);
}

//打印flash扇区
void Print_Flash_Sector(void)
{
    printf("======================================\r\n");
    printf("| Name     | Offset     | Size       |\r\n");
    printf("--------------------------------------\r\n");
    printf("| Boot     | %#010x | %#x |\r\n", BOOT_SECTOR_ADDR, BOOT_SECTOR_SIZE); // bootloader分区
    printf("| Setting  | %#010x | %#x |\r\n", SETTING_SECTOR_ADDR, SETTING_SECTOR_SIZE); // bootloader参数分区
    printf("| App      | %#010x | %#x |\r\n", APP_SECTOR_ADDR, APP_SECTOR_SIZE); // 主程序分区
    printf("| Update   | %#010x | %#x |\r\n", DOWNLOAD_SECTOR_ADDR, DOWNLOAD_SECTOR_SIZE); // 下载程序分区
    printf("| Backup   | %#010x | %#x |\r\n", BACKUP_SECTOR_ADDR, BACKUP_SECTOR_SIZE); // 备份程序分区
    printf("======================================\r\n");
}
