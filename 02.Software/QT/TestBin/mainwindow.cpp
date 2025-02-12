#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList serialNamePort;


    /*定时发送定时器*/
    timersend = new QTimer();
    /*定时器超时信号槽*/
    connect(timersend, SIGNAL(timeout()), this, SLOT(on_SendDataFile_clicked()));
    connect(ui->timesendcheckBox,SIGNAL(stateChanged(int)),this,SLOT(TimeSendReady(int)));
    foreach(const QSerialPortInfo &info , QSerialPortInfo::availablePorts()){
        serialNamePort<<info.portName();
    }
    QFont font("Microsoft YaHei",10,QFont::Normal);//微软雅黑。字体大小16，Normal：正常，Bold 粗体，Black：黑体，Light：高亮
    setWindowTitle("  串口助手 V3.0 ");
    ui->uartbox->addItems(serialNamePort);      // 引入当前串口
    setFixedSize(930,560); // 固定窗口的大小
    ui->CloseUart->setEnabled(false);       // 断开按键关使能
    ui->RecvTextEdit->setFont(font);
    ui->SendTextEdit->setFont(font);
    connect(ui->ClearRecvButton, SIGNAL(clicked()), this, SLOT(ClearRecvButton()));         // 清空接收按钮
    connect(ui->SaveRecvDataFile, SIGNAL(clicked()), this, SLOT(SaveRecvDataFile()));       // 保存数据按钮

}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
    函   数：serialPortReadyRead_Slot
    描   述：接收数据文本框的槽函数
    输   入：无
    输   出：无
*/
void MainWindow::RcvData(QByteArray RecvBuff)
{
    QString     stringdata;

    ui->RecvTextEdit->setTextColor(QColor(Qt::black));
    // 读取串口接收的数据
    if(ui->recvcheckBox->isChecked()){
        stringdata = RecvBuff.toHex(' ').trimmed().toUpper();/*hex显示*/
    }
    else{
       stringdata = QString(RecvBuff);   /*ascii显示*/
    }

    /*时间戳按钮*/
    if (ui->timecheckBox->isChecked()){
       stringdata = QString("[%1]:RX -> %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz")).arg(stringdata);
       ui->RecvTextEdit->append(stringdata);
    }
    else{
        ui->RecvTextEdit->insertPlainText(stringdata);
    }
    ui->RecvTextEdit->moveCursor(QTextCursor::End);  // 自动滚屏到最后一行 有BUG 不可用光标点击文本框
    dataRxNumber += RecvBuff.length();
    ui->RxNumlabel->setText(QString::number(dataRxNumber));
    RecvBuff.clear();
}

/*
    函   数：on_OpenUart_clicked
    描   述：打开并设置串口的信号槽函数
    输   入：无
    输   出：无
*/
void MainWindow::on_OpenUart_clicked()
{
    qint32 state = 0 ;

    Serial   = new ComSerialPort(ui->uartbox->currentText(),ui->baudbox->currentText().toInt(),&state);

    qDebug() << "主线程ID："<< QThread::currentThreadId();
    if(state){              // 打开串口成功
        ui->OpenUart->setEnabled(false);
        ui->CloseUart->setEnabled(true);
        Serial_State = TRUE;
        QMessageBox::warning(this,tr("提示"),tr("串口连接成功"));
        // 获取串口数据
        connect(this,&MainWindow::SendData,Serial,&ComSerialPort::SendSerialData,Qt::AutoConnection);
        connect(Serial,SIGNAL(UpdateData(QByteArray)),this,SLOT(RcvData(QByteArray)),Qt::AutoConnection);
      }
    else{
        QMessageBox::warning(this,tr("错误"),tr("串口连接失败"));
    }
}

void MainWindow::on_CloseUart_clicked()
{
    Serial->deleteLater();
    Serial_State = 0;
    ui->OpenUart->setEnabled(true);     // 连接串口按键使能
    ui->CloseUart->setEnabled(false);   // 断开按键关使能
}
/*
    函   数：find_port
    描   述：查找串口
    输   入：无
    输   出：无
*/
void MainWindow::find_port()
{
    ui->uartbox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if(serial.open(QIODevice::ReadWrite)){
            ui->uartbox->addItem(serial.portName());        //显示串口名称
            serial.close();
        }
    }
}

