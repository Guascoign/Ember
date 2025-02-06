#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , SerialPort(new QSerialPort(this)) // 修复初始化
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayoutGlobal);
    // 关闭扩展
    ui->Addons_groupBox->hide();
    // 设置默认值
    ui->boundrate_comboBox->setCurrentIndex(6);
    ui->databits_comboBox->setCurrentIndex(3);
    ui->parity_comboBox->setCurrentIndex(0);
    ui->stopbits_comboBox->setCurrentIndex(0);
    ui->flowcontrol_comboBox->setCurrentIndex(0);

    // 调用搜索串口方法
    searchSerialPorts();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_Addons_checkBox_clicked(bool checked)
{
    if(checked){
        ui->Addons_groupBox->show();
        qDebug() << checked << Qt::endl;
    }
    else{
        ui->Addons_groupBox->hide();
        qDebug() << checked << Qt::endl;
    }
}

void Widget::searchSerialPorts() // 新增方法定义
{
    // 获取所有可用的串口信息
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &serialInfo : serialList) {
        // 串口名称
        QString portName = serialInfo.portName();
        // 获取串口描述信息（如果有）
        QString description = serialInfo.description();
        // 如果串口描述信息为空，可以使用制造商信息或其他信息
        if (description.isEmpty()) {
            description = serialInfo.manufacturer();
        }
        // 如果描述也为空，就使用串口名称来代替
        if (description.isEmpty()) {
            description = portName;
        }
        // 添加到 QComboBox，显示端口名称和描述信息
        QString displayText = QString("%1:%2").arg(portName, description);
        ui->Serial_number_comboBox->addItem(displayText);
        // 打印调试信息
        qDebug() << displayText;
    }
}


