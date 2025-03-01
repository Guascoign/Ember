/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BSP/UART/uart_device.h"
#include "BSP/IAP/bootloader.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BOOTLOADER_VERSION "V1.0"



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  UART_DeviceTypeDef Test_Uart;   //测试串口
  boot_parameterTypedef boot = {
    .process = START_PROGRAM,
    .app_addr = APP_SECTOR_ADDR,
    .update_addr = DOWNLOAD_SECTOR_ADDR,
    .backup_addr = BACKUP_SECTOR_ADDR,
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  boot = Get_boot_parameter(SETTING_SECTOR_ADDR);
	boot.app_addr = APP_SECTOR_ADDR;
  boot.update_addr = DOWNLOAD_SECTOR_ADDR;
  boot.backup_addr = BACKUP_SECTOR_ADDR;
  boot.retry_times = 0;
  uint16_t i;
  uint8_t update_buf[128];
  uint32_t update_addr;
  uint32_t app_addr;

  /*注册测试串口*****************/
  UART_Init(&Test_Uart,"Test_Uart1",USART1,UART_MODE_NORMAL,Rate_115200,Word_length_8B,PARITY_NONE,Stop_bits_1);

  BOOT_printf("======================================\r\n");
  BOOT_printf("   ______  ______  _______ \r\n");
  BOOT_printf("  / __/  |/  / _ )/ __/ _ \\\r\n");
  BOOT_printf(" / _// /|_/ / _  / _// , _/\r\n");
  BOOT_printf("/___/_/  /_/____/___/_/|_| \r\n\r\n");
  Print_Flash_Sector();
  Print_MCU_Info();
  BOOT_printf("======================================\r\n");
  BOOT_printf("Bootloader Version: %s\r\n", BOOTLOADER_VERSION);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(boot.retry_times > 3)
    {
      BOOT_printf("[Error]End Retry\r\n");
      while (1)
      {
        HAL_Delay(1000);//进入死循环
      }
    }

    switch (boot.process) 
    {
      case START_PROGRAM:
      {
        BOOT_printf("[Info]Start app...\r\n");
          if (!jump_app(boot.app_addr)) 
          {
            BOOT_printf("[Warning]No program\r\n");
            boot.retry_times++;
            HAL_Delay(1000);
          }
          BOOT_printf("[Error]Start app failed\r\n");
        break;
      }
        
      case UPDATE_PROGRAM:
      {
        BOOT_printf("[Info]Update app program...\r\n");
        app_addr = boot.app_addr;
        update_addr = boot.update_addr;
        BOOT_printf("[Info]Erase MCU flash...\r\n");
        if(MCU_Flash_Erase(app_addr, APP_ERASE_SECTORS_NUM))
        {
          BOOT_printf("[Info]MCU flash erase success\r\n");
        }
        BOOT_printf("[Info]Write MCU flash...\r\n");
          for (i = 0; i < (APP_SECTOR_SIZE / FLASH_SECTOR_SIZE); i++)
          {
            MCU_Flash_Read(update_addr, &update_buf[0], FLASH_SECTOR_SIZE);
            HAL_Delay(5);
            MCU_Flash_Write(app_addr, &update_buf[0], FLASH_SECTOR_SIZE);
            HAL_Delay(5);
            update_addr += FLASH_SECTOR_SIZE;
            app_addr += FLASH_SECTOR_SIZE;
          }
          BOOT_printf("[Info]MCU flash write success\r\n");
        boot.process = UPDATE_SUCCESS;
        boot.retry_times = 0;
        Write_boot_parameter(SETTING_SECTOR_ADDR, boot);
        BOOT_printf("[Info]boot_parameter Saved\r\n");
        HAL_Delay(1000);
        BOOT_printf("[Info]Start app...\r\n");
        if (!jump_app(boot.app_addr)) 
        {
          BOOT_printf("[Warning]No program\r\n");
          boot.process = UPDATE_ERROR;
          HAL_Delay(1000);
        }
        BOOT_printf("[Error]Start app failed\r\n");
          break;

      }
        
      case UPDATE_SUCCESS://在主程序中判断是否更新成功，成功置为START_PROGRAM
      {
        BOOT_printf("[Info]UPDATE_SUCCESS...\r\n");
        boot.process = START_PROGRAM;
        Write_boot_parameter(SETTING_SECTOR_ADDR, boot);
        BOOT_printf("[Info]boot_parameter Saved\r\n");
        break;
      }

      case UPDATE_ERROR://更新失败，跳转到备份程序
      {
        BOOT_printf("[Info]UPDATE_ERROR...\r\n");
        HAL_Delay(1000);
          if (!jump_app(boot.backup_addr)) 
          {
            BOOT_printf("[Warning]No program\r\n");
            HAL_Delay(1000);
            boot.retry_times++;
          }
          BOOT_printf("[Error]Backup app failed\r\n");
        break;
      }

      default:
      {
        BOOT_printf("[Error]Unknown Status\r\n");
        boot.retry_times++;
        boot.process = START_PROGRAM;
        Write_boot_parameter(SETTING_SECTOR_ADDR, boot);
        if(boot.retry_times > 3)
        {
          BOOT_printf("[Error]End Retry\r\n");
          while (1)
          {
            HAL_Delay(1000);//进入死循环
          }
        }
        break;
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
