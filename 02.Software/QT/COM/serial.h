#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

struct SerialPortConfig {
    QString portName;   //串口号
    qint32 baudRate;    //波特率
    QSerialPort::DataBits dataBits; //数据位
    QSerialPort::Parity parity;    //校验位
    QSerialPort::StopBits stopBits; //停止位
    QSerialPort::FlowControl flowControl;   //流控制
};

class SerialPortHandler : public QObject {
    Q_OBJECT
public:
    SerialPortHandler();//构造函数
    ~SerialPortHandler();//析构函数
    QList<QSerialPortInfo> SerialPortList;
    QSerialPort *serialPort;//串口
    void refreshSerialPort();//刷新串口
    bool openSerialPort(const SerialPortConfig &config);//打开串口
    void closeSerialPort();//关闭串口
    
private:


};


#endif // SERIAL_H