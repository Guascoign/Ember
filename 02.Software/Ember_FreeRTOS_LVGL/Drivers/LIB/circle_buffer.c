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



void LCDPrint(const char *text, char *bufs, uint16_t buf_size) {
    /*
     * 将缓冲区内容向后移动 `text` 的长度，然后将 `text` 拼接到缓冲区头部。
     *
     * 参数：
     *     text: 要存储的文本字符串。
     *     bufs: 缓冲区，用于存储文本的字符数组。
     *     buf_size: 缓冲区的总大小。
     */

    // 获取文本长度
    uint16_t text_len = strlen(text);

    // 获取缓冲区中原始内容的长度
    uint16_t bufs_len = strlen(bufs);

    // 如果文本长度加原始内容超过缓冲区大小，截断原始内容
    if (text_len + bufs_len >= buf_size) {
        bufs_len = buf_size - text_len - 1; // 为 `text` 和终止符留出空间
    }

    // 将现有内容向后移动，为 `text` 腾出空间
    memmove(bufs + text_len, bufs, bufs_len + 1); // +1 复制 '\0'

    // 将 `text` 复制到缓冲区的头部
    memcpy(bufs, text, text_len);

    // 确保缓冲区以 `\0` 结束
    bufs[text_len + bufs_len] = '\0';

	LCD_text_flag = 1;
}
