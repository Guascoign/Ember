/********************************************************************************
    * 文件名称 ：st7789.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#ifndef __ST7789_H
#define __ST7789_H

#include "LCD/lcd.h"

//extern  void LCD_Write_Cmd(uint8_t cmd);
//extern  void LCD_Write_Data(uint8_t data);

/* 选择分辨率 */
#define USING_135X240
//#define USING_240X240
//#define USING_170X320

/* 选择旋转方向: (0-3) */
#define ST7789_ROTATION 2

#ifdef USING_135X240

    #if ST7789_ROTATION == 0
        #define LCD_Width 135
        #define LCD_Height 240
        #define X_SHIFT 52
        #define Y_SHIFT 40
    #endif

    #if ST7789_ROTATION == 1
        #define LCD_Width 135
        #define LCD_Height 240
        #define X_SHIFT 53
        #define Y_SHIFT 40
    #endif

    #if ST7789_ROTATION == 2
        #define LCD_Width 240
        #define LCD_Height 135
        #define X_SHIFT 40
        #define Y_SHIFT 53
    #endif

    #if ST7789_ROTATION == 3
        #define LCD_Width 240
        #define LCD_Height 135
        #define X_SHIFT 40
        #define Y_SHIFT 52
    #endif

#endif	//end #ifdef USING_135X240

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36


void ST7789V_Init(void);

#endif



