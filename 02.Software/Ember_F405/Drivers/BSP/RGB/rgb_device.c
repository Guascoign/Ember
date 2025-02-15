
#include "BSP/RGB/rgb_device.h"
#include "main.h"
#include "tim.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_tim.h"

/* 定时器句柄 */
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
/* DMA 句柄 */
extern DMA_HandleTypeDef hdma_tim4_ch3;

/*单个灯珠的需要传输的数据对应的比较值数组*/
uint32_t Single_WS2812B_Buffer[DMA_DATA_SIZE] = {0}; // 24bit*灯珠数量 + Reset_Data(1.25us*50>50us)

/**
 * @brief TIM5_PWM_CH2&DMA1&PA1初始化
 * @param  arr重装载值（105 - 1）
 * @retval 无
 */
void PWM_WS2812B_Init(uint16_t arr)
{
    /* 初始化GPIO结构体 */
    GPIO_InitTypeDef GPIO_InitStruct;
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_HandleTypeDef htim4;
		TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC;
    DMA_HandleTypeDef hdma_tim4_ch3;

    /* 使能时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* 初始化GPIO引脚口 */
    GPIO_InitStruct.Pin = RGB_Pin;               // RGB_Pin对应GPIO_PIN_8
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // 设置为复用推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(RGB_GPIO_Port, &GPIO_InitStruct);


    /* 配置TIM4 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;                       // 不分频
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;    // 向上计数模式
    htim4.Init.Period = arr;                        // 设置ARR寄存器
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.RepetitionCounter = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

    /* 配置DMA */
hdma_tim4_ch3.Instance = DMA1_Stream7;           // 使用DMA1 Stream 7
hdma_tim4_ch3.Init.Channel = DMA_CHANNEL_2;      // 使用DMA频道2 (确保与TIM4_CH3匹配)
hdma_tim4_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH; // 内存到外设
hdma_tim4_ch3.Init.PeriphInc = DMA_PINC_DISABLE;     // 外设地址不增加
hdma_tim4_ch3.Init.MemInc = DMA_MINC_ENABLE;         // 内存地址增加
hdma_tim4_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;  // 外设数据对齐为字节
hdma_tim4_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    // 内存数据对齐为字节
hdma_tim4_ch3.Init.Mode = DMA_CIRCULAR;                 // 普通模式
hdma_tim4_ch3.Init.Priority = DMA_PRIORITY_MEDIUM;     // 优先级中等
hdma_tim4_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;    // 禁用FIFO模式

if (HAL_DMA_Init(&hdma_tim4_ch3) != HAL_OK) {
    Error_Handler();
}

/* 关联DMA和TIM4_CH3 */
__HAL_LINKDMA(&htim4, hdma[TIM_DMA_ID_CC3], hdma_tim4_ch3);

}



void WS2812B_Write_24Bits(uint16_t num, uint32_t GRB_Data)
{
	uint8_t i, j;
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < DATA_SIZE; i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			Single_WS2812B_Buffer[i + j * DATA_SIZE] = ((GRB_Data << i) & 0x800000) ? T1H : T0H;
		}
	}
}

void WS2812B_Write_24Bits_independence(uint16_t num, uint32_t *GRB_Data)
{
	uint8_t i, j;
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < DATA_SIZE; i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			Single_WS2812B_Buffer[i + j * DATA_SIZE] = ((GRB_Data[j] << i) & 0x800000) ? T1H : T0H;
		}
	}
}

void WS2812B_Show(void) {

    // 启动 DMA 数据传输
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)Single_WS2812B_Buffer, DMA_DATA_SIZE);
}



// N个灯珠发红光
void PWM_WS2812B_Red(uint16_t num)
{
	WS2812B_Write_24Bits(num, 0x00ff00);
	WS2812B_Show();
}
// N个灯珠发绿光
void PWM_WS2812B_Green(uint16_t num)
{
	WS2812B_Write_24Bits(num, 0xff0000);
	WS2812B_Show();
}
// N个灯珠发蓝光
void PWM_WS2812B_Blue(uint16_t num)
{
	WS2812B_Write_24Bits(num, 0x0000ff);
	WS2812B_Show();
}
// N个灯珠关闭
void PWM_WS2812B_Close(uint16_t num)
{
	WS2812B_Write_24Bits(num, 0x000000);
	WS2812B_Show();
}
/*呼吸灯*/
void set_ws2812_breathing(uint8_t index)
{
	int i = 0;
	switch (index)
	{
	case 0: /* red */
		for (i = 0; i < 254; i += 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(0x00 << 16 | i << 8 | 0x00));
			WS2812B_Show();
		}
		for (i = 254; i > 0; i -= 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(0x00 << 16 | i << 8 | 0x00));
			WS2812B_Show();
		}
		break;
	case 1: /* green */
		for (i = 0; i < 254; i += 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(i << 16 | 0x00 << 8 | 0x00));
			WS2812B_Show();
		}
		for (i = 254; i > 0; i -= 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(i << 16 | 0x00 << 8 | 0x00));
			WS2812B_Show();
		}
		break;
	case 2: /* blue */
		for (i = 0; i < 254; i += 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(0x00 << 16 | 0x00 << 8 | i));
			WS2812B_Show();
		}
		for (i = 254; i > 0; i -= 2)
		{
			WS2812B_Write_24Bits(64, (uint32_t)(0x00 << 16 | 0x00 << 8 | i));
			WS2812B_Show();
		}
		break;
	}
}
