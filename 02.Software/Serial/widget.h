#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QSerialPort>
#include "serial.h"
#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QSerialPort; // 前向声明

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_Addons_checkBox_clicked(bool checked);
    void on_Auto_reconnect_checkBox_clicked(bool checked);
    void on_Opem_COM_pushButton_clicked();
    void on_SEND_pushButton_clicked();
    void on_Clear_sendbuf_pushButton_clicked();
    void on_Clear_recvbuf_pushButton_clicked();
    void on_Serialdata_readytoread(); 
    void on_auto_reline_pushButton_clicked();
    void on_Save_pushButton_clicked();
    void updateTimeLabel();
    void updateSerialPorts();
    void attemptReconnect();
    void handleSerialError(QSerialPort::SerialPortError serialPortErr);
private:
    Ui::Widget *ui;
    QSerialPort *SerialPort;
    bool autoRelineEnabled;
    QTimer *timeUpdateTimer;
    QTimer *serialPortUpdateTimer;
    QTimer *autoReconnectTimer;
    qint64 totalTextSize; 
    QString currentPortName;
    QString currentPortDescription;
    bool disconnectFlag;
    void searchSerialPorts(); 
    void animateProgressBar(int startValue, int endValue, int duration); 
    void updateSizeLabel();
};

#endif // WIDGET_H
