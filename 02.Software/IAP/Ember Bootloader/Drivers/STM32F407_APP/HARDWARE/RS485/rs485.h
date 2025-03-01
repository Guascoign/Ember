#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  

extern uint8_t RS485_RX_BUF[64]; 		
extern uint8_t RS485_RX_CNT;   		

#define RS485_ENABLE

#define RS485_EN_CLK   RCC_AHB1Periph_GPIOG
#define RS485_EN_PORT  GPIOG
#define RS485_EN_PIN   GPIO_Pin_8

#define RS485_TX_EN		  GPIO_SetBits(RS485_EN_PORT, RS485_EN_PIN);
#define RS485_RX_EN		  GPIO_ResetBits(RS485_EN_PORT, RS485_EN_PIN);
						 
void RS485_Init(uint32_t bound);
void RS485_Send_Data(uint8_t *buf,uint8_t len);
void RS485_Receive_Data(uint8_t *buf,uint8_t *len);		 

#endif	   
















