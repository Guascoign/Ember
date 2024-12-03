

#include "FreeRTOS_main.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
/******************************************************************************************************/
/*FreeRTOS配置*/
#include "LCD/lcd.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_demos.h" 
#include "lv_demo_stress.h" 
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
#include "widgets_init.h"
#include "circle_buffer.h"
#include "IIC/iic.h"
#include "UART/uart_pack.h"
#include "uart_printf.h"
#include "EEPROM/at24cxx.h"



/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* LcdRefresh 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LcdRefresh_PRIO      2                   /* 任务优先级 */
#define LcdRefresh_STK_SIZE  1280                 /* 任务堆栈大小 */
TaskHandle_t            LcdRefresh_TaskHandle;  /* 任务句柄 */
void LcdRefresh(void *pvParameters);             /* 任务函数 */

/* Main 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Main_PRIO      3                   /* 任务优先级 */
#define Main_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Main_TaskHandle;  /* 任务句柄 */
void Main(void *pvParameters);             /* 任务函数 */

/* LED 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_PRIO      3                   /* 任务优先级 */
#define LED_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            LED_TaskHandle;  /* 任务句柄 */
void LED(void *pvParameters);             /* 任务函数 */
/******************************************************************************************************/

lv_ui guider_ui;
uint8_t LCD_text_flag = 0; //文本更新标志位
char LCD_text_buf[1280];
typedef struct
  {
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
  }Run_time;
  Run_time run_time = {0,0,0};
	

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_main(void)
{    
	
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
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )LcdRefresh,
                (const char*    )"LcdRefresh_Task",
                (uint16_t       )LcdRefresh_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LcdRefresh_PRIO,
                (TaskHandle_t*  )&LcdRefresh_TaskHandle);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )Main,
                (const char*    )"Main_Task",
                (uint16_t       )Main_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Main_PRIO,
                (TaskHandle_t*  )&Main_TaskHandle);
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
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void LcdRefresh(void *pvParameters)
{    
    while(1)
    {
       setup_ui(&guider_ui);
			events_init(&guider_ui);
	lv_task_handler();
  //lv_label_set_text(guider_ui.screen_2_label_3, "Initializing...\n123");
  for(;;)
  {
    if(LCD_text_flag)
        {
          lv_label_set_text(guider_ui.screen_2_label_3, LCD_text_buf);
          LCD_text_flag = 0;
        }
    lv_timer_handler();
    vTaskDelay(10);
  }
    }
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
			
   vTaskDelay(pdMS_TO_TICKS(2000));
  LCDPrint("=================\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  LCDPrint("System Start!\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  LCDPrint("LVGL V8.4.0\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  LCDPrint("FreeRTOS V202212.01\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  LCDPrint("CMSIS_OS V1.02\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  LCDPrint("=================\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(500));

  //串口初始化
  struct UART_Device *pUARTDev = GetUARTDevice("STM32_Bare_HAL_UART1_IT");//获取外设地址指针
  pUARTDev->Init(pUARTDev, 115200, 8, 'N', 1);
  LCDPrint("UART1_115200\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  //AT24C02初始化
  struct AT24CXX_Device *pAT24Dev = AT24CXX_GetDevice("EEPROM");//获取外设地址指针
  pAT24Dev->AT24CXX_Init(pAT24Dev);
  if(pAT24Dev->AT24CXX_Check(pAT24Dev) == 0)
  {
    LCDPrint("EEPROM OK\n", LCD_text_buf , sizeof(LCD_text_buf));
  }
  else
  {
    LCDPrint("EEPROM Error\n", LCD_text_buf , sizeof(LCD_text_buf));
  }
  vTaskDelay(pdMS_TO_TICKS(100));

	for(;;)
  {
        vTaskDelay(pdMS_TO_TICKS(50));
  }
	
	
    }
}

/**
 * @brief       Led
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void LED(void *pvParameters)
{
  /* USER CODE BEGIN Led */
  char LCD_time_buf[32]; // 用于存储格式化的字符串
  /* Infinite loop */
  for(;;)
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
  /* USER CODE END Led */
}
