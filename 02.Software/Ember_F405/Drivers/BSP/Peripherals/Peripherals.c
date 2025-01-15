/********************************************************************************
    * 文件名称 ：Peripherals.c
    * 作     者：@ZM
    * 版     本：V1.0
    * 编写日期 ：2025-01-08
    * 功     能：外设注册表
*********************************************************************************
V1.0 2025-01-08 First version
*********************************************************************************/

#include "BSP/Peripherals/Peripherals.h"

PeripheralDeviceTypeDef **peripheral_devices = NULL;
int peripheral_device_count = 0;

UART_DeviceTypeDef Test_Uart;   //测试串口
LED_DeviceTypeDef RUNLED;
RGB_DeviceTypeDef WS2812B;
KEY_DeviceTypeDef key1;
KEY_DeviceTypeDef key2;


void All_Peripherals_Init(void) {
/*注册测试串口*****************/
  UART_Init(
    &Test_Uart,
    "Test_Uart1",
    USART1,
    UART_MODE_NORMAL,
    Rate_115200,
    Word_length_8B,
    PARITY_NONE,
    Stop_bits_1);

    LED_Init(&RUNLED , "RUNLED",(void *)RUNLED_GPIO_Port,RUNLED_Pin);
    RGB_Init(&WS2812B,"WS2812B",3);
    Key_Init(&key1,"KEY1",(void *)KEY1_GPIO_Port,KEY1_Pin);
    Key_Init(&key2,"KEY2",(void *)KEY2_GPIO_Port,KEY2_Pin);
    
// 打印初始化成功的所有外设的名称
//Print_AllDevice_Info();

}

/**
 * @brief   添加设备到外设列表
 * @param   name    设备名称
 * param   handle  设备句柄
 */
int Add_Device(char *name, void *handle, DeviceTypeDef type) {
    PeripheralDeviceTypeDef *new_device = (PeripheralDeviceTypeDef *)malloc(sizeof(PeripheralDeviceTypeDef));
    if (new_device == NULL) {
        return -1; // 内存分配失败
    }
    new_device->name = name;
    new_device->handle = handle;
    new_device->type = type;
    PeripheralDeviceTypeDef **new_array = (PeripheralDeviceTypeDef **)realloc(peripheral_devices, (peripheral_device_count + 1) * sizeof(PeripheralDeviceTypeDef *));
    if (new_array == NULL) {
        free(new_device);
        return -1; // 内存分配失败
    }
    peripheral_devices = new_array;
    peripheral_devices[peripheral_device_count] = new_device;
    peripheral_device_count++;
    return 0; // 成功
}

/**
 * @brief   打印所有已注册的外设
 * @return  void
 */
void Print_Devices(void) {
    printf("Initialized Peripheral Devices:\n");
    for (int i = 0; i < peripheral_device_count; i++) {
        printf("Device %d: %s\n", i + 1, peripheral_devices[i]->name);
    }
}

/**
 * @brief   根据设备名称获取对应的设备指针
 * @param   name    设备名称
 * @return  返回对应设备名称的设备指针，如果找不到则返回NULL
 */
void *Get_Device(char *name) {
    for (int i = 0; i < peripheral_device_count; i++) {
        if (strcmp(peripheral_devices[i]->name, name) == 0) {
            return peripheral_devices[i]->handle;
        }
    }
    return (DeviceTypeDef)0; // 未找到设备
}

/**
 * @brief   卸载设备，删除设备名称对应数组的设备指针（未删除设备指针，待添加）
 * @param   name    设备名称
 * @return  void
 */
void Free_Devices(void *name) {
    for (int i = 0; i < peripheral_device_count; i++) {
        if (strcmp(peripheral_devices[i]->name, name) == 0) {
            free(peripheral_devices[i]);
            for (int j = i; j < peripheral_device_count - 1; j++) {
                peripheral_devices[j] = peripheral_devices[j + 1];
            }
            peripheral_device_count--;
            return;
        }
    }
}

/**
 * @brief   根据设备名称获取对应的设备句柄
 * @param   name    设备名称
 * @return  返回对应设备名称的设备句柄，如果找不到则返回NULL
 */
void *Get_Device_Handle(char *name) {
    for (int i = 0; i < peripheral_device_count; i++) {
        if (strcmp(peripheral_devices[i]->name, name) == 0) {
            return peripheral_devices[i]->handle;
        }
    }
    return NULL; // 未找到设备
}

/**
 * @brief   根据设备名称获取对应的设备类型
 * @param   name    设备名称
 * @return  返回对应设备名称的设备类型，如果找不到则返回NULL
 */
DeviceTypeDef Get_Device_Type(char *name) {
    for (int i = 0; i < peripheral_device_count; i++) {
        if (strcmp(peripheral_devices[i]->name, name) == 0) {
            return peripheral_devices[i]->type;
        }
    }
    return Empty_DEVICE; // 未找到设备
}

/**
 * @brief   打印设备详细信息
 * @param   name    设备名称
 */
void Print_Device_Info(char *name) {
    void *handle = Get_Device_Handle(name);
    DeviceTypeDef type = Get_Device_Type(name);
    if (handle != NULL && type != Empty_DEVICE) {
        printf("%s\t\t%p\t%d\n", name, handle, (int)type);
    } else {
        printf("Device %s not found.\n", name);
    }
}

/**
 * @brief   打印所有设备详细信息
 * @return  void
 */
void Print_AllDevice_Info(void) {
    printf("Device Name\t\tHandle\t\tType\n");
    for (int i = 0; i < peripheral_device_count; i++) {
        Print_Device_Info(peripheral_devices[i]->name);
    }
}
