#include "MPU/MPU6050.h"
#include "math.h"

//I2C_BUS MPU6050_I2C;

typedef enum{
    Band_256Hz = 0x00,
    Band_186Hz,
    Band_96Hz,
    Band_43Hz,
    Band_21Hz,
    Band_10Hz,
    Band_5Hz
}Filter_Typedef;

typedef enum{
    gyro_250 = 0x00,
    gyro_500 = 0x08,
    gyro_1000 = 0x10,
    gyro_2000 = 0x18
}GYRO_CONFIG_Typedef;

typedef enum{
    acc_2g = 0x00,
    acc_4g = 0x08,
    acc_8g = 0x10,
    acc_16g = 0x18
}ACCEL_CONFIG_Typedef;

typedef enum{
    FIFO_Disable,
    Acc_OUT = 0x08,
    Gyro_zOUT = 0x10,
    Gyro_yOUT = 0x20,
    Gyro_xOUT = 0x40,
    Temp_OUT =0x80,
}FIFO_EN_Typedef;

typedef enum{
    interrupt_Disable,
    Data_Ready_EN = 0x01,
    I2C_Master_EN = 0x08,
    FIFO_overFolow_EN = 0x10,
    Motion_EN = 0x40,
}INT_EN_Typedef;
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct MPU6050_InitTypeDef
{
    uint16_t SMPLRT_Rate;
    Filter_Typedef Filter;
    GYRO_CONFIG_Typedef gyro_range;
    ACCEL_CONFIG_Typedef acc_range;
    FIFO_EN_Typedef FIFO_EN;
    INT_EN_Typedef INT;
}MPU6050_InitTypeDef;
///////////////////////////////////////////////////////////////////////////////////////////////////
// static void MPU6050_Register_init(struct MPU_Device *p_MPUDev){


//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x80);//复位
    
//     Delay_ms(100);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x00);//唤醒
//    	uint8_t SMPLRT_DIV;
// 	if(this->SMPLRT_Rate>1000)this->SMPLRT_Rate=1000;
// 	else if(this->SMPLRT_Rate<4)this->SMPLRT_Rate=4;
// 	SMPLRT_DIV=1000/this->SMPLRT_Rate-1;//由计算公式得
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_SMPLRT_DIV,SMPLRT_DIV);

//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_INT_ENABLE,this->INT);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_CONFIG,this->Filter);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_GYRO_CONFIG,this->gyro_range);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_ACCEL_CONFIG,this->acc_range);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_FIFO_EN,this->FIFO_EN);
//     uint8_t temp = 0x00;
//     if(this->FIFO_EN!=0x00)//如果打开了FIFO
//         temp = 0x40;
//     if((this->INT & 0x08)==1)//如果打开了中断
//         temp |= 0x08;
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_USER_CTRL,temp);
//     I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x01);//X轴为参考
// }

// void MPU6050_init(struct MPU_Device *p_MPUDev , struct IIC_Bus *p_IICDev){
//     MPU6050_I2C = Create_SI2C(GPIOx,SCl,SDA,MPU6050_ADDRESS<<1);
    
//     MPU6050_InitTypeDef MPU6050_init_Struct;
//     MPU6050_init_Struct.SMPLRT_Rate = 500;            //采样率Hz
//     MPU6050_init_Struct.Filter = Band_5Hz;            //滤波器带宽
//     MPU6050_init_Struct.gyro_range = gyro_2000;       //陀螺仪测量范围
//     MPU6050_init_Struct.acc_range = acc_16g;          //加速度计测量范围
//     MPU6050_init_Struct.FIFO_EN = FIFO_Disable;       //FIFO
//     MPU6050_init_Struct.INT = interrupt_Disable;      //中断配置
    
//     MPU6050_Register_init(&MPU6050_init_Struct);      //初始化寄存器
    
// }

// void MPU6050_Get_Raw(struct MPU_Device *p_MPUDev){
//     this->AccX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_L);
//     this->AccY = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_L);
//     this->AccZ = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_L);
//     this->GyroX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_L);
//     this->GyroX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_L);
//     this->GyroX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_L);
//     this->Temp = ((uint16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_L);
// }

// float MPU6050_GetTemp(struct MPU_Device *p_MPUDev){
// 	uint8_t temp = (((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_H)) << 8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_L);
// 	float temperature = (float)temp/340 + 36.53;
// 	return temperature;
// }

