#include "ymodem.h"
#include "delay.h"

ymodem_t ymodem = {START_PROGRAM, 0, 0, DOWNLOAD_SECTOR_ADDR, 0, {0}};
download_buf_t recvBuf;
seq_queue_t rx_queue;

// 初始化queue_initiate(Q)
void queue_initiate(seq_queue_t *Q)
{
    Q->rear = 0;        
    Q->front = 0;
    Q->count = 0;
}

// 非空否queue_not_empty(Q)
//判断循环队列Q非空否，非空则返回1，否则返回0
int queue_not_empty(seq_queue_t *Q)
{
    if(Q->count != 0)
        return 1;
    else 
        return 0;
}

// 入队列queue_append(Q, x)
//把数据元素值x插入顺序循环队列Q的队尾，成功返回1，失败返回0
int queue_append(seq_queue_t *Q, uint8_t x)
{
    if(Q->count > 0 && Q->rear == Q->front)
    {    
        uart_log("queue is full! \n");
        return 0;
    }
    else
    {    
        Q->queue[Q->rear] = x;
        Q->rear = (Q->rear + 1) % MAX_QUEUE_SIZE;
        Q->count ++;
        return 1;
    }
}

// 出队列  queue_delete(Q, d)
//删除顺序循环队列Q的队头元素并赋值给d，成功则返回1，失败返回0
int queue_delete(seq_queue_t *Q, uint8_t *d)
{
    if(Q->count == 0)
    {    
        // uart_log("queue is empty! \n");
        return 0;
    }
    else
    {    *d = Q->queue[Q->front];
        Q->front = (Q->front + 1) % MAX_QUEUE_SIZE;
        Q->count--;
        return 1;
    }
}

// 取队头数据元素 queue_get(Q, d)
int queue_get(seq_queue_t Q, uint8_t *d)
{
    if(Q.count == 0)
    {
        // uart_log("queue is empty! \n");
        return 0;
    }
    else
    {
        *d = Q.queue[Q.front];
        return 1;
    }
}

uint16_t get_mcu_flash_sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)
        return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)
        return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)
        return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)
        return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)
        return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)
        return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)
        return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)
        return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)
        return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)
        return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)
        return FLASH_Sector_10; 
    else
	return FLASH_Sector_11;	
}

uint8_t mcu_flash_erase(uint32_t addr, uint8_t sector_num) 
{
	uint8_t i;
    uint32_t sector_addr;
	FLASH_Unlock();
	sector_addr = get_mcu_flash_sector(addr);
	for (i = 0; i < sector_num; ++i) 
	{
		FLASH_EraseSector(sector_addr, VoltageRange_3);
		if(sector_addr < FLASH_Sector_10)
			sector_addr += 0x08;
	}
	FLASH_Lock();
	return 1;
}

