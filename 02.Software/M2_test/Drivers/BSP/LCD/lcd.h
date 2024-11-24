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
#define LCD_SPI_PORT hspi2
extern SPI_HandleTypeDef LCD_SPI_PORT;

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

/* 缓存配置 */
#define MCU_SRAM        64000 //64KB STM32F103RCT6

#ifdef LCD_COLOR_MODE_16bit
	#define LCD_TOTAL_BUF_SIZE    (LCD_Height*LCD_Width*2)
#endif
#ifdef LCD_COLOR_MODE_18bit
	#define LCD_TOTAL_BUF_SIZE    (LCD_Height*LCD_Width*3)
#endif

#define LCD_Buf_Page    (LCD_Buf_Size / LCD_TOTAL_BUF_SIZE)	// 计算LCD缓冲区页数
#define LCD_Buf_Size    648	// 单页缓存大小，不得超过MCU_SRAM
static uint8_t lcd_buf[LCD_Buf_Size];

/* 是否使用DMA */
//#define USE_DMA

//#define NOT_SOFT_CS
/*
    LCD_PWR:    PB15
    LCD_RST:    PC7
    LCD_DC:     PC6
    LCD_CS:     PB12
*/
#define LCD_PWR(n)   (n?HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET))
#define LCD_RST(n)   (n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define LCD_DC(n)    (n?HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))
#ifndef NOT_SOFT_CS
#define LCD_CS(n)    (n?HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET))
#endif

#define FPS_ENABLE

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

#define Console_X				0
#define Console_y				20
#define Console_Width		240
#define Console_Height	115
#define Console_Radius	8
#define Console_TextSize	16
#define Console_MAXLINES	6
#define LOG_Info 0
#define LOG_Warn 1
#define LOG_Error 2
#define LOG_Empty 3
#define PI							3.14159

#ifdef FPS_ENABLE
static __IO uint32_t tick_prev = 0;
static __IO uint32_t tick_curr = 0;
static __IO uint16_t fps = 0;
static char fps_buf[10];
#endif

extern uint16_t POINT_COLOR;        //默认笔刷颜色
extern uint16_t BACK_COLOR;        //默认背景颜色
#define MAX_STR_LENGTH 30
// 定义日志级别枚举类型
typedef enum {
    LOG_INFO = 0,
    LOG_WARN,
    LOG_ERROR,
		LOG_EMPTY
} LogLevel;

// 定义日志缓存结构体数组
typedef struct {
    LogLevel loglevel;
    char message[MAX_STR_LENGTH];
} Log_Buf;


void LCD_DisplayOn(void);                                                                //Open display
void LCD_DisplayOff(void);                                                                //Close display
void LCD_Write_HalfWord(const uint16_t da);                                                //Write half a byte of data to LCD
void LCD_Write_Cmd(uint8_t cmd);  
void LCD_Write_Data(uint8_t data);  	
void LCD_Clear(uint16_t color);                                                            //Clean screen
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color);                //Filled monochrome
void LCD_Draw_Point(uint16_t x, uint16_t y);                                                        //Draw points
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color);                                        //Painting with color dots
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                                        //Draw line
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                                    //Draw rectangle
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);                                                //Circle drawing
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size);                                        //Display a character
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);                                    //Display a number
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);                            //Display number
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p);                    //display string
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);                    //display picture
void PutChinese_strings(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint8_t mode);
void LCD_DrawCircleQuarter(int centerX, int centerY, int radius, int startAngle, int endAngle);
void LCD_DrawRoundRect(int x, int y, int width, int height, int radius);
#endif


