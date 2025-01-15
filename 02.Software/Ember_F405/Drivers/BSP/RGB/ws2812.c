/********************************************************************************
    * 文件名称 ：ws2812.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-13
    * 功     能：LCD屏幕驱动
*********************************************************************************/
#include "BSP/RGB/ws2812.h"

/**
 * @brief   WS2812B初始化
 * @param   p_ws2812b   WS2812B设备句柄
 * @param   arr         定时器ARR值
 */
static void init(WS2812B_DeviceTypeDef *p_ws2812b , uint16_t arr)
{
//    /* 初始化GPIO结构体 */
//    GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//    TIM_HandleTypeDef htim4;
//	TIM_MasterConfigTypeDef sMasterConfig = {0};
//    TIM_OC_InitTypeDef sConfigOC;
//    DMA_HandleTypeDef hdma_tim4_ch3;

//    /* 使能时钟 */
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_TIM4_CLK_ENABLE();
//    __HAL_RCC_DMA1_CLK_ENABLE();

//    /* 初始化GPIO引脚口 */
//    GPIO_InitStruct.Pin = RGB_Pin;               // RGB_Pin对应GPIO_PIN_8
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // 设置为复用推挽输出
//    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
//    HAL_GPIO_Init(RGB_GPIO_Port, &GPIO_InitStruct);


//    /* 配置TIM4 */
//    htim4.Instance = TIM4;
//    htim4.Init.Prescaler = 0;                       // 不分频
//    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;    // 向上计数模式
//    htim4.Init.Period = arr;                        // 设置ARR寄存器
//    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//    htim4.Init.RepetitionCounter = TIM_AUTORELOAD_PRELOAD_DISABLE;
//    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//  sConfigOC.Pulse = 0;
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
//  {
//    Error_Handler();
//  }

//    /* 配置DMA */
//hdma_tim4_ch3.Instance = DMA1_Stream7;           // 使用DMA1 Stream 7
//hdma_tim4_ch3.Init.Channel = DMA_CHANNEL_2;      // 使用DMA频道2 (确保与TIM4_CH3匹配)
//hdma_tim4_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH; // 内存到外设
//hdma_tim4_ch3.Init.PeriphInc = DMA_PINC_DISABLE;     // 外设地址不增加
//hdma_tim4_ch3.Init.MemInc = DMA_MINC_ENABLE;         // 内存地址增加
//hdma_tim4_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;  // 外设数据对齐为字节
//hdma_tim4_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    // 内存数据对齐为字节
//hdma_tim4_ch3.Init.Mode = DMA_CIRCULAR;                 // 普通模式
//hdma_tim4_ch3.Init.Priority = DMA_PRIORITY_MEDIUM;     // 优先级中等
//hdma_tim4_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;    // 禁用FIFO模式

//if (HAL_DMA_Init(&hdma_tim4_ch3) != HAL_OK) {
//    Error_Handler();
//}

///* 关联DMA和TIM4_CH3 */
//__HAL_LINKDMA(&htim4, hdma[TIM_DMA_ID_CC3], hdma_tim4_ch3);

}

/**
 * @brief   设置单个灯的RGB值
 * @param   *p_RGBDev   RGB设备句柄
 * @param   R   红色
 * @param   G   绿色
 * @param   B   蓝色
 * @param   num 灯的编号
 */
static void set_rgb(WS2812B_DeviceTypeDef *p_ws2812b, uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    uint16_t i = num * DATA_SIZE;
    uint8_t j;
    for (j = 0; j < 8; j++)
    {
        if (G & (0x80 >> j))
        {
            p_ws2812b->data[i + j] = T1H;
        }
        else
        {
            p_ws2812b->data[i + j] = T0H;
        }
    }
    for (j = 0; j < 8; j++)
    {
        if (R & (0x80 >> j))
        {
            p_ws2812b->data[i + j + 8] = T1H;
        }
        else
        {
            p_ws2812b->data[i + j + 8] = T0H;
        }
    }
    for (j = 0; j < 8; j++)
    {
        if (B & (0x80 >> j))
        {
            p_ws2812b->data[i + j + 16] = T1H;
        }
        else
        {
            p_ws2812b->data[i + j + 16] = T0H;
        }
    }
}

/**
 * @brief   设置单个灯的HSV值
 * @param   *p_RGBDev   RGB设备句柄
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 * @param   num 灯的编号
 */
static void set_hsv(WS2812B_DeviceTypeDef *p_ws2812b, uint8_t H, uint8_t S, uint8_t V, uint16_t num)
{
    RGB_TypeDef rgb;
    HSV_TypeDef hsv;
    hsv.H = H;
    hsv.S = S;
    hsv.V = V;
    rgb = HSV_To_RGB(hsv);
    set_rgb(p_ws2812b, rgb.R, rgb.G, rgb.B, num);
}

/**
 * @brief   设置所有灯的RGB值
 * @param   *p_RGBDev   RGB设备句柄
 * @param   R   红色
 * @param   G   绿色
 * @param   B   蓝色
 */
static void set_allrgb(WS2812B_DeviceTypeDef *p_ws2812b, uint8_t R, uint8_t G, uint8_t B)
{
    uint16_t i;
    for (i = 0; i < WS2812B_NUM; i++)
    {
        set_rgb(p_ws2812b, R, G, B, i);
    }
}

/**
 * @brief   设置所有灯的HSV值
 * @param   *p_RGBDev   RGB设备句柄
 * @param   H   色调
 * @param   S   饱和度
 * @param   V   明度
 */
static void set_allhsv(WS2812B_DeviceTypeDef *p_ws2812b, uint8_t H, uint8_t S, uint8_t V)
{
    uint16_t i;
    for (i = 0; i < WS2812B_NUM; i++)
    {
        set_hsv(p_ws2812b, H, S, V, i);
    }
}

/**
 * @brief   发送灯珠颜色数据
 * @param   *p_RGBDev   RGB设备句柄
 */
static void update(WS2812B_DeviceTypeDef *p_ws2812b)
{
    //HAL_TIM_PWM_Start_DMA(p_ws2812b->tim_handle, p_ws2812b->tim_channel, (uint32_t *)p_ws2812b->data, DMA_DATA_SIZE);
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)p_ws2812b->data, DMA_DATA_SIZE);
}

/**
 * @brief   WS2812B初始化
 * @param   p_ws2812b   WS2812B设备句柄
 * @param   tim_handle  定时器句柄
 * @param   tim_channel 定时器通道
 * @param   name        设备名称
 */
void WS2812B_Init(WS2812B_DeviceTypeDef *p_ws2812b,void *tim_handle, uint32_t tim_channel , char *name )
{
    p_ws2812b->tim_handle = tim_handle;
    p_ws2812b->tim_channel = tim_channel;
    p_ws2812b->name = name;
    p_ws2812b->Init = init;
    //p_ws2812b->DeInit = deinit;
    p_ws2812b->SetRGB = set_rgb;
    p_ws2812b->SetHSV = set_hsv;
    p_ws2812b->SetAllRGB = set_allrgb;
    p_ws2812b->SetAllHSV = set_allhsv;
    p_ws2812b->Update = update;
}
