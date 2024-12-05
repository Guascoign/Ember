
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>

#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"

#include "lv_100ask_lesson_demos/lv_100ask_lesson_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
//static const wchar_t * title = L"LVGL port Windows CodeBlocks.      https://lvgl.io | https://docs.lvgl.io";
static const wchar_t * title = L"百问网LVGL课程案例  © Copyright 2024, Shenzhen Baiwenwang Technology Co., Ltd.   https://www.100ask.net | https://lvgl.100ask.net";

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL for LVGL*/
    lv_display_t * display = lv_windows_create_display(title, 1024, 600, 100, FALSE, FALSE);
    lv_windows_acquire_pointer_indev(display);

    /*Output prompt information to the console, you can also use printf() to print directly*/
    LV_LOG_USER("LVGL initialization completed!");

    /*Run the demo*/
    // 下面这三行是运行一个demo，第一行和后面两行的效果是一样的，只能同时去掉第一行或者后面两行，两者之一的注释
    // 后续学习的时候要确保下面这三行都被注释掉
    lv_demo_widgets();
    //char * demo_str[] = {"widgets"};
    //lv_demos_create(demo_str, strlen((char * )demo_str));

    /* 课程示例代码，一般同时只运行一个
       运行前，请将上面的三行代码都注释掉 */
    //lesson_2_3_1();         // 基础对象(lv_obj)，"Hello, LVGL
    //lesson_2_4_1();         // 基础对象的大小(size)
    //lesson_2_5_1();         // 基础对象的位置(position)
    //lesson_2_6_1();         // 基础对象的盒子模型(border-box)
    //lesson_2_7_1();         // 基础对象的样式(styles)
    //lesson_2_8_1();         // 基础对象的事件(events)、事件冒泡
    //lesson_2_9_1();         // LVGL的定时器(lv_timer)-入门

    //lesson_3_1_1();         // 标签控件(lv_label)的用法
    //lesson_3_2_1();         // 按钮控件(lv_button)的用法
    //lesson_3_3_1();         // 进度条控件(lv_bar)的用法
    //lesson_3_4_1();         // 滑动条控件(lv_slider)的用法
    //lesson_3_5_1();         // 圆弧控件(lv_arc)的用法
    //lesson_3_6_1();         // 环形加载器控件(lv_spinner)的用法
    //lesson_3_7_1();         // 矩阵按钮控件(lv_btnmatrix)的用法
    //lesson_3_8_1();         // 列表控件(lv_list)的用法
    //lesson_3_9_1();         // 开关控件(lv_switch)的用法
    //lesson_3_10_1();        // 复选框控件(lv_checkbox)的用法
    //lesson_3_11_1();        // 线条控件(lv_line)的用法
    //lesson_3_12_1();        // LED控件(lv_led)的用法
    //lesson_3_13_1();        // 下拉列表控件(lv_dropdown)的用法
    //lesson_3_14_1();        // 滚轮控件(lv_roller)的用法
    //lesson_3_15_1();        // 消息框控件lv_msgbox的用法
    //lesson_3_16_1();        // 富文本控件(lv_span)的用法
    //lesson_3_17_1();        // 文本框控件(lv_textarea)的用法
    //lesson_3_18_1();        // 键盘控件(lv_keyboard)的用法
    //lesson_3_19_1();        // 微调框控件(lv_spinbox)的用法
    //lesson_3_20_1();        // 表格控件(lv_table)的用法
    //lesson_3_21_1();        // 标尺控件lv_scale的用法
    //lesson_3_22_1();        // 图表控件(lv_chart)的用法
    //lesson_3_23_1();        // 日历控件(lv_calendar)的用法
    //lesson_3_24_1();        // 图像控件(lv_image)的用法
    //lesson_3_25_1();        // 图像按钮控件(lv_imagebutton)的用法
    //lesson_3_26_1();        // 动画图像控件(lv_animimg)用法
    //lesson_3_27_1();        // 画布控件(lv_canvas)的用法
    //lesson_3_28_1();        // 选项卡控件(lv_tabview)的用法
    //lesson_3_29_1();        // 平铺视图控件(lv_tileview)的用法
    //lesson_3_30_1();        // 菜单控件(lv_menu)的用法
    //lesson_3_31_1();        // 窗口控件(lv_win)的用法
    //lesson_3_32_1();        // Lottie控件(lv_lottie)的用法

    //lesson_4_1();           // Flex（弹性）布局的用法
    //lesson_4_2();           // Grid（弹性）布局的用法
    //lesson_4_3();           // Flex（弹性）和Grid（网格）布局的区别

    //lesson_5_1();           // LVGL文件系统模块的用法
    //lesson_5_2();           // 通过LVGL文件系统模块的显示图片文件
    //lesson_5_3();           // FreeType控件（lv_freetype）的用法
    //lesson_5_4();           // 二维码控件(lv_qrcode)的用法
    //lesson_5_5();           // 条形码控件(lv_barcode)的用法

    //lesson_6_1();           // 文件资源管理器控件（lv_file_explorer）的用法
    //lesson_6_2();           // 拼音输入法控件（lv_ime_pinyin）的用法
    //lesson_6_3();           // 快照控件（lv_snapshot）的用法
    //lesson_6_4();           // Monkey测试（压力测试）的用法

    //lesson_7_1();           // 更进一步理解LVGL的定时器
    //lesson_7_2();           // 动画的用法
    //lesson_7_3();           // 动画时间线的用法
    //lesson_7_4();           // 动画的本质是定时器处理
    //lesson_7_5();           // 为什么创建出来的不同控件会不一样？
    //lesson_7_6();           // 物理按键代替触摸（鼠标模仿编码器）
    //lesson_7_7();           // Windows HOOK拦截键盘输入模拟按键输入

    //lesson_8_1();           // 基于FontAwesome 添加新SYMBOLS
    //lesson_8_2();           // 调试方法之LV_LOG
    //lesson_8_3();           // 调试方法之LV_SYSMON(system monitor)
    
    //lesson_9_1();           // 移植适配 lv_lib_100ask
    //lesson_9_2();           // 移植适配 lv_100ask_ui

    //lesson_10_1();           // LVGL的编码风格
    //lesson_10_2();           // 如何阅读Changelog
    //lesson_10_3();           // 如何阅读Roadmap
    //lesson_10_4();           // 如何参与LVGL开发-为LVGL库做贡献


    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(5000);       /*Just to let the system breath*/
    }
    return 0;
}

