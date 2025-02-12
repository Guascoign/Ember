#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QValueAxis>
#include <QThread>
#include "serialport.h"

QT_CHARTS_USE_NAMESPACE

#define MAX_X   50
#define MAX_Y   70
#define TRUE    1
#define FALSE   0
class ComSerialPort;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
     void SendData(QByteArray data);
private slots:
    void on_OpenUart_clicked();
    void on_CloseUart_clicked();
    void ClearRecvButton();
    void TimeSendReady(int state);
    void SendDataHex(int state);
    void SaveRecvDataFile();
    void find_port();
    void RcvData(QByteArray RecvBuff);
    void on_SendDataFile_clicked();
    void on_RefreshUart_clicked();

private:
    Ui::MainWindow *ui;
    ComSerialPort *Serial;

    QTimer *timersend;
    QString     SendTextStr;
    QByteArray  SendTextByte;
    quint32     dataRxNumber = 0 ;      // 记录Rx数据量
    quint8      Serial_State = 0 ;      // 串口状态

};

#endif // MAINWINDOW_H
