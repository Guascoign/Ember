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

#define BUF_SIZE 256 // 假设缓冲区的最大大小

/**
 * @brief    环形缓冲区结构体
 * @param    r	读指针
 * @param    w	写指针
 * @param    len	缓冲区长度
 * @param    buf	缓冲区指针
 */
typedef struct Circle_buf {
	uint32_t r;
	uint32_t w;
	uint32_t len;
	uint8_t *buf;
}Circle_bufTypeDef, *p_Circle_bufTypeDef;

void circle_buf_init(p_Circle_bufTypeDef p_CircleBuf, uint32_t len, uint8_t *buf);
int circle_buf_read(p_Circle_bufTypeDef p_CircleBuf, uint8_t *pVal);
int circle_buf_write(p_Circle_bufTypeDef p_CircleBuf, uint8_t val);

#endif /* _CIRCLE_BUF_H */
