
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
/*Peripherals******************************************************************************************/
#include "BSP/Peripherals/Peripherals.h"
#include "BSP/LCD/lcd_consle.h"
/*LVGL*************************************************************************************************/
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
#include "lv_demo_benchmark.h"
/******************************************************************************************************/
lv_ui guider_ui;
char g_LCD_consle_buf[1280];   // 显示缓冲区
uint8_t g_LCD_text_flag = 0;

typedef struct Run_time
  {
    uint16_t Count;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
  }Run_time_TypeDef;
  Run_time_TypeDef run_time = {0,0,0};

 
void Boot_anim(void);
/******************************************************************************************************/
/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  64                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* Main 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Main_PRIO      9                   /* 任务优先级 */
#define Main_STK_SIZE  1000                 /* 任务堆栈大小 */
TaskHandle_t            Main_TaskHandle;  /* 任务句柄 */
void Main(void *pvParameters);             /* 任务函数 */

/* LcdRefresh 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LcdRefresh_PRIO      9                   /* 任务优先级 */
#define LcdRefresh_STK_SIZE  500                 /* 任务堆栈大小 */
TaskHandle_t            LcdRefresh_TaskHandle;  /* 任务句柄 */
void LcdRefresh(void *pvParameters);             /* 任务函数 */

/* LCD_RTC 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LCD_RTC_PRIO      4                   /* 任务优先级 */
#define LCD_RTC_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            LCD_RTC_TaskHandle;  /* 任务句柄 */
void LCD_RTC(void *pvParameters);             /* 任务函数 */

/* Soft_timer 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Soft_timer_PRIO     9                   /* 任务优先级 */
#define Soft_timer_STK_SIZE  64                 /* 任务堆栈大小 */
TaskHandle_t            Soft_timer_TaskHandle;  /* 任务句柄 */
void Soft_timer(void *pvParameters);             /* 任务函数 */
/******************************************************************************************************/
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_start(void)
{    

  xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
              (const char*    )"start_task",          /* 任务名称 */
              (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
              (void*          )NULL,                  /* 传入给任务函数的参数 */
              (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
              (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

  vTaskStartScheduler();
}

/**
 * @brief       start_task 开始线程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();           /* 进入临界区 */
  //Soft_timer_Init();						// 蜂鸣器初始化
  //PWM_WS2812B_Init(125);	// RGB初始化
  All_Peripherals_Init();    // 初始化所有外设


  /* 创建任务1 */
  xTaskCreate((TaskFunction_t )Main,
              (const char*    )"Main_Task",
              (uint16_t       )Main_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )Main_PRIO,
              (TaskHandle_t*  )&Main_TaskHandle);
 /* 创建任务2 */          
 xTaskCreate((TaskFunction_t )LcdRefresh,
             (const char*    )"LcdRefresh_Task",
             (uint16_t       )LcdRefresh_STK_SIZE,
             (void*          )NULL,
             (UBaseType_t    )LcdRefresh_PRIO,
             (TaskHandle_t*  )&LcdRefresh_TaskHandle);
  /* 创建任务3 */
  xTaskCreate((TaskFunction_t )LCD_RTC,
              (const char*    )"LCD_RTC_Task",
              (uint16_t       )LCD_RTC_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LCD_RTC_PRIO,
              (TaskHandle_t*  )&LCD_RTC_TaskHandle);
 /* 创建任务4 */
 xTaskCreate((TaskFunction_t )Soft_timer,
             (const char*    )"Soft_timer_Task",
             (uint16_t       )Soft_timer_STK_SIZE,
             (void*          )NULL,
             (UBaseType_t    )Soft_timer_PRIO,
             (TaskHandle_t*  )&Soft_timer_TaskHandle);


  vTaskDelete(StartTask_Handler); /* 删除开始任务 */
  taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       Main
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
extern LED_DeviceTypeDef RUNLED;
extern RGB_DeviceTypeDef WS2812B;
void Main(void *pvParameters)
{
  Boot_anim();
   vTaskDelay(pdMS_TO_TICKS(100));
	//lv_demo_benchmark();//启动benchmark例程
  uint8_t i = 0;
  RUNLED.Set(&RUNLED, LED_Blink_Three, continue_Blink);
  WS2812B.SetAllRGB(&WS2812B,0,0,255);
  while(1)
  {
  lcdprintf("Main Task Runing LCD Refresh %d\r\n",i++);
  vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/**
 * @brief       LCD_RTC
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void LCD_RTC(void *pvParameters)
{
  char LCD_time_buf[32]; // 用于存储格式化的字符串
  while(1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
    run_time.Second = run_time.Second +1;
    if(run_time.Second == 60)
    {
      run_time.Second = 0;
      run_time.Minute = run_time.Minute +1;
      if(run_time.Minute == 60)
      {
        run_time.Minute = 0;
        run_time.Hour = run_time.Hour +1;
      }
    }
    // 格式化字符串 xxH xxM xxS
    sprintf(LCD_time_buf, "%02dH%02dM%02dS", run_time.Hour, run_time.Minute, run_time.Second);
    lv_label_set_text(guider_ui.screen_2_label_2, LCD_time_buf);
  }
}

/**
 * @brief       LcdRefresh  屏幕刷新
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void LcdRefresh(void *pvParameters)
{    
  setup_ui(&guider_ui);
	events_init(&guider_ui);
    while(1)
    {
     if(g_LCD_text_flag) //命令台输出刷新标志位
       {
         lv_label_set_text(guider_ui.screen_2_label_3, g_LCD_consle_buf);
         g_LCD_text_flag = 0;
       }
      lv_timer_handler();//定时器处理
  	  lv_task_handler();//任务处理
      vTaskDelay(30);
    }
}

/**
 * @brief       Soft_timer
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
extern void Check_Soft_Timer(void *args);
void Soft_timer(void *pvParameters)
{
  
  while(1)
  {
    vTaskDelay(pdMS_TO_TICKS(1));//1ms进入一次
  /* ---------- Soft_timer ---------- */
   Check_LED_Soft_Timer( (void *)&RUNLED );//放入1ms定时器中断
  }

}

/**
 * @brief   Boot_anim 
 * @param       无
 * @retval      无
 * 
 */
void Boot_anim(void)
{
  vTaskDelay(pdMS_TO_TICKS(1000));
 lcdprintf("=================\r\n"); 
 lcdprintf("System Start!\r\n"); 
 vTaskDelay(pdMS_TO_TICKS(100));
 lcdprintf("LVGL V8.4.0\r\n");
 vTaskDelay(pdMS_TO_TICKS(100));
 lcdprintf("FreeRTOS V202212.01\r\n");
 vTaskDelay(pdMS_TO_TICKS(100));
 lcdprintf("=================\r\n"); 
 vTaskDelay(pdMS_TO_TICKS(500));
	lcdprintf("HELLO!\n");
 vTaskDelay(pdMS_TO_TICKS(200));
 //PWM_WS2812B_Red(3);
 //Soft_timer_Perform(BOOT);		// 蜂鸣器响声
 vTaskDelay(pdMS_TO_TICKS(150));
 //PWM_WS2812B_Blue(3);
 vTaskDelay(pdMS_TO_TICKS(150));
 //PWM_WS2812B_Green(3);
 vTaskDelay(pdMS_TO_TICKS(250));
 //PWM_WS2812B_Close(3);
}
