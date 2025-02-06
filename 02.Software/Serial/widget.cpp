#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <QRegularExpression> // 新增头文件
#include <QDateTime> // 新增头文件
#include <QFileDialog> // 新增头文件
#include <QFile> // 新增头文件
#include <QTextStream> // 新增头文件
#include <QMessageBox> // 新增头文件

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , SerialPort(new QSerialPort(this))
    , autoRelineEnabled(false) // 初始化成员变量
    , timeUpdateTimer(new QTimer(this)) // 初始化成员变量
    , totalTextSize(0) // 初始化成员变量
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayoutGlobal);
    connect(SerialPort, &QSerialPort::readyRead, this, &Widget::on_Serialdata_readytoread);
    // 关闭扩展
    ui->Addons_groupBox->hide();
    // 设置默认值
    ui->boundrate_comboBox->setCurrentIndex(6);// 115200
    ui->databits_comboBox->setCurrentIndex(3);// 8
    ui->parity_comboBox->setCurrentIndex(0);// None
    ui->stopbits_comboBox->setCurrentIndex(0);// 1
    ui->flowcontrol_comboBox->setCurrentIndex(0);// None
    ui->AUTOSEND_time_spinBox->setValue(1000); // 初始化自动发送时间为1000ms
    // 更新时间到 time_label
    updateTimeLabel();
    // 调用搜索串口方法
    searchSerialPorts();

    // 设置定时器每秒更新一次时间
    connect(timeUpdateTimer, &QTimer::timeout, this, &Widget::updateTimeLabel);
    timeUpdateTimer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

//发送数据
void Widget::on_SEND_pushButton_clicked(){
    if (SerialPort->isOpen()) {
        QString data = ui->send_textEdit->toPlainText(); // 获取文本框中的文本
        if (data.isEmpty()) {
            ui->info_label->setText("Error: 发送框为空");
            qDebug() << "Error: 发送框为空" << Qt::endl;
            return;
        }
        SerialPort->write(data.toUtf8()); // 发送数据
        qDebug() << "Send Data:" << data << Qt::endl;
        
        if (autoRelineEnabled) {
            ui->recv_textEdit->append("<- " + data);
        } else {
            ui->recv_textEdit->append(data);
        }
        // 更新发送的数据量到 Tx_label
        static int totalTxBytes = 0;
        int txBytes = data.toUtf8().size();
        totalTxBytes += txBytes;
        ui->Tx_label->setText(QString("Tx: %1 bytes").arg(totalTxBytes));
        
        // 累加到 size_label
        totalTextSize += txBytes;
        updateSizeLabel();

    } else {
        ui->info_label->setText("Error: 串口未打开");
        qDebug() << "Error: 串口未打开" << Qt::endl;
    }
}

//清空发送数据
void Widget::on_Clear_sendbuf_pushButton_clicked(){
    ui->send_textEdit->clear();
}

//清空接收数据
void Widget::on_Clear_recvbuf_pushButton_clicked(){
    ui->recv_textEdit->clear();
}

//进度条动画
void Widget::animateProgressBar(int startValue, int endValue, int duration) {
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < duration) {
        int value = startValue + (endValue - startValue) * timer.elapsed() / duration;
        ui->progressBar->setValue(value);
        QCoreApplication::processEvents();
    }
    ui->progressBar->setValue(endValue);
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
        animateProgressBar(100, 0, 100); // 关闭串口时进度条从100到0，持续1秒
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
            animateProgressBar(0, 100, 100); 
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

void Widget::on_Serialdata_readytoread()
{
    QString recvData = SerialPort->readAll();
    if (ui->auto_reline_pushButton->text() == "自动换行") {
        recvData.remove(QRegularExpression("\\n"));
        ui->recv_textEdit->moveCursor(QTextCursor::End);
        ui->recv_textEdit->insertPlainText(recvData);
    } else {
        ui->recv_textEdit->append("-> " + recvData);
    }
    // 更新收到的数据量到 Rx_label
    static int totalRxBytes = 0;
    int rxBytes = recvData.toUtf8().size();
    totalRxBytes += rxBytes;
    ui->Rx_label->setText(QString("Rx: %1 bytes").arg(totalRxBytes));

    // 累加到 size_label
    totalTextSize += rxBytes;
    updateSizeLabel();
}

//自动换行
void Widget::on_auto_reline_pushButton_clicked(){
    autoRelineEnabled = !autoRelineEnabled;
    if(autoRelineEnabled){
        ui->recv_textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        ui->auto_reline_pushButton->setText("取消自动换行");
    }
    else{
        ui->recv_textEdit->setWordWrapMode(QTextOption::NoWrap);
        ui->auto_reline_pushButton->setText("自动换行");
    }
    QString text = ui->recv_textEdit->toPlainText();
    if (autoRelineEnabled) {
        if (!text.endsWith("\n")) {
            text.append("\n");
        }
    } else {
        text.remove(QRegularExpression("\\n$"));
    }
    ui->recv_textEdit->setPlainText(text);
}

// 更新时间到 time_label
void Widget::updateTimeLabel() {
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->time_label->setText(currentTime);
}

// 更新 size_label
void Widget::updateSizeLabel() {
    QString sizeText;
    if (totalTextSize < 1024 * 1024) {
        sizeText = QString("Size: %1 Kb").arg(totalTextSize / 1024.0, 0, 'f', 2);
    } else if (totalTextSize < 1024 * 1024 * 1024) {
        sizeText = QString("Size: %1 Mb").arg(totalTextSize / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        sizeText = QString("Size: %1 Gb").arg(totalTextSize / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
    ui->size_label->setText(sizeText);
}

//保存为文件
void Widget::on_Save_pushButton_clicked()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Can't open file to write!");
        return;
    }
    QTextStream out(&file);
    out << ui->recv_textEdit->toPlainText();
    file.close();
    ui->info_label->setText("Saved to " + fileName);
    ui->recv_textEdit->clear();
    totalTextSize = 0; // 重置总文本大小
    updateSizeLabel(); // 更新 size_label
}