// void MPU6050_Get_Angle(struct MPU_Device *p_MPUDev){
// 	int16_t temp=0;
// 	float Ax,Ay,Az=0;
// 	float Gx,Gy,Gz=0;
// 	static float Gyroscope_roll=0;
// 	static float Gyroscope_pitch=0;
//     const static float dt=0.005;
//     const static float weight = 0.95;//权重

//     //static int roll_befor = 0;
//     //static int pitch_befor = 0;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_L);
// 	Ax = temp * 16.0/32768;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_L);
// 	Ay = temp * 16.0/32768;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_L);
// 	Az = temp * 16.0/32768;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_L);
// 	Gx = temp *dt*0.0174533;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_L);
// 	Gy = temp *dt*0.0174533;

// 	temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_L);
// 	Gz = temp *dt*0.0174533;

// 	Gyroscope_roll+=Gy;
// 	Gyroscope_pitch+=Gx;
// 	this->roll=weight * atan2(Ay,Az)/3.1415926 * 180 + (1-weight) * Gyroscope_roll;
// 	this->pitch=-(weight * atan2(Ax,Az)/3.1415926 * 180 + (1-weight) * Gyroscope_pitch);
// 	this->yaw += Gz*20+0.0157;//减小零飘

//     //this->roll = (this->roll+roll_befor)/2;
//     //this->pitch = (this->pitch+pitch_befor)/2;
//     //roll_befor = this->roll;
//     //pitch_befor = this->pitch;
// }

uint8_t MPU6050_ID(struct MPU_Device *p_MPUDev){

    //data = p_MPUDev->IIC_Bus->IIC_ReadRegByte( p_MPUDev->IIC_Bus , MPU6050_ADDRESS , MPU6050_WHO_AM_I);


    uint8_t temp=0;                          
    p_MPUDev->IIC_Bus->IIC_Start(p_MPUDev->IIC_Bus);   //发送起始信号
    p_MPUDev->IIC_Bus->IIC_SendByte(p_MPUDev->IIC_Bus, MPU6050_ADDRESS | 0);    //发送地址 + 写命令
    p_MPUDev->IIC_Bus->IIC_WaitAck(p_MPUDev->IIC_Bus);
    p_MPUDev->IIC_Bus->IIC_SendByte(p_MPUDev->IIC_Bus, MPU6050_WHO_AM_I);   //发送低地址
    p_MPUDev->IIC_Bus->IIC_WaitAck(p_MPUDev->IIC_Bus);
    p_MPUDev->IIC_Bus->IIC_Restart(p_MPUDev->IIC_Bus);   //发送起始信号
    p_MPUDev->IIC_Bus->IIC_SendByte(p_MPUDev->IIC_Bus, MPU6050_ADDRESS | 1);    //发送地址 + 读命令
    p_MPUDev->IIC_Bus->IIC_WaitAck(p_MPUDev->IIC_Bus);
    temp = p_MPUDev->IIC_Bus->IIC_ReceiveByte(p_MPUDev->IIC_Bus,0);//读取一个字节
    p_MPUDev->IIC_Bus->IIC_Nack(p_MPUDev->IIC_Bus);//发送nACK
    p_MPUDev->IIC_Bus->IIC_Stop(p_MPUDev->IIC_Bus);//产生一个停止条件
	return temp;

    
}

//实例化MPU对象
struct MPU_Device MPU6050 = {
    .name = "MPU6050",
    .MPU_ID = MPU6050_ID,
    .priv_data = NULL
};


/**
  *@brief   AT24设备数组
 */
struct MPU_Device *MPU_Devices[] = {
    &MPU6050  //MPU6050
};

/**
  *@brief   根据设备名称获取对应的EEPROM设备指针
 *
  *@param   name    设备名称
  *@return  返回对应设备名称的EEPROM设备指针，如果找不到则返回NULL
 */
struct MPU_Device *MPU_GetDevice(char *name)
{
    int i = 0;
        for (i = 0; i < sizeof(MPU_Devices)/sizeof(MPU_Devices[0]); i++)
    {
        if (0 == strcmp(name, MPU_Devices[i]->name))
            return MPU_Devices[i];
    }
    return NULL;
}
