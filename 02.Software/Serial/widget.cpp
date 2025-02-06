#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>

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
    ui->boundrate_comboBox->setCurrentIndex(6);// 115200
    ui->databits_comboBox->setCurrentIndex(3);// 8
    ui->parity_comboBox->setCurrentIndex(0);// None
    ui->stopbits_comboBox->setCurrentIndex(0);// 1
    ui->flowcontrol_comboBox->setCurrentIndex(0);// None

    // 调用搜索串口方法
    searchSerialPorts();
}

Widget::~Widget()
{
    delete ui;
}

//发送数据
void Widget::on_SEND_pushButton_clicked(){
    QString data = ui->send_textEdit->toPlainText(); // 获取文本框中的文本
    SerialPort->write(data.toUtf8()); // 发送数据
    qDebug() << "Send Data:" << data << Qt::endl;
}



//打开-关闭串口
void Widget::on_Opem_COM_pushButton_clicked()
{
    if (SerialPort->isOpen()) {
        // 获取当前串口信息
        QString selectedPort = SerialPort->portName();
        QString description = ui->Serial_number_comboBox->currentText().split(":").last();
        
        // 关闭串口
        SerialPort->close();
        qDebug() << "Close Serial Port Success!" << Qt::endl;
        ui->info_label->setText(QString("Close %1:%2").arg(selectedPort, description));
        ui->Opem_COM_pushButton->setText("打开串口");

        // 进度条动画
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            int value = ui->progressBar->value();
            if (value > 0) {
                ui->progressBar->setValue(value - 1);
            } else {
                timer->stop();
                timer->deleteLater();
            }
        });
        timer->start(1); // 每1毫秒更新一次
    } else {
        // 获取选择的串口信息
        QString selectedPort = ui->Serial_number_comboBox->currentText().split(":").first();
        // 选择串口
        SerialPort->setPortName(selectedPort);
        // 配置波特率
        SerialPort->setBaudRate(ui->boundrate_comboBox->currentText().toInt());
        // 配置数据位
        SerialPort->setDataBits(static_cast<QSerialPort::DataBits>(ui->databits_comboBox->currentText().toInt()));
        // 配置校验位
        switch (ui->parity_comboBox->currentIndex()) {
            case 0:
                SerialPort->setParity(QSerialPort::NoParity);
                break;
            case 1:
                SerialPort->setParity(QSerialPort::EvenParity);
                break;
            case 2:
                SerialPort->setParity(QSerialPort::OddParity);
                break;
            case 3:
                SerialPort->setParity(QSerialPort::SpaceParity);
                break;
            case 4:
                SerialPort->setParity(QSerialPort::MarkParity);
                break;
            default:
                break;
        }
        // 配置停止位
        SerialPort->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopbits_comboBox->currentText().toInt()));
        // 配置流控制
        if(ui->flowcontrol_comboBox->currentText() == "None"){
            SerialPort->setFlowControl(QSerialPort::NoFlowControl);
        }
        else if(ui->flowcontrol_comboBox->currentText() == "RTS/CTS"){
            SerialPort->setFlowControl(QSerialPort::HardwareControl);
        }
        else if(ui->flowcontrol_comboBox->currentText() == "XON/XOFF"){
            SerialPort->setFlowControl(QSerialPort::SoftwareControl);
        }
        // 打开串口
        if(SerialPort->open(QIODevice::ReadWrite)){
            qDebug() << "Open Serial Port Success!" << Qt::endl;
            QString description = ui->Serial_number_comboBox->currentText().split(":").last();
            ui->info_label->setText(QString("Open %1:%2 Success!").arg(selectedPort, description));
            ui->Opem_COM_pushButton->setText("关闭串口");
            
            // 进度条动画
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, [=]() {
                int value = ui->progressBar->value();
                if (value < 100) {
                    ui->progressBar->setValue(value + 1);
                } else {
                    timer->stop();
                    timer->deleteLater();
                }
            });
            timer->start(1); // 每1毫秒更新一次
        }
        else{
            qDebug() << "Open Serial Port Failed!" << Qt::endl;
            QString description = ui->Serial_number_comboBox->currentText().split(":").last();
            ui->info_label->setText(QString("Open %1:%2 Failed!").arg(selectedPort, description));
            ui->progressBar->setValue(1); // 失败时卡在50%
        }
    }
}


// 扩展开关
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
//串口搜索
void Widget::searchSerialPorts()
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


