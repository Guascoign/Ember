/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD/lcd.h"
#include "RGB/WS2812B.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_demos.h" 
#include "lv_demo_stress.h" 
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
#include "widgets_init.h"//LVGL时钟组件
#include "gui_guider.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LCDHandle;
osThreadId LEDHandle;
osThreadId EEPROMHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
lv_ui guider_ui;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Start_LCD_Task(void const * argument);
void Start_LED_Task(void const * argument);
void Start_EEPROM_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  lv_init();											//lvgl系统初始化
	lv_port_disp_init();						//lvgl显示接口初始化,放在lv_init()的后面
	//LOG_MODULE_REGISTER(app);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LCD */
  osThreadDef(LCD, Start_LCD_Task, osPriorityAboveNormal, 0, 1024);
  LCDHandle = osThreadCreate(osThread(LCD), NULL);

  /* definition and creation of LED */
  osThreadDef(LED, Start_LED_Task, osPriorityIdle, 0, 128);
  LEDHandle = osThreadCreate(osThread(LED), NULL);

  /* definition and creation of EEPROM */
  osThreadDef(EEPROM, Start_EEPROM_Task, osPriorityIdle, 0, 128);
  EEPROMHandle = osThreadCreate(osThread(EEPROM), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Start_LCD_Task */
/**
* @brief Function implementing the LCD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LCD_Task */
void Start_LCD_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LCD_Task */
  /* Infinite loop */
  for(;;)
  {
    //lv_demo_stress();
		
//    lv_obj_t *switch1 = lv_switch_create(lv_scr_act());
//    lv_obj_set_size(switch1,200,100);
//    lv_obj_t *switch2 = lv_switch_create(switch1);
		
		setup_ui(&guider_ui);
   	events_init(&guider_ui);

	lv_task_handler();
	//display_blanking_off(display_dev);
		
		char buffer[128];
    int value = 42;
    char text_buffer[128];
    snprintf(buffer, sizeof(buffer), "Hello, World!\nValue: %d", value);
    while (1)
    {
      lv_timer_handler();
      vTaskDelay(5);
      snprintf(text_buffer, 128, "Updated value: %d", value);
value = value +1;
      lv_label_set_text(guider_ui.screen_label_2, text_buffer);
    }
    
  }
  /* USER CODE END Start_LCD_Task */
}

/* USER CODE BEGIN Header_Start_LED_Task */
/**
* @brief Function implementing the LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LED_Task */
void Start_LED_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LED_Task */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(RUN_LED_GPIO_Port,RUN_LED_Pin);
		HAL_Delay(100);
    osDelay(1);
  }
  /* USER CODE END Start_LED_Task */
}

/* USER CODE BEGIN Header_Start_EEPROM_Task */
/**
* @brief Function implementing the EEPROM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_EEPROM_Task */
void Start_EEPROM_Task(void const * argument)
{
  /* USER CODE BEGIN Start_EEPROM_Task */
  /* Infinite loop */
  for(;;)
  {

    osDelay(1);
  }
  /* USER CODE END Start_EEPROM_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
