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
    void on_Save_pushButton_clicked();
    void updateTimeLabel();
private:
    Ui::Widget *ui;
    QSerialPort *SerialPort;
    bool autoRelineEnabled;
    QTimer *timeUpdateTimer;
    qint64 totalTextSize; 
    void searchSerialPorts(); 
    void animateProgressBar(int startValue, int endValue, int duration); 
    void updateSizeLabel();
};

#endif // WIDGET_H
