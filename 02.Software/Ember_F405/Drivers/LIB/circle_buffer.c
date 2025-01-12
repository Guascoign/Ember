/********************************************************************************
    * 文件名称 ：circle_buffer.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-11-15
    * 功     能：环形缓冲区实现
*********************************************************************************/
#include <stdint.h>
#include "circle_buffer.h"
#include <string.h>


void circle_buf_init(p_circle_buf pCircleBuf, uint32_t len, uint8_t *buf)
{
	pCircleBuf->r = pCircleBuf->w = 0;//初始化读写指针和长度
	pCircleBuf->len = len;//初始化缓冲区长度
	pCircleBuf->buf = buf;//初始化缓冲区指针
}

int circle_buf_read(p_circle_buf pCircleBuf, uint8_t *pVal)
{
	if (pCircleBuf->r != pCircleBuf->w)
	{
		*pVal = pCircleBuf->buf[pCircleBuf->r];
		
		pCircleBuf->r++;
		
		if (pCircleBuf->r == pCircleBuf->len)
			pCircleBuf->r = 0;
		return 0;
	}
	else
	{
		return -1;
	}
}

int circle_buf_write(p_circle_buf pCircleBuf, uint8_t val)
{
	uint32_t next_w;
	
	next_w = pCircleBuf->w + 1;
	if (next_w == pCircleBuf->len)
		next_w = 0;
	
	if (next_w != pCircleBuf->r)
	{
		pCircleBuf->buf[pCircleBuf->w] = val;
		pCircleBuf->w = next_w;
		return 0;
	}
	else
	{
		return -1;
	}
}
