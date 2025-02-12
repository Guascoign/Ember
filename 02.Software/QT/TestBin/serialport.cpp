#include "serialport.h"
#include <QSerialPortInfo>
#include <QDebug>


ComSerialPort::ComSerialPort(QString PortName,qint32 BaudRate,qint32* state, QObject *parent) : QObject(parent)
{

    thread = new QThread();
    Serial = new QSerialPort();

    *state = InitSerial(PortName,BaudRate);

    this->moveToThread(thread);
    Serial->moveToThread(thread);
    thread->start();

    connect(thread, &QThread::finished, this,&QObject::deleteLater);

}

ComSerialPort::~ComSerialPort()
{

}


bool ComSerialPort::InitSerial(QString portname,qint32 BaudRate)
{
    Serial->setPortName(portname);
    if(!Serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"串口打开失败";
        return 0;
    }
    qDebug()<<"串口打开成功";
    Serial->setBaudRate(BaudRate);   // 默认115200
    Serial->setDataBits(QSerialPort::Data8);        // 默认
    Serial->setParity(QSerialPort::NoParity);
    Serial->setStopBits(QSerialPort::OneStop);

    // 读取数据
    connect(Serial,&QSerialPort::readyRead,this,&ComSerialPort::RcvData);

    return 1;
}

void ComSerialPort::CloseSerial()
{

    Serial->clear();
    Serial->close();

}

void ComSerialPort::RcvData()
{
    QByteArray buffer = Serial->readAll();
    qDebug() << "接收数据线程ID："<< QThread::currentThreadId();
    emit  UpdateData(buffer);
}

void ComSerialPort::SendSerialData(QByteArray data)
{
    qDebug() << "发送数据线程ID："<< QThread::currentThreadId();
    // 接收GUI数据并发送
    Serial->write(data);
    Serial->flush();
    Serial->waitForBytesWritten(10);
}

