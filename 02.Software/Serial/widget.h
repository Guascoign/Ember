#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
private:
    Ui::Widget *ui;
    QSerialPort *SerialPort; // 新增成员变量
    void searchSerialPorts(); // 新增方法声明
};

#endif // WIDGET_H
