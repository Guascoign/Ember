
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
/******************************************************************************************************/
#include "LCD/lcd.h"
#include "LCD/lcd_consle.h"
#include "widgets_init.h"
#include "circle_buffer.h"
#include "IIC/iic.h"
#include "EEPROM/at24cxx.h"
#include "UART/uart_pack.h"
#include "uart_printf.h"
/*LVGL*********************************************************************************************/
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
/******************************************************************************************************/
lv_ui guider_ui;
char g_LCD_consle_buf[1280];   // 显示缓冲区
uint8_t g_LCD_text_flag = 0;

typedef struct Run_time
  {
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
  }Run_time_TypeDef;
  Run_time_TypeDef run_time = {0,0,0};

/******************************************************************************************************/
/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* Main 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Main_PRIO      3                   /* 任务优先级 */
#define Main_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Main_TaskHandle;  /* 任务句柄 */
void Main(void *pvParameters);             /* 任务函数 */

/* LcdRefresh 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LcdRefresh_PRIO      3                   /* 任务优先级 */
#define LcdRefresh_STK_SIZE  1280                 /* 任务堆栈大小 */
TaskHandle_t            LcdRefresh_TaskHandle;  /* 任务句柄 */
void LcdRefresh(void *pvParameters);             /* 任务函数 */

/* LED 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_PRIO      3                   /* 任务优先级 */
#define LED_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            LED_TaskHandle;  /* 任务句柄 */
void LED(void *pvParameters);             /* 任务函数 */
/******************************************************************************************************/
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_start(void)
{    
  /*LVGL 界面初始化*/
	lv_init();											//lvgl系统初始化
	lv_port_disp_init();						//lvgl显示接口初始化,放在lv_init()的后面

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
  xTaskCreate((TaskFunction_t )LED,
              (const char*    )"Led_Task",
              (uint16_t       )LED_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LED_PRIO,
              (TaskHandle_t*  )&LED_TaskHandle);

  vTaskDelete(StartTask_Handler); /* 删除开始任务 */
  taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       Main
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void Main(void *pvParameters)
{
  
  while(1)
  {
    lcdprintf("hello");
    vTaskDelay(pdMS_TO_TICKS(100));
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
	    lv_task_handler();
      if(g_LCD_text_flag) //命令台输出刷新标志位
        {
          lv_label_set_text(guider_ui.screen_2_label_3, g_LCD_consle_buf);
          g_LCD_text_flag = 0;
        }
      lv_timer_handler();
      vTaskDelay(10);
  
    }
}

/**
 * @brief       Led
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void LED(void *pvParameters)
{
  char LCD_time_buf[32]; // 用于存储格式化的字符串
  while(1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
    HAL_GPIO_TogglePin(RUNLED_GPIO_Port , RUNLED_Pin);
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
