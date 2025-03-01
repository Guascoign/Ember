#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h" 
#include "stdio.h"
#include "stdlib.h"

#define  UART_DEBUG

#ifdef UART_DEBUG
#define uart_log printf
#else
#define uart_log
#endif

#endif
