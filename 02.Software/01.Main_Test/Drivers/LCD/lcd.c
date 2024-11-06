/********************************************************************************
    * 文件名称 ：lcd.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#include "lcd.h"
#include "font.h"
#include "HzLib.h"
uint16_t POINT_COLOR = WHITE;        //默认笔刷颜色
uint16_t BACK_COLOR = BLACK;        //默认背景颜色
/**
 * @brief    LCD底层SPI发送数据函数
 *
 * @param   data    数据的起始地址
 * @param   size    发送数据大小
 *
 * @return  void
 */

static void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
    LCD_CS(0);
    SPI2_WriteByte(data, size);
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
    LCD_DC(0);

    LCD_SPI_Send(&cmd, 1);
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
    LCD_DC(1);

    LCD_SPI_Send(&data, 1);
}

/**
 * @brief    写半个字的数据到LCD
 *
 * @param   cmd        需要发送的数据
 *
 * @return  void
 */
void LCD_Write_HalfWord(const uint16_t da)
{
    uint8_t data[2] = { 0 };

    data[0] = da >> 8;
    data[1] = da;

    LCD_DC(1);
    LCD_SPI_Send(data, 2);
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

    //LCD_Address_Set(40, 53, LCD_Width - 1 + 40, LCD_Height - 1 + 53); //偏移 40 53
		LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1); //偏移 40 53

    for (j = 0; j < LCD_Buf_Size / 2; j++) {
        lcd_buf[j * 2] = data[0];
        lcd_buf[j * 2 + 1] = data[1];
    }

    LCD_DC(1);

    for (i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++) {
        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
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
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if (size > LCD_Buf_Size) {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while (1) {
        for (i = 0; i < size / 2; i++) {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, size);

        if (size_remain == 0)
            break;

        if (size_remain > LCD_Buf_Size) {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else {
            size = size_remain;
            size_remain = 0;
        }
    }
}

/**
 * 画点函数
 *
 * @param   x,y        画点坐标
 *
 * @return  void
 */
void LCD_Draw_Point(uint16_t x, uint16_t y)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(POINT_COLOR);
}

/**
 * 画点带颜色函数
 *
 * @param   x,y        画点坐标
 *
 * @return  void
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(color);
}



/**
 * @brief    画线函数(直线、斜线)
 *
 * @param   x1,y1    起点坐标
 * @param   x2,y2    终点坐标
 *
 * @return  void
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if (y1 == y2) {
        /*快速画水平线*/
        LCD_Address_Set(x1, y1, x2, y2);

        for (i = 0; i < x2 - x1; i++) {
            lcd_buf[2 * i] = POINT_COLOR >> 8;
            lcd_buf[2 * i + 1] = POINT_COLOR;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;

    else if (delta_x == 0)incx = 0;

    else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;

    else if (delta_y == 0)incy = 0;

    else {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++) {
        LCD_Draw_Point(row, col);
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance) {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance) {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief    画一个矩形
 *
 * @param   x1,y1    起点坐标
 * @param   x2,y2    终点坐标
 *
 * @return  void
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/**
 * @brief    画一个圆
 *
 * @param   x0,y0    圆心坐标
 * @param   r       圆半径
 *
 * @return  void
 */
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while (a <= b) {
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 + b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 - b);
        LCD_Draw_Point(x0 + b, y0 + a);
        LCD_Draw_Point(x0 + a, y0 - b);
        LCD_Draw_Point(x0 + a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 + a);
        a++;

        //Bresenham
        if (di < 0)di += 4 * a + 6;

        else {
            di += 10 + 4 * (a - b);
            b--;
        }

        LCD_Draw_Point(x0 + a, y0 + b);
    }
}

/**
 * @brief    显示一个ASCII码字符
 *
 * @param   x,y        显示起始坐标
 * @param   chr        需要显示的字符
 * @param   size    字体大小(支持16/24/32号字体)
 *
 * @return  void
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size)
{
    uint8_t temp, t1, t;
    uint8_t csize;        //得到字体一个字符对应点阵集所占的字节数
    uint16_t colortemp;
    uint8_t sta;

    chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    if ((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))    return;

    LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if ((size == 16) || (size == 32))    //16和32号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for (t = 0; t < csize; t++) {
            if (size == 16)temp = asc2_1608[chr][t];    //调用1608字体

            else if (size == 32)temp = asc2_3216[chr][t];    //调用3216字体

            else return;            //没有的字库

            for (t1 = 0; t1 < 8; t1++) {
                if (temp & 0x80) colortemp = POINT_COLOR;

                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if (size == 12)    //12号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for (t = 0; t < csize; t++) {
            temp = asc2_1206[chr][t];

            for (t1 = 0; t1 < 6; t1++) {
                if (temp & 0x80) colortemp = POINT_COLOR;

                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if (size == 24)        //24号字体
    {
        csize = (size * 16) / 8;

        for (t = 0; t < csize; t++) {
            temp = asc2_2412[chr][t];

            if (t % 2 == 0)sta = 8;

            else sta = 4;

            for (t1 = 0; t1 < sta; t1++) {
                if (temp & 0x80) colortemp = POINT_COLOR;

                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief    m^n函数
 *
 * @param   m,n        输入参数
 *
 * @return  m^n次方
 */
static uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

/**
 * @brief    显示数字,高位为0不显示
 *
 * @param   x,y        起点坐标
 * @param   num        需要显示的数字,数字范围(0~4294967295)
 * @param   len        需要显示的位数
 * @param   size    字体大小
 *
 * @return  void
 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++) {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + (size / 2) * t, y, ' ', size);
                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size);
    }
}



/**
 * @brief    显示数字,高位为0,可以控制显示为0还是不显示
 *
 * @param   x,y        起点坐标
 * @param   num        需要显示的数字,数字范围(0~999999999)
 * @param   len        需要显示的位数
 * @param   size    字体大小
 * @param   mode    1:高位显示0        0:高位不显示
 *
 * @return  void
 */
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++) {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                if (mode)LCD_ShowChar(x + (size / 2) * t, y, '0', size);

                else
                    LCD_ShowChar(x + (size / 2) * t, y, ' ', size);

                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size);
    }
}


