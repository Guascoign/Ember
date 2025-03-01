#include "main.h"
#include "usart.h"
#include "delay.h"
#include "ymodem.h"

#define APP_VERSION   "V100"

void print_boot_message(void)
{
    uart_log("======================================\r\n");
    uart_log("-------------- Enter APP -------------\r\n");
    uart_log ("app version is: %s\r\n", APP_VERSION);
    uart_log("======================================\r\n");
}

int main(void)
{
    delay_init(168);
    uart_init(115200);
    ymodem_init();
    print_boot_message();
    
    uart_log ("app init success\r\n");
    while (1)
    {
        ymodem_handle();
    }
}
