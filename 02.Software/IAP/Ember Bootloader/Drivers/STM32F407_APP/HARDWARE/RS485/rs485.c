#include "sys.h"            
#include "rs485.h"     
#include "delay.h"

uint8_t RS485_RX_BUF[64];  
uint8_t RS485_RX_CNT = 0;             

void RS485_Init(uint32_t bound)
{       
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
    
    USART_ClearFlag(USART2, USART_FLAG_TC);
      
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  

#ifdef RS485_ENABLE
    RCC_AHB1PeriphClockCmd(RS485_EN_CLK, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = RS485_EN_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(RS485_EN_PORT, &GPIO_InitStructure);
    RS485_RX_EN;
#endif                    
}

void RS485_Send_Data(uint8_t *buf, uint8_t len)
{
    uint8_t t;
#ifdef RS485_ENABLE
    RS485_TX_EN;    
#endif        
    for(t=0;t<len;t++)        
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);     
        USART_SendData(USART2, buf[t]);
    }     
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);    
    RS485_RX_CNT=0;      
#ifdef RS485_ENABLE
    RS485_RX_EN;
#endif
}

void RS485_Receive_Data(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen = RS485_RX_CNT;
    uint8_t i = 0;
    *len = 0;                
    // delay_ms(10);        
    if(rxlen == RS485_RX_CNT&&rxlen)
    {
        for(i = 0; i < rxlen; i++)
        {
            buf[i] = RS485_RX_BUF[i];    
        }        
        *len = RS485_RX_CNT;    
        RS485_RX_CNT=0;
    }
}
