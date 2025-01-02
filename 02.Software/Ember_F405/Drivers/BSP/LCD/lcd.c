/********************************************************************************
    * 文件名称 ：lcd.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#include "LCD/lcd.h"
#include "LCD/font.h"
//#include "LCD/HzLib.h"
uint16_t POINT_COLOR = WHITE;        //默认笔刷颜色
uint16_t BACK_COLOR = BLACK;        //默认背景颜色
#ifdef USE_DMA
#include <string.h>
uint16_t DMA_MIN_SIZE = 16;
/* If you're using DMA, then u need a "framebuffer" to store datas to be displayed.
 * If your MCU don't have enough RAM, please avoid using DMA(or set 5 to 1).
 * And if your MCU have enough RAM(even larger than full-frame size),
 * Then you can specify the framebuffer size to the full resolution below.
 */
 #define HOR_LEN 	10	//	Also mind the resolution of your screen!
uint16_t disp_buf[LCD_Width * HOR_LEN];
#endif


/**
 * @brief    LCD底层SPI发送数据函数
 *
 * @param   data    数据的起始地址
 * @param   size    发送数据大小
 *
 * @return  void
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
 */
void LCD_Write_buff(uint8_t *buff, uint16_t buff_size)
{
    LCD_CS(0);
    LCD_DC(1);
    while (buff_size > 0) {
		uint16_t chunk_size = buff_size > 65535 ? 65535 : buff_size;
		#ifdef USE_DMA
			if (DMA_MIN_SIZE <= buff_size)
			{
				HAL_SPI_Transmit_DMA(&LCD_SPI_PORT, buff, chunk_size);
				while (LCD_SPI_PORT.hdmatx->State != HAL_DMA_STATE_READY)
				{}
			}
			else
				HAL_SPI_Transmit(&LCD_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
		#else
			HAL_SPI_Transmit(&LCD_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
		#endif
		buff += chunk_size;
		buff_size -= chunk_size;
	}
    LCD_CS(1);
}

/**
 * @brief    写命令到LCD
 *
 * @param   cmd        需要发送的命令
 *
 * @return  void
 */
void LCD_Write_Cmd(uint8_t cmd)
{
	LCD_CS(0);
  LCD_DC(0);
  HAL_SPI_Transmit(&LCD_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);	
  LCD_CS(1);
}

/**
 * @brief    写数据到LCD
 *
 * @param   cmd        需要发送的数据
 *
 * @return  void
 */
void LCD_Write_Data(uint8_t data)
{
	LCD_CS(0);
    LCD_DC(1);
    HAL_SPI_Transmit(&LCD_SPI_PORT, &data, sizeof(data), HAL_MAX_DELAY);	
    LCD_CS(1);
}

/**
 * 打开LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * 关闭LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

/**
 * 以一种颜色清空LCD屏
 *
 * @param   color    清屏颜色
 *
 * @return  void
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = { 0 };
    data[0] = color >> 8;
    data[1] = color;
	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1); //偏移 40 53
    for (j = 0; j < LCD_Buf_Size / 2; j++) {
        lcd_buf[j * 2] = data[0];
        lcd_buf[j * 2 + 1] = data[1];
    }
    for (i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++) {
        LCD_Write_buff(lcd_buf, LCD_Buf_Size);
    }
}

/**
 * 用一个颜色填充整个区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end            终点坐标
 * @param   color               填充颜色
 *
 * @return  void
 */
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color)
{
  uint32_t size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2; // 计算总字节数
  // 设置目标区域
  LCD_Address_Set(x_start, y_start, x_end, y_end);
#ifdef USE_DMA
    LCD_Write_buff((uint8_t *)color, size);
#else
  // 非 DMA 模式的常规写入
  uint32_t size_remain = 0;
  uint32_t color_index = 0;
  if (size > LCD_Buf_Size) {
    size_remain = size - LCD_Buf_Size;
    size = LCD_Buf_Size;
  }
  while (1) {
    for (uint16_t i = 0; i < size / 2; i++) {
      lcd_buf[2 * i] = color[color_index] >> 8; // 高字节
      lcd_buf[2 * i + 1] = color[color_index] & 0xFF;   // 低字节
      color_index++;
    }
    LCD_Write_buff(lcd_buf, size);
    if (size_remain == 0) break;
    if (size_remain > LCD_Buf_Size) {
      size_remain -= LCD_Buf_Size;
    } else {
      size = size_remain;
      size_remain = 0;
    }
  }
#endif
}
/**
 * 设置数据写入LCD缓存区域
 *
 * @param   x1,y1    起点坐标
 * @param   x2,y2    终点坐标
 *
 * @return  void
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)//LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1)
{
	x1 = x1 + X_SHIFT;
	x2 = x2 + X_SHIFT;
	y1 = y1 + Y_SHIFT;
	y2 = y2 + Y_SHIFT;
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);
    LCD_Write_Cmd(0x2C);
}


