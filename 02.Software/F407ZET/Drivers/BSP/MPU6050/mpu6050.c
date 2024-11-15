#include "mpu6050.h"
#include "usart.h"   


/**
 * @brief   ��ʼ��MPU6050
 *
 * @param   p_MPUDev   MPU6050�洢���豸�ṹ��ָ��
 * 
 * @return  ����ֵ:0,�ɹ�   1,����
 */
static uint8_t mpu6050_Init(struct MPU_Device *p_MPUDev)
{
    #ifdef Debug_Logs
        printf("MPU_Init\r\n");
    #endif   
	uint8_t res;
 	p_MPUDev->IIC_Device->IIC_Init(p_MPUDev->IIC_Device);//IIC��ʼ��
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
	HAL_Delay(100);
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
//	p_MPUDev->MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
//	p_MPUDev->MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
//	p_MPUDev->MPU_Set_Rate(50);						//���ò�����50Hz
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_INT_EN_REG,0X00);	//�ر������ж�
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=p_MPUDev->MPU_ReadByte(p_MPUDev,MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		p_MPUDev->MPU_WriteByte(p_MPUDev,MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
//		p_MPUDev->MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}



// //����MPU6050�����Ǵ����������̷�Χ
// //fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
// //����ֵ:0,���óɹ�
// //    ����,����ʧ�� 
// uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
// {
// 	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
// }
// //����MPU6050���ٶȴ����������̷�Χ
// //fsr:0,��2g;1,��4g;2,��8g;3,��16g
// //����ֵ:0,���óɹ�
// //    ����,����ʧ�� 
// uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
// {
// 	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
// }
// //����MPU6050�����ֵ�ͨ�˲���
// //lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
// //����ֵ:0,���óɹ�
// //    ����,����ʧ�� 
// uint8_t MPU_Set_LPF(uint16_t lpf)
// {
// 	uint8_t data=0;
// 	if(lpf>=188)data=1;
// 	else if(lpf>=98)data=2;
// 	else if(lpf>=42)data=3;
// 	else if(lpf>=20)data=4;
// 	else if(lpf>=10)data=5;
// 	else data=6; 
// 	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
// }
// //����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
// //rate:4~1000(Hz)
// //����ֵ:0,���óɹ�
// //    ����,����ʧ�� 
// uint8_t MPU_Set_Rate(uint16_t rate)
// {
// 	uint8_t data;
// 	if(rate>1000)rate=1000;
// 	if(rate<4)rate=4;
// 	data=1000/rate-1;
// 	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
//  	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
// }

// //�õ��¶�ֵ
// //����ֵ:�¶�ֵ(������100��)
// short MPU_Get_Temperature(void)
// {
//     uint8_t buf[2]; 
//     short raw;
// 	float temp;
// 	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
//     raw=((uint16_t)buf[0]<<8)|buf[1];  
//     temp=36.53+((double)raw)/340;  
//     return temp*100;;
// }
// //�õ�������ֵ(ԭʼֵ)
// //gx,gy,gz:������x,y,z���ԭʼ����(������)
// //����ֵ:0,�ɹ�
// //    ����,�������
// uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
// {
//     uint8_t buf[6],res;  
// 	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
// 	if(res==0)
// 	{
// 		*gx=((uint16_t)buf[0]<<8)|buf[1];  
// 		*gy=((uint16_t)buf[2]<<8)|buf[3];  
// 		*gz=((uint16_t)buf[4]<<8)|buf[5];
// 	} 	
//     return res;;
// }
// //�õ����ٶ�ֵ(ԭʼֵ)
// //gx,gy,gz:������x,y,z���ԭʼ����(������)
// //����ֵ:0,�ɹ�
// //    ����,�������
// uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
// {
//     uint8_t buf[6],res;  
// 	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
// 	if(res==0)
// 	{
// 		*ax=((uint16_t)buf[0]<<8)|buf[1];  
// 		*ay=((uint16_t)buf[2]<<8)|buf[3];  
// 		*az=((uint16_t)buf[4]<<8)|buf[5];
// 	} 	
//     return res;;
// }

/**
 * @brief   �õ�������ֵ(ԭʼֵ)
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   gx    x��Ƕ�
 * @param   gy    y��Ƕ�
 * @param   gz    z��Ƕ�
 *
 */
uint8_t MPU_Get_Gyroscope(struct MPU_Device *p_MPUDev, short *gx,short *gy,short *gz)
{
	uint8_t buf[6],data;
	p_MPUDev->MPU_Read(p_MPUDev,MPU_ADDR,MPU_GYRO_XOUTH_REG,6,*buf);

}

/**
 * @brief   �õ����ٶ�ֵ(ԭʼֵ)
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   ax    x����ٶ�
 * @param   ay    y����ٶ�
 * @param   az    z����ٶ�
 *
 */
uint8_t MPU_Get_Accelerometer(struct MPU_Device *p_MPUDev, short *ax,short *ay,short *az)
{

}

/**
 * @brief   IICд��һ���ֽ�
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   SlaveAddress    ���豸��ַ
 * @param   RegAddress      �Ĵ�����ַ
 * @param   ByteData        д�������
 *
 */
void mpu_WriteByte(struct MPU_Device *p_MPUDev, uint16_t WriteAddr, uint8_t DataToWrite)
{
	p_MPUDev->IIC_Device->IIC_WriteRegByte(p_MPUDev->IIC_Device, MPU_ADDR, WriteAddr);
}

/**
 * @brief   IIC��ȡһ���ֽ�
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   SlaveAddress    ���豸��ַ
 * @param   RegAddress      �Ĵ�����ַ
 * @param   ByteData        ��ȡ������
 *
 */
uint8_t mpu_ReadByte(struct MPU_Device *p_MPUDev, uint16_t ReadAddr)
{
	return p_MPUDev->IIC_Device->IIC_ReadRegByte(p_MPUDev->IIC_Device, MPU_ADDR, ReadAddr);
}

/**
 * @brief   IIC����д
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   SlaveAddress    ���豸��ַ
 * @param   RegAddress      �Ĵ�����ַ
 * @param   len            д�볤��
 * @param   pBuffer        ������
 *
 */
void mpu_Write(struct MPU_Device *p_MPUDev, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t len)
{
	p_MPUDev->IIC_Device->IIC_WriteReg(p_MPUDev->IIC_Device, MPU_ADDR, WriteAddr, len, pBuffer);
}

/**
 * @brief   IIC������
 *
 * @param   p_MPUDev    IIC�豸�ṹ��
 * @param   SlaveAddress    ���豸��ַ
 * @param   RegAddress      �Ĵ�����ַ
 * @param   len            Ҫ��ȡ�ĳ���
 * @param   pBuffer        ��ȡ�������ݴ洢��
 *
 */
void mpu_Read(struct MPU_Device *p_MPUDev, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t len)
{
	p_MPUDev->IIC_Device->IIC_ReadReg(p_MPUDev->IIC_Device, MPU_ADDR, ReadAddr, pBuffer, len);
}
