#include "widget.h"
#include "ncombobox.h"
#include "./ui_widget.h"
#include "chart.h"
#include "chartview.h"

#include <QLineSeries>
#include <QMainWindow>
#include <QRandomGenerator>
#include <QtMath>
#include <QValueAxis>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , serialPortHandler(new SerialPortHandler())
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayoutGlobal);//布局缩放

    serialPortHandler = new SerialPortHandler();
    System_Init();//系统初始化
}

Widget::~Widget()
{
    delete ui;
}

// 系统初始化
void Widget::System_Init()
{
//----------------------时间初始化----------------------
    Update_RTC();//更新时间,打开定时器
    // 设置定时器每秒更新一次时间
    timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, this, &Widget::Update_RTC);
    timeUpdateTimer->start(TIMEOUT1S);
    animateProgressBar(0, 10, 100);
//---------------------界面初始化----------------------
    ui->Addons_groupBox->hide();//默认隐藏扩展
    // 设置默认值
    ui->BaudRate_comboBox->setCurrentIndex(6);// 115200
    ui->DataBits_comboBox->setCurrentIndex(3);// 8
    ui->Parity_comboBox->setCurrentIndex(0);// None
    ui->StopBits_comboBox->setCurrentIndex(0);// 1
    ui->FlowControl_comboBox->setCurrentIndex(0);// None
    ui->AUTOSEND_time_spinBox->setValue(1000); // 初始化自动发送时间为1000ms
    totalTextSize = 0; // 初始化总文本大小为0
    autoReline_Flag = 0;//初始化自动换行标志
    autoSend_Flag = 0;//初始化自动发送标志
    animateProgressBar(0, 20, 100);

//---------------------串口初始化----------------------
    connect(ui->Serial_number_comboBox,SIGNAL(clicked()),this,SLOT(Update_SerialPort()));//点击串口下拉框刷新串口
    connect(serialPortHandler->serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleSerialError(QSerialPort::SerialPortError)));//串口错误处理
    Update_SerialPort();
    animateProgressBar(0, 30, 100);

    //生成正弦波
    auto series = new QLineSeries;
    for (int i = 0; i < 500; i++) {
        QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        p.ry() += QRandomGenerator::global()->bounded(20);
        *series << p;
    }
    //生成方波
    auto series2 = new QLineSeries;
    for (int i = 0; i < 500; i++) {
       if(i%50 < 25){
           QPointF p((qreal) i, 100);
           p.ry() += QRandomGenerator::global()->bounded(20);
           *series2 << p;
        }
        else{
            QPointF p((qreal) i, -100);
            p.ry() += QRandomGenerator::global()->bounded(20);
            *series2 << p;
        }
}
    //创建图表
    auto chart = new Chart;
    chart->addSeries(series);
    chart->setTitle("Zoom in/out example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    chart->createDefaultAxes();
    auto chartView = new ChartView(chart); // 使用自定义的 ChartView 类
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->DCDC_ChartView = chartView; // 设置自定义的 ChartView 对象
    ui->gridLayout_7->addWidget(ui->DCDC_ChartView, 1, 0, 1, 1);//添加到布局
    //ui->DCDC_ChartView->setChart(chart);

    auto Chart22 = new Chart;
    Chart22->addSeries(series2);
    Chart22->setTitle("Zoom in/out example");
    Chart22->setAnimationOptions(QChart::SeriesAnimations);
    Chart22->legend()->hide();
    Chart22->createDefaultAxes();
    auto chartView2 = new ChartView(Chart22); // 使用自定义的 ChartView 类
    chartView2->setRenderHint(QPainter::Antialiasing);
    ui->chart2 = chartView2; // 设置自定义的 ChartView 对象

    ui->gridLayout_8->addWidget(ui->chart2, 0, 0, 1, 1);
    //ui->chart2->setChart(chart);
   
}

