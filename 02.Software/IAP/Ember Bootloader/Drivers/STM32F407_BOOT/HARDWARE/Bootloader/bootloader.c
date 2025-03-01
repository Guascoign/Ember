#include "bootloader.h"

boot_t boot_parameter = {START_PROGRAM, 0, 0, 0};

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

uint16_t get_mcu_flash_sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)
        return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)
        return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)
        return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)
        return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)
        return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)
        return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)
        return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)
        return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)
        return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)
        return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)
        return FLASH_Sector_10; 
    else
	return FLASH_Sector_11;	
}

uint8_t mcu_flash_erase(uint32_t addr, uint8_t sector_num) 
{
	uint8_t i;
    uint32_t sector_addr;
	FLASH_Unlock();
	sector_addr = get_mcu_flash_sector(addr);
	for (i = 0; i < sector_num; ++i) 
	{
		FLASH_EraseSector(sector_addr, VoltageRange_3);
		if(sector_addr < FLASH_Sector_10)
			sector_addr += 0x08;
	}
	FLASH_Lock();
	return 1;
}

uint8_t mcu_flash_write(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
	FLASH_Status result;
	uint16_t i, data = 0;
	FLASH_Unlock();
	for (i = 0; i < length; i += 2) 
	{
		data = (*(buffer + i + 1) << 8) + (*(buffer + i));
		result = FLASH_ProgramHalfWord((uint32_t)(addr + i), data);
	}
	FLASH_Lock();

	if(result != FLASH_COMPLETE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
 
void mcu_flash_read(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
	memcpy(buffer, (void *)addr, length);
}

void set_boot_state(process_status process) 
{
    boot_parameter.process = process;
}

process_status get_boot_state(void) 
{
    process_status process;
    process = boot_parameter.process;

    return process;
}

process_status read_setting_boot_state(void)
{
	process_status process;
	uint8_t boot_state;
    mcu_flash_read(SETTING_BOOT_STATE, &boot_state, 1);
 
    if(boot_state != UPDATE_PROGRAM_STATE)
    {
       process = START_PROGRAM;
    }
    else
    {
       process = UPDATE_PROGRAM;
    }
	return process;
}

uint8_t write_setting_boot_state(uint8_t boot_state)
{
	uint8_t result;
	result = mcu_flash_erase(SETTING_BOOT_STATE, 1);
	if(result)
	{
		result = mcu_flash_write(SETTING_BOOT_STATE, &boot_state, 1);
		if(result != 1)
		{
			return result;
		}
	}
    
	return result;
}
