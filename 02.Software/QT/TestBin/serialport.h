#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <QThread>
#include <QObject>
#include <QSerialPort>


class ComSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit ComSerialPort(QString PortName,qint32 BaudRate,qint32* state,QObject *parent = nullptr);
    ~ComSerialPort();
    bool InitSerial(QString portname,qint32 BaudRate);
    void CloseSerial();

signals:
    void UpdateData(QByteArray data);

public slots:
    void RcvData();
    void SendSerialData(QByteArray data);

private:
    QSerialPort *Serial;
    QThread* thread;
};

#endif // COMSERVICE_H

#endif // SERIALPORT_H
