/********************************************************************************
     *�ļ����� ��at24cxx.h
     *��     �ߣ�ZM
     *��     ����V1.0
     *��д���� ��2024-05-09
     *��     �ܣ�AT24CXX����ͷ�ļ�
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

#define MPU6050_INTBP_CFG_REG	  0X37	//�жϼĴ���
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
//������ԭ�����ݽṹ��
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

//�����ǽǶȽṹ��
typedef struct MPU_data
{
    float yaw;
    float roll;
    float pitch;
    float temperature;
}MPU_data;


typedef struct MPU_Device
{
    char *name;             //����
    IIC_BusTypeDef *IIC_Bus;                          // IIC�豸�ӿ�
    // void (*MPU_Init)(struct MPU_Device *p_MPUDev); //��ʼ��MPU
    // uint8_t (*MPU_ReadOneByte)(struct MPU_Device *p_MPUDev, uint16_t ReadAddr);
    // void (*MPU_WriteOneByte)(struct MPU_Device *p_MPUDev, uint16_t WriteAddr, uint8_t DataToWrite);
    // uint8_t (*MPU_Check)(struct MPU_Device *p_MPUDev);
    uint8_t (*MPU_ID)(struct MPU_Device *p_MPUDev);
    MPU_rawdata *p_rawdata; //ԭ������
    MPU_data *p_data;   //����
    void *priv_data;//˽������
}MPU_DeviceDef;


// void MPU6050_init(struct MPU_Device *p_MPUDev);   //��ʼ��������
// void MPU6050_Get_Raw(struct MPU_Device *p_MPUDev);                            //�õ�ԭ������
// void MPU6050_Get_Angle(struct MPU_Device *p_MPUDev);                              //�õ��Ƕ�
// float MPU6050_GetTemp(struct MPU_Device *p_MPUDev);                                            //�õ��¶�
uint8_t MPU6050_ID(struct MPU_Device *p_MPUDev);                                               //��ȡID
struct MPU_Device *MPU_GetDevice(char *name);
#endif
