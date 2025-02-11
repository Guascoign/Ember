#include "serial.h"
#include <QDebug>
//构造函数
SerialPortHandler::SerialPortHandler()
{
    serialPort = new QSerialPort();

}
//析构函数
SerialPortHandler::~SerialPortHandler()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
}
//刷新串口
void SerialPortHandler::refreshSerialPort()
{
    SerialPortList = QSerialPortInfo::availablePorts();
}



// 打开串口
bool SerialPortHandler::openSerialPort(const SerialPortConfig &config) {
    // 设置串口号
    serialPort->setPortName(config.portName);
    // 设置波特率
    serialPort->setBaudRate(config.baudRate);
    // 设置数据位
    serialPort->setDataBits(config.dataBits);
    // 设置校验位
    serialPort->setParity(config.parity);
    // 设置停止位
    serialPort->setStopBits(config.stopBits);
    // 设置流控制
    serialPort->setFlowControl(config.flowControl);

    // 打开串口
    return serialPort->open(QIODevice::ReadWrite);
}

// 关闭串口
void SerialPortHandler::closeSerialPort() {
        serialPort->close();
}
