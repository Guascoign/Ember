#ifndef __BOOTLOARDER_H
#define __BOOTLOARDER_H

#include "string.h" 
#include "stm32f4xx.h" 
#include "main.h" 

/******************************************************** 
| 0x08000000 | 0x08004000 |  0x08008000  |  0x08020000 |   
---------------------------------------------------------
|    BOOT    |  SETTING   |      APP     |   DOWNLOAD  |
---------------------------------------------------------
|    16k     |     16K     |    256K     |     128K     |   
*********************************************************/

#define BOOT_DEBUG
#ifdef BOOT_DEBUG
#define BOOT_LOG printf
#else
#define BOOT_LOG 
#endif

#define FLASH_SECTOR_SIZE           1024
#define FLASH_SECTOR_NUM            512    // 512K
#define FLASH_START_ADDR            ((uint32_t)0x8000000)
#define FLASH_END_ADDR              ((uint32_t)(0x8000000 + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))

//flash sector addr
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

#define BOOT_SECTOR_ADDR            0x08000000
#define BOOT_SECTOR_SIZE            0x4000  //16K
#define SETTING_SECTOR_ADDR         (BOOT_SECTOR_ADDR + BOOT_SECTOR_SIZE)
#define SETTING_SECTOR_SIZE         0x4000  //16K
#define APP_SECTOR_ADDR             (SETTING_SECTOR_ADDR + SETTING_SECTOR_SIZE)
#define APP_SECTOR_SIZE             0x40000 //256K
#define DOWNLOAD_SECTOR_ADDR        (APP_SECTOR_ADDR + APP_SECTOR_SIZE)
#define DOWNLOAD_SECTOR_SIZE        0x40000 //256K

#define BOOT_ERASE_SECTORS_NUM      1  // 16k
#define SETTING_ERASE_SECTORS_NUM   1  // 16k
#define APP_ERASE_SECTORS_NUM       3  // 16k + 16k + 64k
#define DOWNLOAD_ERASE_SECTORS_NUM  1  // 128k

#define MaxQueueSize                1200

#define SETTING_BOOT_STATE          0x08004000
#define UPDATE_PROGRAM_STATE        2
#define UPDATE_SUCCESS_STATE        3

typedef enum 
{
    NONE,
    START_PROGRAM,
    UPDATE_PROGRAM,
    UPDATE_SUCCESS,
} process_status;

typedef void (*boot_callback)(process_status);

typedef struct 
{
    process_status process;
    uint8_t status;
    uint8_t id;
    uint32_t addr;
} boot_t;

extern boot_t boot_parameter;

//顺序循环队列的结构体定义如下：
typedef struct
{
    uint8_t queue[MaxQueueSize];
    int rear;  //队尾指针
    int front;  //队头指针
    int count;  //计数器
} SeqCQueue; 

typedef void (*jump_callback)(void);

uint8_t jump_app(uint32_t appAddr);
uint8_t mcu_flash_erase(uint32_t addr, uint8_t sector_num);
uint8_t mcu_flash_write(uint32_t addr, uint8_t *buffer, uint32_t length);
void mcu_flash_read(uint32_t addr, uint8_t *buffer, uint32_t length);
void set_boot_state(process_status process);
process_status get_boot_state(void);
process_status read_setting_boot_state(void);
uint8_t write_setting_boot_state(uint8_t boot_state);

#endif
