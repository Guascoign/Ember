## SH36730X 通用固件库，可实现 SH36730X 的全部功能。支持SH367303 SH367305 SH367306

#### 1. 库将提供以下实用函数：  
* SH36730X 硬件配置初始化函数  
* SH36730X 读取中断状态函数  
* SH36730X 读取内置ADC转换结果函数  
* SH36730X 设置参数值函数  
* SH36730X 获取参数设置值函数  
* SH36730X 设置硬件配置函数  
* SH36730X 获取硬件配置状态函数

#### 2.要使用此库，需要提供以下外部函数：  
```c
void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData); //通过I2C向设备寄存器写一个字节  
uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress); //通过I2C从设备寄存器读一个字节  
void SoftwareDelay(uint8_t ms); //软件延时毫秒  
```

#### 3，注意事项：  
* 