// 串口错误处理
void Widget::handleSerialError(QSerialPort::SerialPortError error)
{
    switch (error)
    {
        case QSerialPort::DeviceNotFoundError:
            ui->info_label->setText("Error: 找不到设备。");
            break;
        case QSerialPort::PermissionError:
            ui->info_label->setText("Error: 没有足够的权限。");
            break;
        case QSerialPort::OpenError:
            ui->info_label->setText("Error: 打开设备时出错。");
            break;
        // case QSerialPort::ParityError:
        //     ui->info_label->setText("Error: 奇偶校验错误。");
        //     break;
        // case QSerialPort::FramingError:
        //     ui->info_label->setText("Error: 帧错误。");
        //     break;
        // case QSerialPort::BreakConditionError:
        //     ui->info_label->setText("Error: 断线条件错误。");
        //     break;
        case QSerialPort::WriteError:
            ui->info_label->setText("Error: 写数据时出错。");
            break;
        case QSerialPort::ReadError:
            ui->info_label->setText("Error: 读数据时出错。");
            break;
        case QSerialPort::ResourceError:
            ui->info_label->setText("Error: 设备被意外移除或系统资源不足。");
            break;
        case QSerialPort::UnsupportedOperationError:
            ui->info_label->setText("Error: 不支持的操作。");
            break;
        case QSerialPort::UnknownError:
            ui->info_label->setText("Error: 未知错误。");
            break;
        case QSerialPort::TimeoutError:
            ui->info_label->setText("Error: 操作超时。");
            break;
        case QSerialPort::NotOpenError:
            ui->info_label->setText("Error: 尝试操作一个未打开的设备。");
            break;
        default:
            ui->info_label->setText("Error: 未知错误。");
            break;
    }
}

void Widget::on_Serial_number_comboBox_clicked()
{
    Update_SerialPort();
}

// 定时器事件回调函数
void Widget::timerEvent(QTimerEvent *event)
{
    // if(event->timerId() == SerialPort_Refreash_TimerID){
    //     qDebug() << "SerialPort_Refreash_TimerID" << Qt::endl;
    // }
}

//更新串口到ComboBox
void Widget::Update_SerialPort()
{
    serialPortHandler->refreshSerialPort();
    ui->Serial_number_comboBox->clear();
   for(int i = 0; i < serialPortHandler->SerialPortList.size(); i++){
        ui->Serial_number_comboBox->addItem(serialPortHandler->SerialPortList.at(i).portName() + ":" + serialPortHandler->SerialPortList.at(i).description());
    }
    if(serialPortHandler->SerialPortList.size() == 0){
        ui->Serial_number_comboBox->addItem("No Serial Port");
    }
    ui->Serial_number_comboBox->setCurrentIndex(0);
    ui->info_label->setText("Serial Port Refreshed!");
}

//更新RTC
void Widget::Update_RTC()
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->time_label->setText(currentTime);
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

//清空发送数据
void Widget::on_Clear_sendbuf_pushButton_clicked(){
    ui->send_textEdit->clear();
    ui->info_label->setText("Clear Send Data!");
}

//清空接收数据
void Widget::on_Clear_recvbuf_pushButton_clicked(){
    ui->recv_textEdit->clear();
    ui->info_label->setText("Clear Recv Data!");
    ui->Rx_label->setText("Rx: 0 bytes");
    ui->Tx_label->setText("Tx: 0 bytes");
}
//自动发送
void Widget::on_AUTOSEND_pushButton_clicked()
{
    if(serialPortHandler->serialPort->isOpen())
    {
        QString data = ui->send_textEdit->toPlainText(); // 获取文本框中的文本
        if (data.isEmpty()) {
            ui->info_label->setText("Error: 发送框为空");
            qDebug() << "Error: 发送框为空" << Qt::endl;
            return;
        }
        if (ui->AUTOSEND_pushButton->text() == "自动发送") {
            autoSend_time = ui->AUTOSEND_time_spinBox->value();
            if (autoSend_time < 100) {
                ui->info_label->setText("Error: 时间间隔太短");
                qDebug() << "Error: 时间间隔太短" << Qt::endl;
                return;
            }
            autoSendTimer = new QTimer(this);
            connect(autoSendTimer, &QTimer::timeout, this, &Widget::on_SEND_pushButton_clicked);
            autoSendTimer->start(autoSend_time);
            autoSend_Flag = 1;
            ui->AUTOSEND_pushButton->setText("停止发送");
            ui->info_label->setText("Auto Send Start!");
        } else {
            autoSendTimer->stop();
            autoSend_Flag = 0;
            ui->AUTOSEND_pushButton->setText("自动发送");
            ui->info_label->setText("Auto Send Stop!");
        }
    }
    else{
        ui->info_label->setText("Error: 串口未打开");
        qDebug() << "Error: 串口未打开" << Qt::endl;
    }
    
}

