/********************************************************************************
     *文件名称 ：at24cxx.h
     *作     者：ZM
     *版     本：V1.0
     *编写日期 ：2024-05-09
     *功     能：AT24CXX驱动头文件
*********************************************************************************/
#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f4xx.h"
#include "IIC/iic.h"

#define MPU6050_ADDRESS		      0x68 //i2c address    
//**************************** register below
#define	MPU6050_SMPLRT_DIV		  0x19
#define	MPU6050_CONFIG			  0x1A
#define	MPU6050_GYRO_CONFIG		  0x1B
#define	MPU6050_ACCEL_CONFIG	  0x1C
#define MPU6050_FIFO_EN           0x23

#define MPU6050_INTBP_CFG_REG	  0X37	//中断寄存器
#define MPU6050_INT_ENABLE        0x38

#define	MPU6050_ACCEL_XOUT_H	  0x3B
#define	MPU6050_ACCEL_XOUT_L	  0x3C
#define	MPU6050_ACCEL_YOUT_H	  0x3D
#define	MPU6050_ACCEL_YOUT_L	  0x3E
#define	MPU6050_ACCEL_ZOUT_H	  0x3F
#define	MPU6050_ACCEL_ZOUT_L	  0x40
#define	MPU6050_TEMP_OUT_H		  0x41
#define	MPU6050_TEMP_OUT_L		  0x42
#define	MPU6050_GYRO_XOUT_H		  0x43
#define	MPU6050_GYRO_XOUT_L		  0x44
#define	MPU6050_GYRO_YOUT_H		  0x45
#define	MPU6050_GYRO_YOUT_L		  0x46
#define	MPU6050_GYRO_ZOUT_H		  0x47
#define	MPU6050_GYRO_ZOUT_L		  0x48
#define MPU6050_SIGNAL_PATH_RESET 0x68

#define MPU6050_USER_CTRL         0x6A
#define	MPU6050_PWR_MGMT_1		  0x6B
#define	MPU6050_WHO_AM_I		  0x75
//陀螺仪原生数据结构体
typedef struct MPU_rawdata
{
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
    uint16_t Temp;
    uint8_t ID;
}MPU_rawdata;

//陀螺仪角度结构体
typedef struct MPU_data
{
    float yaw;
    float roll;
    float pitch;
    float temperature;
}MPU_data;


typedef struct MPU_Device
{
    char *name;             //名称
    IIC_BusTypeDef *IIC_Bus;                          // IIC设备接口
    // void (*MPU_Init)(struct MPU_Device *p_MPUDev); //初始化MPU
    // uint8_t (*MPU_ReadOneByte)(struct MPU_Device *p_MPUDev, uint16_t ReadAddr);
    // void (*MPU_WriteOneByte)(struct MPU_Device *p_MPUDev, uint16_t WriteAddr, uint8_t DataToWrite);
    // uint8_t (*MPU_Check)(struct MPU_Device *p_MPUDev);
    uint8_t (*MPU_ID)(struct MPU_Device *p_MPUDev);
    MPU_rawdata *p_rawdata; //原生数据
    MPU_data *p_data;   //数据
    void *priv_data;//私有数据
}MPU_DeviceDef;


// void MPU6050_init(struct MPU_Device *p_MPUDev);   //初始化陀螺仪
// void MPU6050_Get_Raw(struct MPU_Device *p_MPUDev);                            //得到原生数据
// void MPU6050_Get_Angle(struct MPU_Device *p_MPUDev);                              //得到角度
// float MPU6050_GetTemp(struct MPU_Device *p_MPUDev);                                            //得到温度
uint8_t MPU6050_ID(struct MPU_Device *p_MPUDev);                                               //读取ID
struct MPU_Device *MPU_GetDevice(char *name);
#endif
