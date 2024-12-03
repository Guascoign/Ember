
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
char g_LCD_consle_buf[1280];   // ��ʾ������
uint8_t g_LCD_text_flag = 0;

typedef struct Run_time
  {
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
  }Run_time_TypeDef;
  Run_time_TypeDef run_time = {0,0,0};

/******************************************************************************************************/
/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* Main ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define Main_PRIO      3                   /* �������ȼ� */
#define Main_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Main_TaskHandle;  /* ������ */
void Main(void *pvParameters);             /* ������ */

/* LcdRefresh ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LcdRefresh_PRIO      3                   /* �������ȼ� */
#define LcdRefresh_STK_SIZE  1280                 /* �����ջ��С */
TaskHandle_t            LcdRefresh_TaskHandle;  /* ������ */
void LcdRefresh(void *pvParameters);             /* ������ */

/* LED ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_PRIO      3                   /* �������ȼ� */
#define LED_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            LED_TaskHandle;  /* ������ */
void LED(void *pvParameters);             /* ������ */
/******************************************************************************************************/
/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_start(void)
{    
  /*LVGL �����ʼ��*/
	lv_init();											//lvglϵͳ��ʼ��
	lv_port_disp_init();						//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���

  xTaskCreate((TaskFunction_t )start_task,            /* ������ */
              (const char*    )"start_task",          /* �������� */
              (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
              (void*          )NULL,                  /* ������������Ĳ��� */
              (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
              (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

  vTaskStartScheduler();
}

/**
 * @brief       start_task ��ʼ�߳�
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();           /* �����ٽ��� */
  /* ��������1 */
  xTaskCreate((TaskFunction_t )Main,
              (const char*    )"Main_Task",
              (uint16_t       )Main_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )Main_PRIO,
              (TaskHandle_t*  )&Main_TaskHandle);
  /* ��������2 */          
  xTaskCreate((TaskFunction_t )LcdRefresh,
              (const char*    )"LcdRefresh_Task",
              (uint16_t       )LcdRefresh_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LcdRefresh_PRIO,
              (TaskHandle_t*  )&LcdRefresh_TaskHandle);
  /* ��������3 */
  xTaskCreate((TaskFunction_t )LED,
              (const char*    )"Led_Task",
              (uint16_t       )LED_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LED_PRIO,
              (TaskHandle_t*  )&LED_TaskHandle);

  vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
  taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

/**
 * @brief       Main
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @brief       LcdRefresh  ��Ļˢ��
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void LcdRefresh(void *pvParameters)
{    
  setup_ui(&guider_ui);
	events_init(&guider_ui);
    while(1)
    {
	    lv_task_handler();
      if(g_LCD_text_flag) //����̨���ˢ�±�־λ
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void LED(void *pvParameters)
{
  char LCD_time_buf[32]; // ���ڴ洢��ʽ�����ַ���
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
    // ��ʽ���ַ��� xxH xxM xxS
    sprintf(LCD_time_buf, "%02dH%02dM%02dS", run_time.Hour, run_time.Minute, run_time.Second);
    lv_label_set_text(guider_ui.screen_2_label_2, LCD_time_buf);
  }
}
