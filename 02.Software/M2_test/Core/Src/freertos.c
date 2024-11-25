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
#include "RGB/ws2812b.h"
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
#include "UART/uart_printf.h"
#include "AT24CXX/at24cxx.h"
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
osThreadId Main_TaskHandle;
osThreadId LcdRefresh_TaskHandle;
osThreadId Led_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

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



#define WS2812_NUM_LEDS  8  // WS2812 灯带的 LED 数量
#define WS2812_LED_DATA_SIZE (WS2812_NUM_LEDS * 24)  // 每个 LED 24 位

// WS2812 数据结构（RGB 颜色）
typedef struct {
    uint8_t G;
    uint8_t R;
    uint8_t B;
} WS2812_Color_t;

// 存储 WS2812 的颜色数据
WS2812_Color_t WS2812_Data[WS2812_NUM_LEDS];

// 定义用于存储 WS2812 数据的 PWM 比较值
uint16_t WS2812_PWM_Buffer[WS2812_LED_DATA_SIZE];

void WS2812_TransmitData(void) {
    int i, j;
    int bitIndex = 0;

    // 将数据转换为适合 PWM 输出的形式
    for (i = 0; i < WS2812_NUM_LEDS; i++) {
        uint8_t byte = 0;
        
        // 每个 LED 有 24 位（RGB），按顺序转换
        for (j = 0; j < 8; j++) {
            // 获取每个颜色的高位/低位值
            byte = WS2812_Data[i].R;
            // 通过 PWM 控制生成一个 0 或 1 的信号
            WS2812_PWM_Buffer[bitIndex++] = (byte & (1 << (7 - j))) ? 70 : 35;
            byte = WS2812_Data[i].G;
            WS2812_PWM_Buffer[bitIndex++] = (byte & (1 << (7 - j))) ? 70 : 35;
            byte = WS2812_Data[i].B;
            WS2812_PWM_Buffer[bitIndex++] = (byte & (1 << (7 - j))) ? 70 : 35;
        }
    }

    // 这里假设你已配置好 DMA 来更新 PWM 数据
    // 使用 DMA 或定时器比较寄存器（CCR）来驱动 WS2812 数据线
    HAL_TIM_PWM_Start_DMA(&htim5, TIM_CHANNEL_2, (uint32_t *)WS2812_PWM_Buffer, WS2812_LED_DATA_SIZE);
}


/* USER CODE END FunctionPrototypes */

void Main(void const * argument);
void LcdRefresh(void const * argument);
void Led(void const * argument);

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
  /* definition and creation of Main_Task */
  osThreadDef(Main_Task, Main, osPriorityAboveNormal, 0, 640);
  Main_TaskHandle = osThreadCreate(osThread(Main_Task), NULL);

  /* definition and creation of LcdRefresh_Task */
  osThreadDef(LcdRefresh_Task, LcdRefresh, osPriorityIdle, 0, 1280);
  LcdRefresh_TaskHandle = osThreadCreate(osThread(LcdRefresh_Task), NULL);

  /* definition and creation of Led_Task */
  osThreadDef(Led_Task, Led, osPriorityIdle, 0, 128);
  Led_TaskHandle = osThreadCreate(osThread(Led_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Main */
/**
  * @brief  Function implementing the Main_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Main */
void Main(void const * argument)
{
  /* USER CODE BEGIN Main */
  /* Infinite loop */
  osDelay(2000);
  LCDPrint("=================\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
  LCDPrint("System Start!\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
  LCDPrint("LVGL V8.4.0\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
  LCDPrint("FreeRTOS V10.3.1\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
  LCDPrint("CMSIS_OS V1.02\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
  LCDPrint("=================\r\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(500);

  //串口初始化
  struct UART_Device *pUARTDev = GetUARTDevice("STM32_Bare_HAL_UART1_IT");//获取外设地址指针
  pUARTDev->Init(pUARTDev, 115200, 8, 'N', 1);
  LCDPrint("UART1_115200\n", LCD_text_buf , sizeof(LCD_text_buf));
  osDelay(100);
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
  osDelay(100);

 int i;
    for (i = 0; i < WS2812_NUM_LEDS; i++) {
        WS2812_Data[i].R = 255;
        WS2812_Data[i].G = 0;
        WS2812_Data[i].B = 0;
    }



  for(;;)
  {
  WS2812_TransmitData();  // 将数据发送到 WS2812
        LCDPrint("RGB DMA\n", LCD_text_buf , sizeof(LCD_text_buf));
        // 暂停一段时间以控制更新频率（比如 50ms 更新一次）
        vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END Main */
}

/* USER CODE BEGIN Header_LcdRefresh */
/**
* @brief Function implementing the LcdRefresh_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LcdRefresh */
void LcdRefresh(void const * argument)
{
  /* USER CODE BEGIN LcdRefresh */
  /* Infinite loop */
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
  /* USER CODE END LcdRefresh */
}

/* USER CODE BEGIN Header_Led */
/**
* @brief Function implementing the Led_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Led */
void Led(void const * argument)
{
  /* USER CODE BEGIN Led */
  char LCD_time_buf[32]; // 用于存储格式化的字符串
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
    HAL_GPIO_TogglePin(RUN_LED_GPIO_Port , RUN_LED_Pin);
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
// {
//     HAL_TIM_PWM_Stop_DMA(&htim5,TIM_CHANNEL_2);
// }
/* USER CODE END Application */