// 发送数据
void Widget::on_SEND_pushButton_clicked() {
    if (serialPortHandler->serialPort->isOpen()) {
        QString data = ui->send_textEdit->toPlainText(); // 获取文本框中的文本
        if (data.isEmpty()) {
            ui->info_label->setText("Error: 发送框为空");
            qDebug() << "Error: 发送框为空" << Qt::endl;
            return;
        }
        if(autoSend_Flag ==1)
        {
           // animateProgressBar(0, 100, autoSend_time);
        }
        serialPortHandler->serialPort->write(data.toUtf8()); // 发送数据
        qDebug() << "Send Data:" << data << Qt::endl;

        if (autoReline_Flag == 1) {
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

//保存为文件
void Widget::on_Save_pushButton_clicked()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".txt";
    QFile file(fileName);
    animateProgressBar(0, 50, 500); 
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Can't open file to write!");
        animateProgressBar(50, 0, 500); 
        return;
    }
    QTextStream out(&file);
    out << ui->recv_textEdit->toPlainText();
    file.close();
    animateProgressBar(50, 100, 500);
    ui->info_label->setText("Saved to " + fileName);
    ui->recv_textEdit->clear();
    ui->Rx_label->setText("Rx: 0 bytes");
    ui->Tx_label->setText("Tx: 0 bytes");
    totalTextSize = 0; // 重置总文本大小
    updateSizeLabel(); // 更新 size_label
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

// 扩展开关
void Widget::on_Addons_checkBox_clicked(bool checked)
{
    if(checked){
        ui->Addons_groupBox->show();
        //打印 Addons_checkBox->on/off
        qDebug() <<"Addons_checkBox->" << checked << Qt::endl;
    }
    else{
        ui->Addons_groupBox->hide();
        qDebug() <<"Addons_checkBox->" << checked << Qt::endl;
    }
}

// 打开串口
void Widget::on_Opem_COM_pushButton_clicked()
{
    qDebug() << "Opem_COM_pushButton Clicked" << Qt::endl;
    if(serialPortHandler->serialPort->isOpen()){
        serialPortHandler->closeSerialPort();
        ui->Opem_COM_pushButton->setText("Open COM");
        QString description = ui->Serial_number_comboBox->currentText().split(":").last();
        ui->info_label->setText(QString("%1:%2 Closed!").arg(ui->Serial_number_comboBox->currentText().split(":").at(0), description));
    }
    else{
        // 获取串口配置信息
        SerialPortConfig config;
        config.portName = ui->Serial_number_comboBox->currentText().split(":").at(0);
        config.baudRate = ui->BaudRate_comboBox->currentText().toInt();
        config.dataBits = static_cast<QSerialPort::DataBits>(ui->DataBits_comboBox->currentText().toInt());
        // 配置校验位
        switch (ui->Parity_comboBox->currentIndex()) {
            case 0:
                config.parity = QSerialPort::NoParity;
                break;
            case 1:
                config.parity = QSerialPort::EvenParity;
                break;
            case 2:
                config.parity = QSerialPort::OddParity;
                break;
            case 3:
                config.parity = QSerialPort::SpaceParity;
                break;
            case 4:
                config.parity = QSerialPort::MarkParity;
                break;
            default:
                config.parity = QSerialPort::NoParity;
                break;
        }
        // 配置停止位
        config.stopBits = static_cast<QSerialPort::StopBits>(ui->StopBits_comboBox->currentIndex());

        // 配置流控制
        if(ui->FlowControl_comboBox->currentText() == "None"){
            config.flowControl = QSerialPort::NoFlowControl;
        }
        else if(ui->FlowControl_comboBox->currentText() == "RTS/CTS"){
            config.flowControl = QSerialPort::HardwareControl;
        }
        else if(ui->FlowControl_comboBox->currentText() == "XON/XOFF"){
            config.flowControl = QSerialPort::SoftwareControl;
        }
        else {
            config.flowControl = QSerialPort::NoFlowControl;
        }
        // 打开串口
        if(serialPortHandler->openSerialPort(config)){
            QString description = ui->Serial_number_comboBox->currentText().split(":").last();
            ui->info_label->setText(QString("%1:%2 Open!").arg(config.portName, description));
            ui->Opem_COM_pushButton->setText("Close COM");
        }
        else{
            QString description = ui->Serial_number_comboBox->currentText().split(":").last();
            ui->info_label->setText(QString("ERROR: Open %1:%2 Failed!").arg(config.portName, description));
        }
    }
}
