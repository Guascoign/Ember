/********************************************************************************
     *文件名称 ：at24cxx.c
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动
*********************************************************************************/
#include "FLASH/w25qxx.h"
#include "string.h"

#ifdef USE_AT24C01
struct AT24CXX_Type AT24C01 = { 
    .EEP_SIZE = 128,
    .EEP_PAGENUM = 16,
    .EEP_PAGESIZE = 8,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C02
struct AT24CXX_Type AT24C02 = { 
    .EEP_SIZE = 256,
    .EEP_PAGENUM = 32,
    .EEP_PAGESIZE = 8,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C04
struct AT24CXX_Type AT24C04 = { 
    .EEP_SIZE = 512,
    .EEP_PAGENUM = 32,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C08
struct AT24CXX_Type AT24C08 = { 
    .EEP_SIZE = 1024,
    .EEP_PAGENUM = 64,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C16
struct AT24CXX_Type AT24C16 = { 
    .EEP_SIZE = 2048,
    .EEP_PAGENUM = 128,
    .EEP_PAGESIZE = 16,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C32
struct AT24CXX_Type AT24C32 = { 
    .EEP_SIZE = 4096,
    .EEP_PAGENUM = 128,
    .EEP_PAGESIZE = 32,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C64
struct AT24CXX_Type AT24C64 = { 
    .EEP_SIZE = 8192,
    .EEP_PAGENUM = 256,
    .EEP_PAGESIZE = 32,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C128
struct AT24CXX_Type AT24C128 = { 
    .EEP_SIZE = 16384,
    .EEP_PAGENUM = 512,
    .EEP_PAGESIZE = 64,
    .ADDR = AT24_Address
};
#endif

#ifdef USE_AT24C256
struct AT24CXX_Type AT24C256 = { 
    .EEP_SIZE = 32768,
    .EEP_PAGENUM = 512,
    .EEP_PAGESIZE = 128,
    .ADDR = AT24_Address
};
#endif
