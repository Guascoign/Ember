#include "BEEP/beep.h"

/*创建Beeper的handle*/
BEEPER_Tag Beeper0;

/*C4-B8的音调对应的频率大小*/
const uint16_t MusicNoteFrequency[] = {
    // rest_note
    0,
    // C    C#   D   Eb    E    F    F#   G    G#   A   Bb    B
    262,
    277,
    294,
    311,
    330,
    349,
    370,
    392,
    415,
    440,
    466,
    494,
    523,
    554,
    587,
    622,
    659,
    698,
    740,
    784,
    830,
    880,
    932,
    988,
    1047,
    1109,
    1175,
    1245,
    1319,
    1397,
    1480,
    1568,
    1661,
    1760,
    1865,
    1976,
    2093,
    2218,
    2349,
    2489,
    2637,
    2794,
    2960,
    3136,
    3322,
    3520,
    3729,
    3951,
    4186,
    4435,
    4699,
    4978,
    5274,
    5588,
    5920,
    6272,
    6645,
    7040,
    7459,
    7902,
    // check_note
    0,
};

/*全局TONE结构体指针，用于定时器TIM4中断函数中*/
TONE *MySound;

/*乐曲*/
TONE const BEEPER_KEYPRESS[] = {
    {NOTE_C6, 7},
    {CHECK_NOTE, 0}, // 检查位
};

TONE const BEEPER_TRITONE[] = {
    {NOTE_B5, 7},
    {REST_NOTE, 2},
    {NOTE_D6, 6},
    {REST_NOTE, 2},
    {NOTE_F6, 6},
    {CHECK_NOTE, 0}, // 检查位
};

TONE const BEEPER_WARNING[] = {
    {NOTE_F4, 5},
    {REST_NOTE, 2},
    {NOTE_F4, 5},
    {CHECK_NOTE, 0}, // 检查位
};

TONE const TWINKLE_TWINKLE[] = {
    {NOTE_C4, 4},  {REST_NOTE, 8},{NOTE_C4, 4}, {REST_NOTE, 8}, // 小星星
    {NOTE_G4, 4},  {REST_NOTE, 8},{NOTE_G4, 4}, {REST_NOTE, 8},
    {NOTE_A4, 8},  {REST_NOTE, 8},{NOTE_A4, 4}, {REST_NOTE, 8},
    {NOTE_G4, 8},  {REST_NOTE, 8},              // 一闪一闪亮晶晶
    {NOTE_F4, 4}, {REST_NOTE, 8}, {NOTE_F4, 4}, {REST_NOTE, 8}, // 满天都是小星星
    {NOTE_E4, 4},  {REST_NOTE, 8},{NOTE_E4, 4}, {REST_NOTE, 8},
    {NOTE_D4, 4},  {REST_NOTE, 8},{NOTE_D4, 4}, {REST_NOTE, 8},
    {NOTE_C4,8},   {NOTE_C4,8},                        
    {CHECK_NOTE, 0}                           // 检查位
};



TONE const BEEP2[] = {
    {REST_NOTE, 20},
    {REST_NOTE, 20},
    {REST_NOTE, 20},
    {NOTE_C5, 10},
    {NOTE_B4, 10},
    {NOTE_A4, 20},
    {NOTE_E5, 40},
    {NOTE_C5, 10},
    {NOTE_A4, 10},

    {NOTE_B4, 20},
    {NOTE_F5, 20},
    {NOTE_E5, 10},
    {NOTE_D5, 30},

    {NOTE_C5, 10},
    {NOTE_D5, 10},
    {NOTE_C5, 10},
    {NOTE_D5, 10},
    {NOTE_E5, 20},
    {NOTE_C5, 10},
    {NOTE_B4, 10},

    {NOTE_A4, 20},
    {NOTE_D5, 20},
    {NOTE_C5, 10},
    {NOTE_B4, 10},
    {REST_NOTE, 10},
    {NOTE_A4, 5},
    {NOTE_B4, 5},

    {NOTE_C5, 20},
    {NOTE_A4, 20},
    {NOTE_E5, 20},
    {NOTE_C5, 20},

    {NOTE_D5, 20},
    {NOTE_A5, 20},
    {NOTE_G5, 20},
    {NOTE_F5, 10},
    {NOTE_E5, 5},
    {NOTE_D5, 5},

    {NOTE_E5, 80},

    {CHECK_NOTE, 0}, // 检查位
};

TONE const BOOT[] = {
    {REST_NOTE, 2},  // 休止
    {NOTE_G4, 4},  // 音符G4
    {REST_NOTE, 2},  // 休止
    {NOTE_C5, 4},  // 中高音C5
    {NOTE_E5, 8},  // 高音E5
    {REST_NOTE, 4},  // 休止
    {CHECK_NOTE, 0}  // 结束标志
};