void MainWindow::ClearRecvButton()
{
    ui->RecvTextEdit->clear();
    dataRxNumber = 0 ;
    ui->RxNumlabel->setText(QString::number(dataRxNumber));
}
/*
    函   数：SaveRecvDataFile
    描   述：保存数据按钮点击槽函数
    输   入：无
    输   出：无
*/
void MainWindow::SaveRecvDataFile()
{
    QString data;

    data = ui->RecvTextEdit->toPlainText();

    if (data.isEmpty()){
        QMessageBox::information(this, "提示", "数据内容空");
        return;
    }
    QString curPath = QDir::currentPath();            //获取系统当前目录
    QString dlgTitle = "保存文件";                     //对话框标题
    QString filter = "文本文件(*.txt);;所有文件(*.*)";  //文件过滤器
    QString filename = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)){
        return;
    }
    /*保存文件*/
    QTextStream stream(&file);
    stream << data;
    file.close();
}

/*
    函   数：on_SendDataFile_clicked
    描   述：手动发送数据
    输   入：无
    输   出：无
*/
void MainWindow::on_SendDataFile_clicked()
{

    if (Serial_State == TRUE){
        //获取发送框字符
        SendTextStr = ui->SendTextEdit->document()->toPlainText();
        SendTextByte = SendTextStr.toUtf8();
        if (SendTextByte.isEmpty() != true){

            if (ui->HexsendcheckBox->isChecked()) {
                SendTextByte = SendTextByte.fromHex(SendTextByte);
                emit SendData(SendTextByte);
                QString strdata = SendTextByte.toHex(' ').trimmed().toUpper();
                if (ui->timecheckBox->isChecked()) { // 时间戳发送
                    ui->RecvTextEdit->setTextColor(QColor(Qt::blue));  // 时间戳颜色
                    ui->RecvTextEdit->append(QString("[%1]TX -> ").arg(QTime::currentTime().toString("HH:mm:ss:zzz")));
                }
                else{
                  strdata = strdata.append("\r\n");
                }
                ui->RecvTextEdit->setTextColor(QColor(Qt::black));
                ui->RecvTextEdit->insertPlainText(strdata);
            }
            else{
                // 发送ascii数据
                emit SendData(SendTextByte);
                QString strdata = QString(SendTextByte);
                if (ui->timecheckBox->isChecked()) { // 时间戳发送
                    ui->RecvTextEdit->setTextColor(QColor(Qt::red)); // 时间戳颜色
                    ui->RecvTextEdit->append(QString("[%1]TX -> ").arg(QTime::currentTime().toString("HH:mm:ss:zzz")));
                }
                else{
                   strdata = strdata.append("\r\n");
                }
                ui->RecvTextEdit->setTextColor(QColor(Qt::black));
                ui->RecvTextEdit->insertPlainText(strdata);
            }
            //移动光标到末尾
            ui->RecvTextEdit->moveCursor(QTextCursor::End);
        }
        else{
            QMessageBox::warning(this, "警告", "您需要在发送编辑框中输入要发送的数据");
        }

    }
    else{
        QMessageBox::information(this, "警告", "串口未打开");
    }
}
/*
    函   数：TimeSendReady
    描   述：定时发送信号槽函数
    输   入：无
    输   出：无
*/
void MainWindow::TimeSendReady(int state)
{
    int settime;

    if (FALSE == Serial_State){
        QMessageBox::information(this, "提示", "串口未打开");
        return;
    }
    /*判断是否有数据*/
    if (ui->SendTextEdit->document()->isEmpty() == true){
        if (ui->timesendcheckBox->isChecked()){
            QMessageBox::warning(this, "警告", "您需要在发送编辑框中输入要发送的数据");
        }
        return;
    }
    /*判断勾选状态*/
    if (state == Qt::Checked){
        /*获取设定时间*/
        settime = ui->timelineEdit->text().toInt();
        if (settime > 0) {
            timersend->start(settime);
        } else {
            QMessageBox::warning(this, "警告", "时间必须大于0");
        }
        ui->timelineEdit->setEnabled(false);
    }
    else{
        /*停止发送*/
        timersend->stop();
        ui->timelineEdit->setEnabled(true);
    }
}
void MainWindow::SendDataHex(int state)
{
    //获取发送框字符
    SendTextStr = ui->SendTextEdit->document()->toPlainText();
    SendTextByte = SendTextStr.toUtf8();
    if (SendTextStr.isEmpty()){
        return;
    }
    //asccii与hex转换
    if (state == Qt::Checked){
        //转换成16进制数并转换为大写
        SendTextByte = SendTextByte.toHex(' ').toUpper();
        ui->SendTextEdit->document()->setPlainText(SendTextByte);
    }
    else{
        //从QByteArray转换为QString
        SendTextStr = SendTextByte.fromHex(SendTextByte);
        ui->SendTextEdit->document()->setPlainText(SendTextStr);
    }
}

void MainWindow::on_RefreshUart_clicked()
{
    ui->uartbox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if(serial.open(QIODevice::ReadWrite)){
            ui->uartbox->addItem(serial.portName());        //显示串口名称
            serial.close();
        }
    }
}
