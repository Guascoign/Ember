/********************************************************************************
    * 文件名称 ：lcd.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#ifndef __LCD_H
#define __LCD_H

#include "gpio.h"
#include "spi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* 选择硬件SPI通道 */
#define LCD_SPI_PORT hspi1
extern SPI_HandleTypeDef LCD_SPI_PORT;
/* choose whether use DMA or not */
//#define USE_DMA
/* 是否使用硬件片选信号 */
#define USE_SOFT_CS
/* 选择驱动 */
#define USING_ST7789VW
//Sitronix https://www.alldatasheet.com/datasheet-pdf/pdf/1663350/SITRONIX/ST7789VW.html

#ifdef USING_ST7789VW
	#include "st7789.h"
  #define LCD_Init ST7789V_Init
	#define LCD_Init ST7789V_Init
#endif

/* 编码格式 */
//#define LCD_COLOR_MODE_14bit 0x53    //  Not USED
#define LCD_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)
//#define LCD_COLOR_MODE_18bit 0x66    //  RGB666 (18bit)


#ifdef LCD_COLOR_MODE_16bit
	#define LCD_TOTAL_BUF_SIZE    (LCD_Height*LCD_Width*2)
#endif
#ifdef LCD_COLOR_MODE_18bit
	#define LCD_TOTAL_BUF_SIZE    (LCD_Height*LCD_Width*3)
#endif

#define LCD_Buf_Page    (LCD_Buf_Size / LCD_TOTAL_BUF_SIZE)	// 计算LCD缓冲区页数
#define LCD_Buf_Size    648	// 单页缓存大小，不得超过MCU_SRAM
static uint8_t lcd_buf[LCD_Buf_Size];

/*
    LCD_PWR:    LCD背光
    LCD_RST:    SPI复位
    LCD_DC:     SPI数据
    LCD_CS:     SPI片选
*/
//n=1 高	n=0 低
#define LCD_PWR(n)   (n?HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET))//0关闭背光 1打开背光
#define LCD_RST(n)   (n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define LCD_DC(n)    (n?HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))  
#ifdef USE_SOFT_CS
#define LCD_CS(n)    (n?HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET))
#endif

/* 笔刷颜色 */
#define WHITE           0xFFFF    // 白色
#define BLACK           0x0000    // 黑色
#define BLUE            0x001F    // 蓝色
#define BRED            0XF81F    // 棕红色
#define GRED            0XFFE0    // 浅灰色
#define GBLUE           0X07FF    // 灰蓝色
#define RED             0xF800    // 红色
#define MAGENTA         0xF81F    // 紫红色
#define GREEN           0x07E0    // 绿色
#define CYAN            0x7FFF    // 青色
#define YELLOW          0xFFE0    // 黄色
#define BROWN           0XBC40    // 棕色
#define BRRED           0XFC07    // 棕红色
#define GRAY            0X8430    // 灰色
#define DARKBLUE        0X01CF    // 深蓝色
#define LIGHTBLUE       0X7D7C    // 浅蓝色
#define GRAYBLUE        0X5458    // 灰蓝色
#define LIGHTGREEN      0X841F    // 浅绿色
#define LGRAY           0XC618    // 浅灰色
#define LGRAYBLUE       0XA651    // 浅灰蓝色
#define LBBLUE          0X2B12    // 浅棕蓝色

extern uint16_t POINT_COLOR;        //默认笔刷颜色
extern uint16_t BACK_COLOR;        //默认背景颜色

void LCD_DisplayOn(void);                                                                //Open display
void LCD_DisplayOff(void);                                                                //Close display
void LCD_Write_buff(uint8_t *buff, uint16_t buff_size);
void LCD_Write_Cmd(uint8_t cmd);  
void LCD_Write_Data(uint8_t data);  	
void LCD_Clear(uint16_t color);                                                            //Clean screen
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color);                //Filled monochrome
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                //Setting up the data display area

#endif