uint8_t mcu_flash_write(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
	FLASH_Status result;
	uint16_t i, data = 0;
	FLASH_Unlock();
	for (i = 0; i < length; i += 2) 
	{
		data = (*(buffer + i + 1) << 8) + (*(buffer + i));
		result = FLASH_ProgramHalfWord((uint32_t)(addr + i), data);
	}
	FLASH_Lock();

	if(result != FLASH_COMPLETE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
 
void mcu_flash_read(uint32_t addr, uint8_t *buffer, uint32_t length) 
{
	memcpy(buffer, (void *)addr, length);
}

void ymodem_ack(void) 
{
    uint8_t buf;
    buf = YMODEM_ACK;
    RS485_Send_Data(&buf, 1);
}

void ymodem_nack(void) 
{
    uint8_t buf;
    buf = YMODEM_NAK;
    RS485_Send_Data(&buf, 1);
}

void ymodem_c(void) 
{
    uint8_t buf;
    buf = YMODEM_C;
    RS485_Send_Data(&buf, 1);
}

void set_ymodem_status(process_status process) 
{
    ymodem.process = process;
}

process_status get_ymodem_status(void) 
{
    process_status process = ymodem.process;
    return process;
}

void ymodem_start(ymodem_callback cb) 
{
    if (ymodem.status == 0) 
    {
        ymodem.cb = cb;
    }
}

void ymodem_recv(download_buf_t *p) 
{
    uint8_t type = p->data[0];
    switch (ymodem.status) 
    {
        case 0:
            if (type == YMODEM_SOH) 
            {
                ymodem.process = BUSY;
                ymodem.addr = DOWNLOAD_SECTOR_ADDR;
                mcu_flash_erase(ymodem.addr, ERASE_SECTORS);
                ymodem_ack();
                ymodem_c();
                ymodem.status++;
            }
            else if (type == '1') 
            {
                uart_log("enter update mode\r\n");
                ymodem.process = UPDATE_PROGRAM;
            }
            break;
        case 1:
            if (type == YMODEM_SOH || type == YMODEM_STX) 
            {
                if (type == YMODEM_SOH) 
                {
                    mcu_flash_write(ymodem.addr, &p->data[3], 128);
                    ymodem.addr += 128;
                }
                else 
                {
                    mcu_flash_write(ymodem.addr, &p->data[3], 1024);
                    ymodem.addr += 1024;
                }
                ymodem_ack();
            }
            else if (type == YMODEM_EOT) 
            {
                ymodem_nack();
                ymodem.status++;
            }
            else 
            {
                ymodem.status = 0;
            }
            break;
        case 2:
            if (type == YMODEM_EOT) 
            {
                ymodem_ack();
                ymodem_c();
                ymodem.status++;
            }
            break;
        case 3:
            if (type == YMODEM_SOH) 
            {
                ymodem_ack();
                ymodem.status = 0;
                ymodem.process = UPDATE_SUCCESS;
            }
    }
    p->len = 0;
}

void system_reboot(void)
{
    __set_FAULTMASK(1);//关闭总中断
    NVIC_SystemReset();//请求单片机重启
}

void ymodem_handle(void)
{
    uint8_t boot_state;
    process_status process;

    process = get_ymodem_status();
    switch (process) 
    {
        case START_PROGRAM:
            break;
        case UPDATE_PROGRAM:
            ymodem_c();
            delay_ms(1000);
            break;
        case UPDATE_SUCCESS:
            boot_state = UPDATE_PROGRAM_STATE;
            mcu_flash_erase(SETTING_BOOT_STATE, 1);
            mcu_flash_write(SETTING_BOOT_STATE, &boot_state, 1);
            // mcu_flash_read(SETTING_BOOT_STATE, &boot_state, 1);
            // uart_log("boot_state:%d\r\n", boot_state);
            uart_log("firmware download success\r\n");
            uart_log("system reboot...\r\n");
            delay_ms(2000);
            system_reboot();
            break;
        default:
            break;
    }
}

void ymodem_init(void)
{
    RS485_Init(115200);
    timer_init();
    queue_initiate(&rx_queue);
}

void USART2_IRQHandler(void)
{
    uint8_t res;     
    // printf("%d", res);   
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {         
        res = USART_ReceiveData(USART2);
        queue_append(&rx_queue, res);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
    }        
    TIM3->CNT = 0;
    TIM_Cmd(TIM3, ENABLE);                                        
}  

void timer_init(void) 
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
    
    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值 (9999+1)*1us=10ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71; //设置用来作为TIMx时钟频率除数的预分频值 72M/(71+1)=1MHz 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIMx    
}

void TIM3_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) 
    {
        int result = 1;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIM_Cmd(TIM3, DISABLE);
        
        result = queue_not_empty(&rx_queue);
        if(result == 1)
        {
            recvBuf.len = 0;
            do
            {
                result = queue_delete(&rx_queue, &recvBuf.data[recvBuf.len]);
                if(result == 1)
                {
                    recvBuf.len ++;
                }
            }
            while(result);
            ymodem_recv(&recvBuf);
        }
    }
}