/**
 * @brief    显示字符串
 *
 * @param   x,y        起点坐标
 * @param   width    字符显示区域宽度
 * @param   height    字符显示区域高度
 * @param   size    字体大小
 * @param   p        字符串起始地址
 *
 * @return  void
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x >= width) {
            x = x0;
            y += size;
        }

        if (y >= height)break; //退出

        LCD_ShowChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
}


/**
 * @brief    显示图片
 *
 * @remark    Image2Lcd取模方式：    C语言数据/水平扫描/16位真彩色(RGB565)/高位在前        其他的不要选
 *
 * @param   x,y        起点坐标
 * @param   width    图片宽度
 * @param   height    图片高度
 * @param   p        图片缓存数据起始地址
 *
 * @return  void
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    if (x + width > LCD_Width || y + height > LCD_Height) {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC(1);

    if (width * height * 2 > 65535) {
        LCD_SPI_Send((uint8_t *)p, 65535);
        LCD_SPI_Send((uint8_t *)(p + 65535), width * height * 2 - 65535);
    } else {
        LCD_SPI_Send((uint8_t *)p, width * height * 2);
    }
}



/**
 * @brief   显示单个汉字字符串
 *
 * @param   Xpos    横坐标，范围：0~(319-16)
 * @param   Ypos    纵坐标，范围：0~(239-16)
 * @param   str     中文字符串首址
 * @param   Color   字符色   
 * @param   mode    显示模式，0 表示文字背景色为白色，1 表示文字悬浮
 *
 * @return  void
 *
 * @example PutChinese21(200, 100, "好", 0, 0);
 *
 * @note    如果输入大于1的汉字字符串，显示将会截断，只显示最前面一个汉字
 */
void PutChinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint8_t mode)
{
    uint8_t i, j;
    uint8_t buffer[32];
    uint16_t tmp_char = 0;
#ifdef SONG_TYPEFACE                                                                    
    GetGBKCode(buffer, str); /* 取字模数据 */
#endif
    for (i = 0;i < 16;i++) {
        tmp_char = buffer[i * 2];
        tmp_char = (tmp_char << 8);
        tmp_char |= buffer[2 * i + 1];
        for (j = 0;j < 16;j++) {

            if ((( (tmp_char >> 15) - j ) & 0x01) == 0x01) 
							{
                LCD_Draw_Point(Ypos + j, Xpos + i);
            } else {
                if (mode == 0)
                    LCD_Draw_ColorPoint(Ypos + j, Xpos + i, BACK_COLOR);//指定字体背景颜色
                else if (mode == 1) {
                    //不写入
                }


            }
        }
    }
}

/**
 * @brief   显示汉字字符串
 *
 * @param   Xpos    横坐标，范围：0~(319-16)
 * @param   Ypos    纵坐标，范围：0~(239-16)
 * @param   str     中文字符串首址
 * @param   Color   字符色   
 * @param   mode    显示模式，0 表示文字背景色为白色，1 表示文字悬浮
 *
 * @return  void
 *
 * @example PutChinese_strings21(200, 100, "好人", 0, 0);
 *
 * @note    无
 */
void PutChinese_strings(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint8_t mode)
{

    uint16_t Tmp_x, Tmp_y;
    uint8_t *tmp_str = str;
    Tmp_x = Ypos;
    Tmp_y = Xpos;

    while (*tmp_str != '\0') {
        PutChinese(Tmp_x, Tmp_y, tmp_str, mode);

        tmp_str += 2;
        Tmp_y += 16;
    }
}


uint8_t logLevel_buf[MAX_STR_LENGTH]={0};
// 初始化日志缓存数组
Log_Buf consoleLog[Console_MAXLINES];
uint8_t Log_Head = 0;
uint8_t Print_Head;
#define LOG_LEVEL_STR_LEN 6	//[INFO] 6字节

/**
 * @brief   获取日志级别对应的字符串表示
 *
 * @param   logLevel    日志级别
 *
 * @return  char*       日志级别字符串表示
 */
char *getLogLevelStr(LogLevel logLevel) {
    switch(logLevel) {
        case LOG_INFO:
            return "[INFO]";
        case LOG_WARN:
            return "[WARN]";
        case LOG_ERROR:
            return "[ ERR]";
        case LOG_EMPTY:
            return "";
        default:
            return "";
    }
}

/**
 * @brief   将字符串写入 LCD 控制台
 *
 * @param   logLevel    日志级别
 * @param   str         待写入的字符串
 *
 * @return  void
 */
void LCD_Console_Write(LogLevel logLevel, char *str) {
    // 获取输入字符串的长度
    size_t len = strlen(str);

    // 确保长度不超过 MAX_STR_LENGTH - LOG_LEVEL_STR_LEN，如果超过，则截断
    if (len > MAX_STR_LENGTH ) {
        len = MAX_STR_LENGTH ;
    }

    // 获取日志级别字符串
    char *levelStr = getLogLevelStr(logLevel);
    
    // 复制字符串到缓冲区，并在前面添加日志级别标识
    snprintf(consoleLog[Log_Head].message, MAX_STR_LENGTH + 1, "%s%.*s", levelStr, (int)len, str);

    // 如果字符串长度小于 MAX_STR_LENGTH - LOG_LEVEL_STR_LEN，补全空格
    for (size_t i = len + strlen(levelStr); i < MAX_STR_LENGTH; i++) {
        consoleLog[Log_Head].message[i] = ' ';
    }

    // 设置日志级别
    consoleLog[Log_Head].loglevel = logLevel;

    // 更新 Log_Head
    Log_Head = (Log_Head + 1) % Console_MAXLINES;
}