void TIM3_PWM_Init(uint16_t arr)
{
    TIM_HandleTypeDef htim3;
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* 开启TIM3时钟 */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3基础配置 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 100 - 1; // 预分频器，输入时钟为1 MHz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数模式
    htim3.Init.Period = arr - 1; // 自动重装载值
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 无时钟分频
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // 禁用自动重装载预装载
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        Error_Handler(); // 错误处理
    }

    /* 配置PWM通道 */
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // PWM模式1
    sConfigOC.Pulse = arr / 2;          // 占空比为50%
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // 输出高电平有效
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;  // 禁用快速模式
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler(); // 错误处理
    }
    /* 启动PWM输出 */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);

    /* 启用主输出（必要时开启，通常用于高级定时器） */
    __HAL_TIM_MOE_ENABLE(&htim3);
}


void Beeper_Init(void)
{
    /* 声明GPIO初始化结构体 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 声明TIM句柄 */
    TIM_HandleTypeDef htim3;

    /* 开启GPIOB时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* 开启TIM3时钟 */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* 配置PA3为TIM3_CH2功能 */
    GPIO_InitStruct.Pin = BEEPER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // 复用推挽模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;     // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;    // 复用功能为TIM3
    HAL_GPIO_Init(BEEPER_GPIO_Port, &GPIO_InitStruct);

    TIM3_PWM_Init(1000);
    

    /* 设置蜂鸣器标志 */
    Beeper0.Beeper_Enable = 1;
    Beeper0.Beeper_Continue_Flag = 0;

    Beeper0.Sound_Loud = 20;
}


/*计算对应的预重装值ARR（psc = 10 - 1）//用1000kHz / 音调频率*/
uint16_t Set_Musical_Note(uint16_t frq)
{
    /*防止休止符时蜂鸣器怪叫*/
    if (frq == 0)
        return 0;
    float temp = 0;
    temp = 1000000.0f / (float)frq;
    return (uint16_t)temp;
}

void Beeper_Set_Musical_Tone(uint16_t frq)
{
    float temp = 0;

    /* 如果定时器未启动，则启动PWM通道 */
    if ((htim3.Instance->CR1 & TIM_CR1_CEN) == 0)
    {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    }

    /* 计算自动重装载值（ARR） */
    temp = 1000000.0f / (float)frq;
    __HAL_TIM_SET_AUTORELOAD(&htim3, (uint16_t)temp);
}

/**
 * @brief Beeper的应用层函数
 * @param  TONE *Sound 传入结构体数组
 * @retval 无
 */
void Beeper_Perform(const TONE *Sound)
{
    /* 定义变量以计算结构体数组长度 */
    uint16_t Note_Length;

    /* 停止PWM输出 */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);

    /* 将全局结构体指针指向传入的乐曲 */
    MySound = (TONE *)Sound;

    /* 通过遍历计算传入结构体数组的长度 */
    for (Note_Length = 0; MySound[Note_Length].Note != CHECK_NOTE; Note_Length++)
        ;

    /* 赋值音符表的长度 */
    Beeper0.Sound_Size = Note_Length;

    /* 清零音符表索引 */
    Beeper0.Beep_Play_Schedule = 0;

    /* 启用蜂鸣器播放标志 */
    Beeper0.Beeper_Continue_Flag = 1;
    Beeper0.Beeper_Count = 0;
}


/* 用于10ms定时器中断进行循环 */
void Beeper_Proc(void)
{
    /* 判断是否继续播放 */
    if (Beeper0.Beeper_Continue_Flag && Beeper0.Beeper_Enable)
    {
        /* 判断音符表是否播放完毕 */
        if (Beeper0.Beep_Play_Schedule < Beeper0.Sound_Size)
        {
            /* 时间减少10ms */
            Beeper0.Beeper_Count--;
            
            /* 判断是否需要切换音符 */
            if (!(Beeper0.Beeper_Count < 65535))
            {
                /* 设置新的音符频率 */
                uint16_t arr_value = Set_Musical_Note(MusicNoteFrequency[MySound[Beeper0.Beep_Play_Schedule].Note]);
                __HAL_TIM_SET_AUTORELOAD(&htim3, arr_value);

                /* 设置新的占空比 */
                uint16_t ccr_value = arr_value / (100 - Beeper0.Sound_Loud);
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, ccr_value);

                /* 更新延时计数 */
                Beeper0.Beeper_Count = MySound[Beeper0.Beep_Play_Schedule].Delay;

                /* 跳转到下一个音符 */
                Beeper0.Beep_Play_Schedule++;

                /* 启动PWM输出 */
                HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
            }
        }
        /* 播放结束，关闭蜂鸣器 */
        else
        {
            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
            Beeper0.Beeper_Continue_Flag = 0;
            HAL_GPIO_WritePin(BEEPER_GPIO_Port, BEEPER_Pin, GPIO_PIN_RESET);
        }
    }
    else
    {
        /* 关闭蜂鸣器 */
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
        Beeper0.Beeper_Continue_Flag = 0;
        HAL_GPIO_WritePin(BEEPER_GPIO_Port, BEEPER_Pin, GPIO_PIN_RESET);
    }
}

