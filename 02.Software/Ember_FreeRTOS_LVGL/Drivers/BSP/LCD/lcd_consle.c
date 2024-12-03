/********************************************************************************
    * 文件名称 lcd_consle.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD仿命令台输出
*********************************************************************************/
#include "lcd_consle.h"

// 显示缓冲区：总缓冲区大小为 buflen * bufrow
char LCD_printf_buf[buflen];    // 单条格式化数据缓冲区
size_t LCD_printf_bufsize = sizeof(LCD_printf_buf);  // 单条数据缓冲区的大小

// 将格式化的文本添加到 LCD_consle_buf 循环缓冲区
void lcdprintf_cicle_buff(char *text, uint16_t buf_size) {
    uint16_t text_len = strlen(text);
    uint16_t consle_len = strlen(g_LCD_consle_buf);  // 当前 LCD_consle_buf 中的内容长度

    if (text_len + consle_len >= buf_size) {
        // 如果新文本加上现有内容超出了缓冲区大小，截断原始内容
        consle_len = buf_size - text_len - 1;  // 保留终止符空间
    }

    // 将现有内容向后移动，为新文本腾出空间
    memmove(g_LCD_consle_buf + text_len, g_LCD_consle_buf, consle_len + 1);  // +1 保证 '\0' 被移动

    // 将新文本复制到缓冲区的头部
    memcpy(g_LCD_consle_buf, text, text_len);

    // 确保缓冲区以 '\0' 结尾
    g_LCD_consle_buf[text_len + consle_len] = '\0';

    g_LCD_text_flag = 1;  // 设置标志位，表示新的内容已准备好刷新
}

// 打印格式化文本并存储到显示缓冲区
int lcdprintf(const char *format, ...) {
    if (LCD_printf_buf == NULL || LCD_printf_bufsize == 0) {
        return -1;  // 错误：无效的缓冲区
    }

    va_list args;
    va_start(args, format);
    int written = vsnprintf(LCD_printf_buf, LCD_printf_bufsize, format, args);  // 格式化文本
    va_end(args);

    // 如果格式化失败或缓冲区溢出，处理错误
    if (written < 0 || (size_t)written >= LCD_printf_bufsize) {
        LCD_printf_buf[LCD_printf_bufsize - 1] = '\0';
        return LCD_printf_bufsize - 1;
    }

    // 将格式化后的文本添加到 LCD_consle_buf（显示缓冲区）
    lcdprintf_cicle_buff(LCD_printf_buf, sizeof(g_LCD_consle_buf));

    return written;
}