/**
 * @brief   打印 LCD 控制台缓存中的所有数据
 *
 * @return  void
 */
void LCD_Console_Print() {
    // 从尾部开始打印缓存中的所有数据
    int Print_Head = (Log_Head - 1 + Console_MAXLINES) % Console_MAXLINES;
    for (int i = 0; i < Console_MAXLINES; i++) {
				        switch(consoleLog[Print_Head].loglevel) {
            case LOG_INFO:
                POINT_COLOR = GREEN; // 白色
                break;
            case LOG_WARN:
                POINT_COLOR = YELLOW; // 黄色
                break;
            case LOG_ERROR:
                POINT_COLOR = RED; // 红色
                break;
            case LOG_EMPTY:
                POINT_COLOR = WHITE; // 白色
                break;
            default:
                POINT_COLOR = WHITE; // 白色
                break;
        }
        // 在屏幕上打印消息
        LCD_ShowString(Console_X + 2, Console_y + (Console_MAXLINES - i) * Console_TextSize - 10, Console_Width, Console_Height, Console_TextSize, consoleLog[Print_Head].message);
        
        // 更新要打印的数据的索引，从尾部向前
        Print_Head = (Print_Head - 1 + Console_MAXLINES) % Console_MAXLINES;
    }
}


/**
 * @brief    画一个圆角矩形
 *
 * @param   x1,y1    起点坐标
 * @param   x2,y2    终点坐标
 *
 * @return  void
 */
void LCD_DrawRoundRect(int x, int y, int width, int height, int radius)
{
    // 确保半径不大于矩形的宽度或高度的一半
    if (radius >= width / 2 || radius >= height / 2) {
        // 半径太大，无法形成圆角矩形
        return;
    }

    // 画上边线
    LCD_DrawLine(x + radius, y, x + width - radius, y);

    // 画右边线
    LCD_DrawLine(x + width, y + radius, x + width, y + height - radius);

    // 画下边线
    LCD_DrawLine(x + radius, y + height, x + width - radius, y + height);

    // 画左边线
    LCD_DrawLine(x, y + radius, x, y + height - radius);

    // 画四个角的圆弧
    LCD_DrawCircleQuarter(x + radius, y + radius, radius, 180, 270);  // 左上角
    LCD_DrawCircleQuarter(x + width - radius, y + radius, radius, 270, 360);  // 右上角
    LCD_DrawCircleQuarter(x + radius, y + height - radius, radius, 90, 180);  // 左下角
    LCD_DrawCircleQuarter(x + width - radius, y + height - radius, radius, 0, 90);  // 右下角
}


/**
 * @brief   绘制圆弧的一个象限
 *
 * @param   centerX     圆心的横坐标
 * @param   centerY     圆心的纵坐标
 * @param   radius      圆的半径
 * @param   startAngle  起始角度（度）
 * @param   endAngle    终止角度（度）
 *
 * @return  void
 */
void LCD_DrawCircleQuarter(int centerX, int centerY, int radius, int startAngle, int endAngle)
{
    int x, y;

    for (int angle = startAngle; angle <= endAngle; angle++) {
        // 将极坐标转换为直角坐标
        x = centerX + radius * cos(angle * PI / 180);
        y = centerY + radius * sin(angle * PI / 180);

        LCD_Draw_Point(x, y);
    }
}

/**
 * @brief   在LCD上显示帧率
 *
 * @param   x       显示位置的横坐标
 * @param   y       显示位置的纵坐标
 * @param   Type    显示类型，1 表示显示帧率及其标签，2 表示仅显示帧率
 *
 * @return  void
 */
