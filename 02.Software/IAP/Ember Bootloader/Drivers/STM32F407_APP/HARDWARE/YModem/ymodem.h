#ifndef __YMODEM_H
#define __YMODEM_H

#include "string.h" 
#include "stm32f4xx.h" 
#include "usart.h"	 
#include "rs485.h"	 
#include "main.h"

#define YMODEM_SOH		0x01
#define YMODEM_STX		0x02
#define YMODEM_EOT		0x04
#define YMODEM_ACK		0x06
#define YMODEM_NAK		0x15
#define YMODEM_CA		0x18
#define YMODEM_C		0x43

#define FLASH_SECTOR_SIZE       1024
#define FLASH_SECTOR_NUM        512    // 512K
#define FLASH_START_ADDR        ((uint32_t)0x08000000)
#define FLASH_END_ADDR          ((uint32_t)(0x08000000 + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))

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

#define DOWNLOAD_SECTOR_ADDR    0x08020000     // Download sector start address
#define ERASE_SECTORS	        1              // 128k

#define MAX_QUEUE_SIZE            1200

#define SETTING_BOOT_STATE      0x08004000
#define UPDATE_PROGRAM_STATE    2
#define UPDATE_SUCCESS_STATE    3

typedef enum {
	NONE,
	BUSY,
	START_PROGRAM,
	UPDATE_PROGRAM,
	UPDATE_SUCCESS
} process_status;

typedef void (*ymodem_callback)(process_status);

typedef struct {
	process_status process;
	uint8_t status;
	uint8_t id;
	uint32_t addr;
	uint32_t filesize;
	char filename[32];
	ymodem_callback cb;
} ymodem_t;

//顺序循环队列的结构体定义如下：
typedef struct
{
	uint8_t queue[MAX_QUEUE_SIZE];
	int rear;  //队尾指针
	int front;  //队头指针
	int count;  //计数器
} seq_queue_t; 

typedef  void (*jump_callback)(void);

typedef struct 
{
	uint8_t data[1200];
	uint16_t len;
} download_buf_t;

extern seq_queue_t rx_queue;
extern download_buf_t down_buf;

void queue_initiate(seq_queue_t *Q);
int queue_not_empty(seq_queue_t *Q);
int queue_delete(seq_queue_t *Q, uint8_t *d);

uint8_t mcu_flash_erase(uint32_t addr, uint8_t sector_num);
uint8_t mcu_flash_write(uint32_t addr, uint8_t *buffer, uint32_t length);
void mcu_flash_read(uint32_t addr, uint8_t *buffer, uint32_t length);

void set_ymodem_status(process_status process);
process_status get_ymodem_status(void);
void ymodem_start(ymodem_callback cb);
void ymodem_recv(download_buf_t *p);
void ymodem_init(void);
void ymodem_handle(void);
void timer_init(void);

#endif
