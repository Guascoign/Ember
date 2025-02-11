#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serial.h"
#include <QDateTime>//时间
#include <QTimer>//定时器
#include <QElapsedTimer>//计时器
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#define TIMEOUT1S 1000
#define TIMEOUT2S 2000

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void timerEvent(QTimerEvent *event);//定时器事件

private slots:
    void on_Addons_checkBox_clicked(bool checked);//打开/关闭插件信号槽
    void on_AUTOSEND_pushButton_clicked();//自动发送信号槽
    void on_SEND_pushButton_clicked();//发送信号槽
    void on_Clear_recvbuf_pushButton_clicked();//清空接收区信号槽
    void on_Clear_sendbuf_pushButton_clicked();//清空发送区信号槽
    void on_Save_pushButton_clicked();//保存数据信号槽
    void on_Opem_COM_pushButton_clicked();//打开串口信号槽
    void on_Serial_number_comboBox_clicked();//串口号信号槽
    void handleSerialError(QSerialPort::SerialPortError error);//串口错误处理

private:
    Ui::Widget *ui;
    void Update_RTC();//更新RTC
    void Update_SerialPort();//更新串口到ComboBox（获取串口号 判断断联）
    SerialPortHandler *serialPortHandler;//串口
    QTimer *timeUpdateTimer;//时间更新定时器
    QTimer *autoSendTimer;//自动发送定时器
    QTimer *autoReconnectTimer;//自动重连定时器
    QTimer *SerialPort_Refreash;//自动重连定时器
    void animateProgressBar(int startValue, int endValue, int duration); // 进度条动画
    void System_Init();//系统初始化
    void updateSizeLabel();//更新数据量
    int autoReline_Flag;//自动换行标志
    int totalTextSize;//总数据量
    int autoSend_Flag;//自动发送标志
    int autoSend_time;//自动发送时间
};
#endif // WIDGET_H