void LCD_DisPlayFrames(int x, int y, int Type) {
    POINT_COLOR = WHITE;
    tick_curr = HAL_GetTick();
    if (tick_curr > tick_prev)
        fps = 1000u / (tick_curr - tick_prev);
    else if (tick_curr < tick_prev)
        fps = 1000u / (UINT32_MAX - tick_prev + tick_curr);
    else
        fps = 1000u;
    tick_prev = tick_curr;

    char *fps_buf = NULL;
    if (Type == 1) {
        fps_buf = LCD_Printf("FPS:%d  ", fps);
				    // 在此处使用 fps_buf
    if (fps_buf != NULL) {
        LCD_ShowString(x, y, 200, 100, 16, fps_buf);
        free(fps_buf); // 使用完后释放内存
    } else {
       printf("LCD_DisPlayFrames RAM NULL\n"); // 处理内存分配失败的情况
    }
    } else if (Type == 2) {
        fps_buf = LCD_Printf("%d  ", fps);
			    // 在此处使用 fps_buf
    if (fps_buf != NULL) {
        LCD_ShowString(x+20, y, 200, 100, 16, fps_buf);
        free(fps_buf); // 使用完后释放内存
    } else {
       printf("LCD_DisPlayFrames RAM NULL\n"); // 处理内存分配失败的情况
    }
    } else {
        fps_buf = LCD_Printf("Invalid Type");
    }
}

/**
 * @brief   LCD引导动画
 *
 * @return  void
 */
void LCD_BootAnimation(void)
{
	LCD_Console_Write(LOG_EMPTY, "   ______  ______  _______ ");
	LCD_Console_Print(); 
	LCD_Console_Write(LOG_EMPTY, "  / __/  |/  / _ )/ __/ _ \\");
	LCD_Console_Print(); 
	LCD_Console_Write(LOG_EMPTY, " / _// /|_/ / _  / _// , _/");
	LCD_Console_Print(); 
	LCD_Console_Write(LOG_EMPTY, "/___/_/  /_/____/___/_/|_| ");
	LCD_Console_Print(); 
	LCD_Console_Write(LOG_EMPTY, "Version: 1.0");
	LCD_Console_Print(); 
	LCD_Console_Write(LOG_EMPTY, "System Start!");
	LCD_Console_Print(); 
}

/**
 * @brief   在LCD上打印系统运行时间
 *
 * @return  void
 */
void LCD_PrintRunTime(void) {
    char *TimeStr = NULL;
    if (runtime.days > 0) {
        TimeStr = LCD_Printf("%02dD %02d:%02d:%02d", runtime.days, runtime.hours, runtime.minutes, runtime.seconds);
    } else {
        TimeStr = LCD_Printf("%02d:%02d:%02d", runtime.hours, runtime.minutes, runtime.seconds);
    }
    
    if (TimeStr != NULL) {
					POINT_COLOR = WHITE;
				 LCD_ShowString(0, 0, 200, 100, 16, TimeStr);
        //LCD_ConsolePrintf(LOG_INFO, TimeStr);
        free(TimeStr); // 使用完后释放内存
    } else {
        // 处理内存分配失败的情况
        printf("LCD_PrintRunTime RAM NULL\n");
    }
}

/**
 * @brief   格式化字符串输出函数
 *
 * @param   format  格式化字符串
 * @param   ...     可变参数列表
 *
 * @return  char*   格式化后的字符串，使用完需要手动释放内存
 */
char* LCD_Printf(const char *format, ...) {
    char *buffer = (char *)malloc(MAX_STR_LENGTH * sizeof(char)); // 动态分配内存
    if (buffer == NULL) 
		{
			printf("LCD_Printf RAM NULL\n");	// 内存分配失败，返回空指针
        return NULL;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, MAX_STR_LENGTH, format, args);
    va_end(args);
    return buffer;
}

/**
 * @brief   格式化输出到LCD控制台
 *
 * @param   logLevel    日志级别
 * @param   format      格式化字符串
 * @param   ...         可变参数列表
 *
 * @return  void
 */
void LCD_ConsolePrintf(LogLevel logLevel, const char *format, ...) {
    char buffer[MAX_STR_LENGTH]; // 缓冲区大小可以根据需要调整
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    LCD_Console_Write(logLevel, buffer);
}
