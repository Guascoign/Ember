/********************************************************************************
    * 文件名称 ：st7789.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-12
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#include "LCD/st7789.h"


/**
 * @brief    LCD初始化
 *
 * @param   void
 *
 * @return  void
 */
void ST7789V_Init(void)
{
    //LCD_Gpio_Init();    //硬件接口初始化

    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(12);
    LCD_RST(1);

    HAL_Delay(12);
    /* Sleep Out */
    LCD_Write_Cmd(0x11);
    /* wait for power stability */
    HAL_Delay(12);


//-----------------------ST7789V Frame rate setting-----------------//
//************************************************
                LCD_Write_Cmd(0x3A);        //65k mode
                LCD_Write_Data(0x05);
                LCD_Write_Cmd(0xC5); 		//VCOM
                LCD_Write_Data(0x1A);//1A
                LCD_Write_Cmd(0x36);         // 屏幕显示方向设置
								switch (ST7789_ROTATION) 
								{
									case 0:
											LCD_Write_Data(0x00);   // 上显1
											break;
									case 1:
											LCD_Write_Data(0xC0);   // 下显2
											break;
									case 2:
											LCD_Write_Data(0x70);   // 左显3
											break;
									case 3:
											LCD_Write_Data(0xA0);   // 右显4
											break;
									default:
											// 默认情况下，执行相应的操作（例如，上显1）
											LCD_Write_Data(0x00);
											break;
								}
                //-------------ST7789V Frame rate setting-----------//
                LCD_Write_Cmd(0xb2);		//Porch Setting
                LCD_Write_Data(0x05);
                LCD_Write_Data(0x05);
                LCD_Write_Data(0x00);
                LCD_Write_Data(0x33);
                LCD_Write_Data(0x33);

                LCD_Write_Cmd(0xb7);			//Gate Control
                LCD_Write_Data(0x05);			//12.2v   -10.43v
                //--------------ST7789V Power setting---------------//
                LCD_Write_Cmd(0xBB);//VCOM
                LCD_Write_Data(0x3F);//3f

                LCD_Write_Cmd(0xC0); //Power control
                LCD_Write_Data(0x2c);

                LCD_Write_Cmd(0xC2);		//VDV and VRH Command Enable
                LCD_Write_Data(0x01);

                LCD_Write_Cmd(0xC3);			//VRH Set
                LCD_Write_Data(0x0F);		//4.3+( vcom+vcom offset+vdv)

                LCD_Write_Cmd(0xC4);			//VDV Set
                LCD_Write_Data(0x20);				//0v

                LCD_Write_Cmd(0xC6);				//Frame Rate Control in Normal Mode
                LCD_Write_Data(0X01);			//111Hz

                LCD_Write_Cmd(0xd0);				//Power Control 1
                LCD_Write_Data(0xa4);
                LCD_Write_Data(0xa1);

                LCD_Write_Cmd(0xE8);				//Power Control 1
                LCD_Write_Data(0x03);

                LCD_Write_Cmd(0xE9);				//Equalize time control
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x08);
                //---------------ST7789V gamma setting-------------//
                LCD_Write_Cmd(0xE0); //Set Gamma
                LCD_Write_Data(0xD0);
                LCD_Write_Data(0x05);
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x08);
                LCD_Write_Data(0x14);
                LCD_Write_Data(0x28);
                LCD_Write_Data(0x33);
                LCD_Write_Data(0x3F);
                LCD_Write_Data(0x07);
                LCD_Write_Data(0x13);
                LCD_Write_Data(0x14);
                LCD_Write_Data(0x28);
                LCD_Write_Data(0x30);
                 
                LCD_Write_Cmd(0XE1); //Set Gamma
                LCD_Write_Data(0xD0);
                LCD_Write_Data(0x05);
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x09);
                LCD_Write_Data(0x08);
                LCD_Write_Data(0x03);
                LCD_Write_Data(0x24);
                LCD_Write_Data(0x32);
                LCD_Write_Data(0x32);
                LCD_Write_Data(0x3B);
                LCD_Write_Data(0x14);
                LCD_Write_Data(0x13);
                LCD_Write_Data(0x28);
                LCD_Write_Data(0x2F);

                LCD_Write_Cmd(0x21); 		//反显
               
                LCD_Write_Cmd(0x29);         //开启显示 

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    LCD_Clear(WHITE);

    /* Display on */
    LCD_PWR(1);
}


