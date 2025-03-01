/********************************************************************************
    * 文件名称 ：bootloader.h
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-03-01
    * 功     能：iap固件升级
*********************************************************************************/
#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "string.h" 
#include "stm32f4xx.h" 
#include "main.h" 
#define STM32F405RG

#ifdef BOOT_DEBUG
#define BOOT_printf printf
#else
#define BOOT_printf
#endif


#ifdef STM32F405RG
#define MCU_NAME                    "STM32F405RG"
#define FLASH_START_ADDR            ((uint32_t)0x8000000)
#define FLASH_END_ADDR              ((uint32_t)0x807FFFF)
#define FLASH_SIZE                  1024
#define FREQUENCY                   168
#define RAM_SIZE                    192
#define RAM_START_ADDR              ((uint32_t)0x20000000)
#define FLASH_SECTOR_SIZE           1024
#endif


//flash sector addr
#ifdef STM32F405RG
#define ADDR_FLASH_SECTOR_0         ((uint32_t)0x08000000) 	//sector0 addr, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1         ((uint32_t)0x08004000) 	//sector1 addr, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2         ((uint32_t)0x08008000) 	//sector2 addr, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3         ((uint32_t)0x0800C000) 	//sector3 addr, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4         ((uint32_t)0x08010000) 	//sector4 addr, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5         ((uint32_t)0x08020000) 	//sector5 addr, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6         ((uint32_t)0x08040000) 	//sector6 addr, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7         ((uint32_t)0x08060000) 	//sector7 addr, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8         ((uint32_t)0x08080000) 	//sector8 addr, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9         ((uint32_t)0x080A0000) 	//sector9 addr, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10        ((uint32_t)0x080C0000) 	//sector10 addr,128 Kbytes  
#define ADDR_FLASH_SECTOR_11        ((uint32_t)0x080E0000) 	//sector11 addr,128 Kbytes  
#endif



#define BOOT_SECTOR_ADDR            FLASH_START_ADDR//0x08000000 Bootloader程序存储区
#define BOOT_SECTOR_SIZE            0x4000  //16K
#define SETTING_SECTOR_ADDR         (BOOT_SECTOR_ADDR + BOOT_SECTOR_SIZE)//0x08004000 Bootloader参数存储区
#define SETTING_SECTOR_SIZE         0x4000  //16K
#define APP_SECTOR_ADDR             (ADDR_FLASH_SECTOR_5)//0x08020000 主程序存储区
#define APP_SECTOR_SIZE             0x40000 //256K
#define DOWNLOAD_SECTOR_ADDR        (APP_SECTOR_ADDR + APP_SECTOR_SIZE)//0x08060000 下载程序存储区
#define DOWNLOAD_SECTOR_SIZE        0x40000 //256K
#define BACKUP_SECTOR_ADDR          (DOWNLOAD_SECTOR_ADDR + DOWNLOAD_SECTOR_SIZE)//0x08080000 备份程序存储区
#define BACKUP_SECTOR_SIZE          0x00000 //0K

#define BOOT_ERASE_SECTORS_NUM      1  // 16k
#define SETTING_ERASE_SECTORS_NUM   1  // 16k   存储boot_parameter
#define APP_ERASE_SECTORS_NUM       2  // 256K  第五扇区开始存储程序
#define DOWNLOAD_ERASE_SECTORS_NUM  2  // 256K
#define BACKUP_ERASE_SECTORS_NUM    0  // 256K

typedef enum
{
    NONE = 0,           // 无操作
    START_PROGRAM = 1,  // 启动程序 直接启动主程序
    UPDATE_PROGRAM = 2, // 更新程序 复制更新区程序至主程序后启动
    UPDATE_SUCCESS = 3, // 更新成功 更新程序成功
    UPDATE_ERROR = 4,   // 更新错误 更新程序失败
} Boot_status;

typedef struct boot_parameter
{
    Boot_status process;    //boot状态
    uint32_t app_addr;      //主程序地址
    uint32_t update_addr;   //更新程序地址
    uint32_t backup_addr;   //备份程序地址
    uint8_t retry_times;    //重试次数
} boot_parameterTypedef;

static FLASH_EraseInitTypeDef EraseInitStruct;//HAL库擦除结构体

typedef void (*jump_callback)(void);

uint8_t jump_app(uint32_t appAddr);
uint8_t MCU_Flash_Erase(uint32_t addr, uint8_t sector_num);
uint8_t MCU_Flash_Write(uint32_t addr, uint8_t *buffer, uint32_t length);
void MCU_Flash_Read(uint32_t addr, uint8_t *buffer, uint32_t length);
uint16_t Get_MCU_Flash_Sector(uint32_t addr);
void Print_Flash_Sector(void);
void Print_MCU_Info(void);
boot_parameterTypedef Get_boot_parameter(uint32_t addr);
void Write_boot_parameter(uint32_t addr, boot_parameterTypedef boot_parameter);


#endif  // __BOOTLOADER_H
