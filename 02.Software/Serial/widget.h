#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer> // 新增头文件

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
    void on_Opem_COM_pushButton_clicked();
    void on_SEND_pushButton_clicked();
    void on_Clear_sendbuf_pushButton_clicked();
    void on_Clear_recvbuf_pushButton_clicked();
    void on_Serialdata_readytoread(); 
    void on_auto_reline_pushButton_clicked();
    void updateTimeLabel(); // 新增方法声明
    // 删除自动发送相关的槽函数声明
private:
    Ui::Widget *ui;
    QSerialPort *SerialPort; // 新增成员变量
    bool autoRelineEnabled; // 新增成员变量
    QTimer *timeUpdateTimer; // 新增成员变量
    // 删除自动发送相关的成员变量
    void searchSerialPorts(); // 新增方法声明
    void animateProgressBar(int startValue, int endValue, int duration); // 新增方法声明
};

#endif // WIDGET_H
