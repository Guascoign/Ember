/********************************************************************************
    * 文件名称 lcd_consle.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD仿命令台输出
*********************************************************************************/
#ifndef __LCD_CONSLE_H
#define __LCD_CONSLE_H

#include "LCD/lcd.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#define buflen 128     // 每行显示的字符数
#define bufrow 10      // 显示的行数（历史条数）


extern char g_LCD_consle_buf[buflen * bufrow];   // 显示缓冲区
extern uint8_t g_LCD_text_flag;

int lcdprintf(const char *format, ...);


#endif


