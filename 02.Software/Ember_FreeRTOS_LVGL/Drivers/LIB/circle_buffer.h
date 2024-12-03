/********************************************************************************
    * 文件名称 ：circle_buffer.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：环形缓冲区实现
*********************************************************************************/
#ifndef _CIRCLE_BUF_H
#define _CIRCLE_BUF_H

#include <stdint.h>

typedef struct circle_buf {
	uint32_t r;
	uint32_t w;
	uint32_t len;
	uint8_t *buf;
}circle_buf, *p_circle_buf;

#define BUF_SIZE 256 // 假设缓冲区的最大大小
extern uint8_t LCD_text_flag;

void LCDPrint(const char *text, char *bufs, uint16_t buf_size);

void circle_buf_init(p_circle_buf pCircleBuf, uint32_t len, uint8_t *buf);

int circle_buf_read(p_circle_buf pCircleBuf, uint8_t *pVal);

int circle_buf_write(p_circle_buf pCircleBuf, uint8_t val);

#endif /* _CIRCLE_BUF_H */
