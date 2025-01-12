/********************************************************************************
    * 文件名称 ：CRC.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-04-17
    * 功     能：CRC校验函数
*********************************************************************************/
#include "CRC.h"

/**
 * @brief   CRC16校验表
 *
 * @param   data    数据
 * @param   Len     数据长度
 *
 */
uint16_t CRC16(uint8_t *data, uint8_t Len) {
    uint16_t crc16 = 0xffff;

    for (int i = 0; i < Len; i++) {
        crc16 = (crc16 >> 8) ^ CRC16Table[(crc16 ^ data[i]) & 0xFF];
    }

    return crc16;
}

/**
 * @brief   CRC8校验表
 *
 * @param   data    数据
 * @param   Len     数据长度
 *
 */
uint8_t CRC8(uint8_t *data , uint8_t len)
{
    
	uint8_t crc8 = 0;

    while (len --)
    {
        crc8 = crc8 ^ (*data++);
        crc8 = CRC8Table[crc8];
    }

    return crc8;
}
