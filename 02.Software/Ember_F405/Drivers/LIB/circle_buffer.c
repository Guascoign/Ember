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

/**
 * @brief    初始化环形缓冲区
 *
 * @param   p_CircleBuf	环形缓冲区句柄
 * @param   len	缓冲区长度
 * @param   buf	缓冲区指针
 *
 * @return  void
 */
void circle_buf_init(p_Circle_bufTypeDef p_CircleBuf, uint32_t len, uint8_t *buf)
{
	p_CircleBuf->r = p_CircleBuf->w = 0;//初始化读写指针和长度
	p_CircleBuf->len = len;//初始化缓冲区长度
	p_CircleBuf->buf = buf;//初始化缓冲区指针
}

/**
 * @brief    读取环形缓冲区
 *
 * @param   p_CircleBuf	环形缓冲区句柄
 * @param   pVal	读取数据指针
 *
 * @return  int 0:成功 1:失败
 */
int circle_buf_read(p_Circle_bufTypeDef p_CircleBuf, uint8_t *pVal)
{
	if (p_CircleBuf->r != p_CircleBuf->w)
	{
		*pVal = p_CircleBuf->buf[p_CircleBuf->r];
		
		p_CircleBuf->r++;
		
		if (p_CircleBuf->r == p_CircleBuf->len)
			p_CircleBuf->r = 0;
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief    写入环形缓冲区
 *
 * @param   p_CircleBuf	环形缓冲区句柄
 * @param   val	写入数据
 *
 * @return  int 0:成功 1:失败
 */
int circle_buf_write(p_Circle_bufTypeDef p_CircleBuf, uint8_t val)
{
	uint32_t next_w;
	
	next_w = p_CircleBuf->w + 1;
	if (next_w == p_CircleBuf->len)
		next_w = 0;
	
	if (next_w != p_CircleBuf->r)
	{
		p_CircleBuf->buf[p_CircleBuf->w] = val;
		p_CircleBuf->w = next_w;
		return 0;
	}
	else
	{
		return 1;
	}
}
