#include "bootloader.h"
#include "usart.h"
#include "delay.h"

void print_boot_message(void)
{
    uart_log("---------- Enter BootLoader ----------\r\n");
    uart_log("\r\n");
    uart_log("======== flash pration table =========\r\n");
    uart_log("| name     | offset     | size       |\r\n");
    uart_log("--------------------------------------\r\n");
    uart_log("| boot     | 0x08000000 | 0x00004000 |\r\n");
    uart_log("| setting  | 0x08004000 | 0x00004000 |\r\n");
    uart_log("| app      | 0x08008000 | 0x00018000 |\r\n");
    uart_log("| download | 0x08020000 | 0x00020000 |\r\n");
    uart_log("======================================\r\n");
}

int main() 
{
    process_status process;
    uint16_t i;
    uint8_t boot_state;
    uint8_t down_buf[128];
    uint32_t down_addr;
    uint32_t app_addr;

    delay_init(168);
    uart_init(115200);
    print_boot_message();

    boot_parameter.process = read_setting_boot_state();
    boot_parameter.addr = APP_SECTOR_ADDR;

    while (1) 
    {
        process = get_boot_state();
        switch (process) 
        {
            case START_PROGRAM:
                uart_log("start app...\r\n");
                delay_ms(50);
                if (!jump_app(boot_parameter.addr)) 
                {
                    uart_log("no program\r\n");
                    delay_ms(1000);
                }
                uart_log("start app failed\r\n");
                break;
            case UPDATE_PROGRAM:
                uart_log("update app program...\r\n");
                app_addr = APP_SECTOR_ADDR;
                down_addr = DOWNLOAD_SECTOR_ADDR;

                uart_log("app addr: 0x%08X \r\n", app_addr);
                uart_log("down addr: 0x%08X \r\n", down_addr);

                uart_log("erase mcu flash...\r\n");
                mcu_flash_erase(app_addr, APP_ERASE_SECTORS_NUM);  
                uart_log("mcu flash erase success\r\n");
            
                uart_log("write mcu flash...\r\n");
                // memset(down_buf, 0, sizeof(down_buf));
                for (i = 0; i < (APP_SECTOR_SIZE / FLASH_SECTOR_SIZE); i++)
                {
                    mcu_flash_read(down_addr, &down_buf[0], FLASH_SECTOR_SIZE);
                    delay_ms(5);
                    mcu_flash_write(app_addr, &down_buf[0], FLASH_SECTOR_SIZE);
                    delay_ms(5);
                    down_addr += FLASH_SECTOR_SIZE;
                    app_addr += FLASH_SECTOR_SIZE;
                    // uart_log("mcu_flash_write: %d\r\n", i);
                }
                uart_log("mcu flash write success\r\n");

                set_boot_state(UPDATE_SUCCESS);
                break;
            case UPDATE_SUCCESS:
                uart_log("update success\r\n");
                boot_state = UPDATE_SUCCESS_STATE;
                write_setting_boot_state(boot_state);
                set_boot_state(START_PROGRAM);
                break;
            default:
                break;
        }
    }
}
