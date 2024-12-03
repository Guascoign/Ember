

#include "FreeRTOS_main.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
/******************************************************************************************************/
/*FreeRTOS����*/
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



/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* LcdRefresh ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LcdRefresh_PRIO      2                   /* �������ȼ� */
#define LcdRefresh_STK_SIZE  1280                 /* �����ջ��С */
TaskHandle_t            LcdRefresh_TaskHandle;  /* ������ */
void LcdRefresh(void *pvParameters);             /* ������ */

/* Main ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define Main_PRIO      3                   /* �������ȼ� */
#define Main_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Main_TaskHandle;  /* ������ */
void Main(void *pvParameters);             /* ������ */

/* LED ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_PRIO      3                   /* �������ȼ� */
#define LED_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            LED_TaskHandle;  /* ������ */
void LED(void *pvParameters);             /* ������ */
/******************************************************************************************************/

lv_ui guider_ui;
uint8_t LCD_text_flag = 0; //�ı����±�־λ
char LCD_text_buf[1280];
typedef struct
  {
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
  }Run_time;
  Run_time run_time = {0,0,0};
	

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_main(void)
{    
	
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
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */
    /* ��������1 */
    xTaskCreate((TaskFunction_t )LcdRefresh,
                (const char*    )"LcdRefresh_Task",
                (uint16_t       )LcdRefresh_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LcdRefresh_PRIO,
                (TaskHandle_t*  )&LcdRefresh_TaskHandle);
    /* ��������2 */
    xTaskCreate((TaskFunction_t )Main,
                (const char*    )"Main_Task",
                (uint16_t       )Main_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Main_PRIO,
                (TaskHandle_t*  )&Main_TaskHandle);
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
 * @brief       task1
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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

  //���ڳ�ʼ��
  struct UART_Device *pUARTDev = GetUARTDevice("STM32_Bare_HAL_UART1_IT");//��ȡ�����ַָ��
  pUARTDev->Init(pUARTDev, 115200, 8, 'N', 1);
  LCDPrint("UART1_115200\n", LCD_text_buf , sizeof(LCD_text_buf));
  vTaskDelay(pdMS_TO_TICKS(100));
  //AT24C02��ʼ��
  struct AT24CXX_Device *pAT24Dev = AT24CXX_GetDevice("EEPROM");//��ȡ�����ַָ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void LED(void *pvParameters)
{
  /* USER CODE BEGIN Led */
  char LCD_time_buf[32]; // ���ڴ洢��ʽ�����ַ���
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
    // ��ʽ���ַ��� xxH xxM xxS
    sprintf(LCD_time_buf, "%02dH%02dM%02dS", run_time.Hour, run_time.Minute, run_time.Second);
    lv_label_set_text(guider_ui.screen_2_label_2, LCD_time_buf);
  }
  /* USER CODE END Led */
}
