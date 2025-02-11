#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newcombobox.h"
#include <algorithm>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

/*********************************串口基本设置*********************************/
    //创建一个定时器用来定时发送
    QTimer *timer1 = new QTimer();
    timer1->start(1000);
    connect(timer1,&QTimer::timeout,[=](){
        int timed = ui->comboBox_timedSend->currentText().toInt();  //获取comboBox的值作为定时发送的时间
        timer1->start(timed);
        if(ui->checkBox_timedSend->isChecked() == true)             //如果定时发送的checkBox被勾选，则调用串口发送函数
        {
            sendData();
        }
    });
    serialPort = new QSerialPort(); //新建一个用来与外界进行通信的串口接口
    serialPortInit();               //串口初始化
    windowInit();                   //基本设置初始化
    rxTxNumLcdInit();               //发送与接收数量lcd初始化

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleSerialError(QSerialPort::SerialPortError)));
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(lcdShowRcvPacketNum()));
    connect(serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleSerialError(QSerialPort::SerialPortError)));
    connect(ui->comboBox_chooseCom,SIGNAL(clicked()),this,SLOT(comboBox_buadClicked()));
    connect(ui->comboBox_setBuad,SIGNAL(activated(int)),this,SLOT(setBuad(int)));
    connect(ui->pushButton_sendData,SIGNAL(clicked()),this,SLOT(sendData()));
    connect(ui->pushButton_openSerialPort,SIGNAL(clicked()),this,SLOT(openSerialport()));
    connect(ui->pushButton_closeSerialPort,SIGNAL(clicked()),this,SLOT(closeSerialport()));
    connect(ui->pushButton_clearRcv,SIGNAL(clicked()),this,SLOT(clearRcv()));
    connect(ui->pushButton_serialPortOutputToFile,SIGNAL(clicked()),this,SLOT(rcvDataOutputToFile()));
/******************************************************************/


/*********************************协议收发*********************************/
    tableInit();    //协议收发表格初始化
    connect(ui->pushButton_convert,SIGNAL(clicked()),this,SLOT(on_btnConvert_clicked()));
    connect(ui->pushButton_clearConvertData,SIGNAL(clicked()),this,SLOT(on_btnClear_clicked()));
    connect(ui->pushButton_sendProtocalData,SIGNAL(clicked()),this,SLOT(sendProtocalHexData()));
/******************************************************************/


/*********************************波形显示*********************************/
    iniBarChart();  //电压波形chartBar初始化
    lcdVoltageInit();

    QTimer *timerDrawLine = new QTimer();
    timerDrawLine->start(1000);
    connect(timerDrawLine,&QTimer::timeout,[=](){
        int timed = ui->comboBox_showPeriod->currentText().toInt();
        timerDrawLine->start(timed);
        if(ui->radioButton_showWave->isChecked())
        {
            drawLine();
        }
    });

    connect(ui->pushButton_zoomIn,SIGNAL(clicked()),this,SLOT(on_actZoomIn_triggered()));
    connect(ui->pushButton_zoomOut,SIGNAL(clicked()),this,SLOT(on_actZoomOut_triggered()));
    connect(ui->pushButton_zoomReset,SIGNAL(clicked()),this,SLOT(on_actZoomReset_triggered()));
    connect(ui->pushButton_serialPortOutputToFileWaveData,SIGNAL(clicked()),this,SLOT(waveDataOutputToFile()));
/******************************************************************/


/*********************************pid调试*********************************/
    pidInit();
    QTimer *pidTimer = new QTimer();
    pidTimer->start(1000);
    connect(pidTimer,&QTimer::timeout,[=](){
        int timed = ui->comboBox_pidShowPeriod->currentText().toInt();
        pidTimer->start(timed);
        if(ui->radioButton_pidShow->isChecked())
        {
            showPidWave();
        }
    });
    connect(ui->doubleSpinBox_pidKp,SIGNAL(valueChanged(double)),this,SLOT(pidKpSet(double)));
    connect(ui->doubleSpinBox_pidKi,SIGNAL(valueChanged(double)),this,SLOT(pidKiSet(double)));
    connect(ui->doubleSpinBox_pidKd,SIGNAL(valueChanged(double)),this,SLOT(pidKdSet(double)));
    connect(ui->doubleSpinBox_pidTar,SIGNAL(valueChanged(double)),this,SLOT(pidTarSet(double)));
/******************************************************************/


/*********************************固件升级*********************************/
    //固件升级部分待完善
    fwUpdateInit();
    connect(fwUpdateTimeOut,&QTimer::timeout,[=](){
        if(fwUpdateState == fwStart)
        {
            fwStartCmd();
        }
        if(fwUpdateState == fwNum)
        {
            fwNumCmd();
        }
        fwExceedIndex++;
        if(fwExceedIndex >= fwExceedTimes)
        {
            fwUpdateState = fwStop;
            fwUpdateTimer->stop();
            fwUpdateTimeOut->stop();
            ui->textEdit_fwRcvInfo->clear();
            ui->textEdit_fwRcvInfo->setWordWrapMode(QTextOption::WordWrap);
            ui->textEdit_fwRcvInfo->insertPlainText("超时未回复 升级失败");
        }
    });


    connect(fwUpdateTimer,&QTimer::timeout,[=](){
        if(fwUpdateState == fwStart)
        {
        }

        if(fwUpdateState == fwTransfer)
        {
            QByteArray fwSendBuff = binRawData.mid(fwPackIndex*fwPackLength,fwPackLength);
            fwSendBuff.insert(0,fwSendBuff.length());
            QByteArray fwSendProtocolBuff = loadFwPackData(&fwSendBuff);
            serialPort->write(fwSendProtocolBuff);

            fwPackIndex++;
            QString fwDataString = ByteArrayToHexString(fwSendProtocolBuff).toLatin1();
            ui->textEdit_fwSendInfo->clear();
            ui->textEdit_fwSendInfo->setWordWrapMode(QTextOption::WordWrap);
            ui->textEdit_fwSendInfo->insertPlainText(QString("["));
            ui->textEdit_fwSendInfo->insertPlainText(QString::number(fwPackIndex));
            ui->textEdit_fwSendInfo->insertPlainText(QString("]  "));
            ui->textEdit_fwSendInfo->insertPlainText(fwDataString);
            fwUpdateTimer->stop();
            if(fwPackIndex>=fwPackNum)
            {
                fwUpdateState = fwComplete;
                fwUpdateTimer->stop();
                QByteArray stopCmd;
                uchar stopCmd1 = 0xAB;
                uchar stopCmd2 = 0xf3;
                stopCmd = loadTxMsg(stopCmd1, stopCmd2, &stopCmd);
                serialPort->write(stopCmd);
            }
        }
    });

    connect(ui->pushButton_chooseFwUpdateFile,SIGNAL(clicked()),this,SLOT(readFw()));
    connect(ui->pushButton_startFwUpdate,SIGNAL(clicked()),this,SLOT(startFwUpdate()));
    connect(ui->pushButton_stopFwUpdate,SIGNAL(clicked()),this,SLOT(stopFwUpdate()));
/******************************************************************/

}

//MainWindow类的析构函数，关闭窗口时对象资源被释放
MainWindow::~MainWindow()
{
    delete ui;
}


/*********************************串口基本设置相关函数*********************************/
void MainWindow::handleSerialError(QSerialPort::SerialPortError serialPortErr)
{
    if(serialPortErr == QSerialPort::ResourceError)
    {
        QMessageBox::critical(NULL, "critical", "设备拔出", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        closeSerialport();
    }
    if(serialPortErr == QSerialPort::DeviceNotFoundError)
    {
        QMessageBox::critical(NULL, "critical", "找不到串口", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        closeSerialport();
    }
}

void MainWindow::lcdShowRcvPacketNum()
{
    ui->lcdNumber_rcvPackets->display(rcvPackets);
}

void MainWindow::lcdShowSendPacketNum()
{
    ui->lcdNumber_sendPackets->display(sendPackets);
}

void MainWindow::rcvDataOutputToFile()
{
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString fileName = dlg.getSaveFileName(this, tr("Save As"), "./", tr("Text File(*.txt)"));

    if( fileName == "" )
    {
        return;
    }
    //内容保存到路径文件
    QFile file(fileName);
    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file); //IO设备对象的地址对其进行初始化

        out << ui->textEdit_RecData->toPlainText(); //输出

        QMessageBox::warning(this, tr("Finish"), tr("Successfully save the file!"));

        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Fail to open file!"));
    }
}

//波特率选择comboBox点击的槽函数
void MainWindow::comboBox_buadClicked()
{
    refreshCom();
}

void MainWindow::refreshCom()
{
    //显示串口列表
    ui->comboBox_chooseCom->clear();
    foreach(QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
        ui->comboBox_chooseCom->addItem(portInfo.portName()+":"+portInfo.description());
    ui->pushButton_openSerialPort->setEnabled(ui->comboBox_chooseCom->count()>0);	//
}

void MainWindow::windowInit()
{
    ui->pushButton_closeSerialPort->setEnabled(false);
    ui->pushButton_openSerialPort->setEnabled(false);
    ui->pushButton_sendData->setEnabled(false);
    //    setWindowTitle(tr("串口收发"));

    ui->comboBox_timedSend->addItem("10");
    ui->comboBox_timedSend->addItem("100");
    ui->comboBox_timedSend->addItem("1000");
    ui->comboBox_timedSend->setCurrentIndex(2);
}

void MainWindow::serialPortInit(){
    refreshCom();
    ui->comboBox_setBuad->addItem("9600");
    ui->comboBox_setBuad->addItem("115200");
    ui->comboBox_setBuad->addItem("921600");
    ui->comboBox_setBuad->setCurrentIndex(1);
}

void MainWindow::openSerialport()
{
    ui->pushButton_closeSerialPort->setEnabled(true);
    ui->pushButton_openSerialPort->setEnabled(false);

    QList<QSerialPortInfo>  comList = QSerialPortInfo::availablePorts();
    QSerialPortInfo portInfo = comList.at(ui->comboBox_chooseCom->currentIndex());
    serialPort->setPort(portInfo);      //设置使用哪个串口

    if(serialPort->open(QIODevice::ReadWrite) == false)
    {
        QMessageBox::critical(NULL, "critical", "找不到串口/串口被占用", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        closeSerialport();
    }
    else
    {
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        ui->pushButton_sendData->setEnabled(true);
    }
}

void MainWindow::closeSerialport()
{
    if(serialPort->isOpen()){
        serialPort->clear();
        serialPort->close();
    }
    ui->pushButton_closeSerialPort->setEnabled(false);
    ui->pushButton_openSerialPort->setEnabled(true);
}

void MainWindow::setBuad(int buad)
{
    QString str = ui->comboBox_setBuad->currentText();
    serialPort->setBaudRate(str.toInt());
}

void MainWindow::clearRcv()
{
    rcvPackets = 0;
    ui->textEdit_RecData->clear();
}

void MainWindow::sendData()
{
    if(serialPort->isOpen())
    {
        QString message = ui->lineEdit_sendData->text();
        if(ui->checkBox_hexSend->isChecked() == true)
        {
            if(ui->checkBox_sendNewLine->isChecked() == true)
            {
                QByteArray sendBuf = QByteArray::fromHex(message.toLatin1());
                int cnt = sendBuf.size();
                sendBuf = sendBuf.insert(cnt,"\r\n");
                serialPort->write(sendBuf);
            }
            else
            {
                serialPort->write(QByteArray::fromHex(message.toLatin1()));
            }
        }
        else
        {
            if(ui->checkBox_sendNewLine->isChecked() == true)
            {
                QByteArray sendBuf = message.toLocal8Bit();
                int cnt = sendBuf.size();
                sendBuf = sendBuf.insert(cnt,"\r\n");
                serialPort->write(sendBuf);
            }
            else
            {
                serialPort->write(message.toLocal8Bit());
            }

        }
        if(message.length() != 0)
        {
            sendPackets++;
            lcdShowSendPacketNum();
        }
    }
}

void MainWindow::receiveData()
{
    QByteArray message = serialPort->readAll();
    QString hexMsg;
    QDateTime time = QDateTime::currentDateTime();  //获取系统现在的时间
    QString date = time.toString("hh:mm:ss");       //设置显示格式
    if(ui->checkBox_hexRcv->isChecked() == true)
    {
        int s32Ret = 0;
        s32Ret = msgProcCrc(&message);
        if (s32Ret != 0)
        {
            return;
        }

        tableRowCnt++;
        ui->tableWidget->setRowCount(tableRowCnt);
        createItemsARow(tableRowCnt-1,&message);

        rcvFwReply(&message);

        if(ui->radioButton_showWave->isChecked())
        {
            parsingData(&message);
        }

        if(ui->radioButton_pidShow->isChecked())
        {
            pidParsingData(&message);
        }

        hexMsg = ByteArrayToHexString(message).toLatin1();
        if(ui->checkBox_timeStamp->isChecked() == true)
        {
            ui->textEdit_RecData->append(date+QString("->  ")+hexMsg.toUpper());
        }
        else
        {
            ui->textEdit_RecData->setWordWrapMode(QTextOption::WordWrap);
            ui->textEdit_RecData->insertPlainText(hexMsg.toUpper());
        }
    }
    else
    {
        QString data = QString::fromLocal8Bit(message);
        if(ui->checkBox_timeStamp->isChecked() == true)
        {

            ui->textEdit_RecData->append(date+QString("->  ")+QString(data));
        }
        else
        {
            ui->textEdit_RecData->insertPlainText(QString(data));
        }
    }
    rcvPackets++;
}

/******************************************************************/


/*********************************协议收发相关函数*********************************/
void MainWindow::rxTxNumLcdInit()
{
    ui->lcdNumber_rcvPackets->setDigitCount(5);
    ui->lcdNumber_rcvPackets->setDigitCount(5);
    ui->lcdNumber_rcvPackets->setMode(QLCDNumber::Dec);
    ui->lcdNumber_sendPackets->setMode(QLCDNumber::Dec);
    ui->lcdNumber_rcvPackets->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_sendPackets->setSegmentStyle(QLCDNumber::Flat);
}

void MainWindow::tableInit()
{
    QStringList headerText;
    headerText<<"时间"<<"命令1"<<"命令2"<<"数据";
    ui->tableWidget->setColumnCount(headerText.size());      //设置表格列数
    ui->tableWidget->resizeColumnsToContents();
    for (int i=0;i<ui->tableWidget->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setBold(true);              //设置为粗体
        font.setPointSize(10);           //字体大小
        headerItem->setForeground(QBrush(Qt::black));  //设置文字颜色
        headerItem->setFont(font);       //设置字体
        ui->tableWidget->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }
    ui->tableWidget->setColumnWidth(0, 60);
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 50);
    ui->tableWidget->setColumnWidth(3, 250);
}

void MainWindow::on_btnClear_clicked()
{
    ui->lineEdit_protocalData->clear();
}

void MainWindow::on_btnConvert_clicked()
{
    ui->lineEdit_protocalData->clear();
    bool ok;
    int val1= ui->lineEditCmd1->text().toInt(&ok,16);  //以十六进制数读入
    uchar cmd1 = uchar(val1);

    int val2= ui->lineEditCmd2->text().toInt(&ok,16);  //以十六进制数读入
    uchar cmd2 = uchar(val2);

    QByteArray data = QByteArray::fromHex(ui->lineEditData->text().toLatin1());

    QByteArray prorocolData = loadTxMsg(cmd1, cmd2, &data);
    QString showProrocolData = ByteArrayToHexString(prorocolData);
    ui->lineEdit_protocalData->insert(showProrocolData);
}

void MainWindow::sendProtocalHexData()
{
    QString message = ui->lineEdit_protocalData->text();
    serialPort->write(QByteArray::fromHex(message.toLatin1()));
    sendPackets++;
    lcdShowSendPacketNum();
}
/******************************************************************/

/*********************************pid调参相关函数*********************************/
void MainWindow::pidInit()
{
    ui->comboBox_pidShowPeriod->addItem("100");
    ui->comboBox_pidShowPeriod->addItem("1000");
    ui->comboBox_pidShowPeriod->addItem("5000");
    ui->comboBox_pidShowPeriod->setCurrentIndex(1);

    QChart *pidChart =ui->chartViewPid->chart(); //获取ChartView关联的chart
    pidChart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewPid->setChart(pidChart);  //为ChartView设置chart
    ui->chartViewPid->setRenderHint(QPainter::Antialiasing);
    ui->chartViewPid->setViewportUpdateMode(QChartView::FullViewportUpdate);

    QLineSeries *pidSeriesLineTar = new QLineSeries();
    pidSeriesLineTar->setName("目标值");
    QLineSeries *pidSeriesLineAct = new QLineSeries();
    pidSeriesLineAct->setName("实际值");

    pidChart->addSeries(pidSeriesLineTar);
    pidChart->addSeries(pidSeriesLineAct);

    //创建x坐标
    QDateTimeAxis *pidQaX = new QDateTimeAxis;
    //格式
    pidQaX->setFormat("mm:ss");
    pidQaX->setTickCount(10);
    pidQaX->setTitleText("time");

    //创建y坐标
    QValueAxis *PidQaY = new QValueAxis;
    //设置范围
    PidQaY->setRange(-2,8);
    PidQaY->setTickCount(5);

    //将线条放入表中
    pidChart->setAxisX(pidQaX,pidSeriesLineTar);
    pidChart->setAxisY(PidQaY,pidSeriesLineTar);
    pidChart->setAxisX(pidQaX,pidSeriesLineAct);
    pidChart->setAxisY(PidQaY,pidSeriesLineAct);
}

void MainWindow::showPidWave()
{
    //获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    //获取初始化的qchart
    QChart *chart =(QChart *)ui->chartViewPid->chart();

    //获取之前的ser
    QLineSeries *pidSeriesLineTar = (QLineSeries *)ui->chartViewPid->chart()->series().at(0);
    pidSeriesLineTar->append(currentTime.toMSecsSinceEpoch(),pidRcvTarData);

    QLineSeries *pidSeriesLineAct = (QLineSeries *)ui->chartViewPid->chart()->series().at(1);
    pidSeriesLineAct->append(currentTime.toMSecsSinceEpoch(),pidRcvActData);


    chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-1*30));
    chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(1*10));
}

//cmd1为0xBA, 将cmd2和f32数据进行组包
QByteArray MainWindow::protocalDataSet(uchar cmd, double doubleData)
{
    uchar preFix = 0xA5;
    uchar len = 0x0A;
    uchar cmd1 = 0xBA;
    uchar cmd2 = cmd;
    uchar uData[4];
    uchar crc = len + cmd1 + cmd2;
    uchar postFix = 0x5A;
    QByteArray sendBuf;
    float data = static_cast<float>(doubleData);
    sendBuf.append(preFix);
    sendBuf.append(len);
    sendBuf.append(cmd1);
    sendBuf.append(cmd2);
    *(int *)&uData[0] = *(int *)&data;
    sendBuf.append(uData[0]);
    sendBuf.append(uData[1]);
    sendBuf.append(uData[2]);
    sendBuf.append(uData[3]);

    for(int i=4; i<8; i++)
    {
        crc += sendBuf.at(i);
    }
    sendBuf.append(crc);
    sendBuf.append(postFix);
    return sendBuf;
}

void MainWindow::pidKpSet(double sendKp)
{
    if(serialPort->isOpen())
    {
        uchar cmd = 0x14;
        QByteArray sendBuf = protocalDataSet(cmd,sendKp);
        serialPort->write(sendBuf);
    }
}

void MainWindow::pidKiSet(double sendKi)
{
    if(serialPort->isOpen())
    {
        uchar cmd = 0x15;
        QByteArray sendBuf = protocalDataSet(cmd,sendKi);
        serialPort->write(sendBuf);
    }
}

void MainWindow::pidKdSet(double sendKd)
{
    if(serialPort->isOpen())
    {
        uchar cmd = 0x16;
        QByteArray sendBuf = protocalDataSet(cmd,sendKd);
        serialPort->write(sendBuf);

    }
}

void MainWindow::pidTarSet(double sendTar)
{
    if(serialPort->isOpen())
    {
        uchar cmd = 0x17;
        QByteArray sendBuf = protocalDataSet(cmd,sendTar);
        serialPort->write(sendBuf);
    }
}

void MainWindow::pidParsingData(QByteArray *protocalData)
{
    uchar preFix = 0xA5;
    uchar cmd1Rcv = 0xAB;
    uchar cmd2RcvTar = 0x13;
    uchar cmd2RcvAct = 0x12;
    uchar crc = 0;
    uchar temp = 0;
    if(static_cast<uchar>(protocalData->at(0)) == preFix)
    {
        for(int i=1; i<protocalData->length()-2; i++)
        {
            temp = static_cast<uchar>(protocalData->at(i));
            crc += static_cast<uchar>(protocalData->at(i));
        }
        if(crc != static_cast<uchar>(protocalData->at(protocalData->length()-2)))
        {
            return;
        }
        uchar cmd1 = protocalData->at(2);
        uchar cmd2 = protocalData->at(3);
        uchar data = protocalData->at(4);

        if(cmd1==cmd1Rcv)
        {
            uchar uData[4];
            uData[0] = static_cast<uchar>(protocalData->at(4));
            uData[1] = static_cast<uchar>(protocalData->at(5));
            uData[2] = static_cast<uchar>(protocalData->at(6));
            uData[3] = static_cast<uchar>(protocalData->at(7));
            if(cmd2==cmd2RcvTar)
            {
                *(int *)&pidRcvTarData = *(int *)&uData;
                ui->lcdnumber_pidTarget->display(pidRcvTarData);
            }
            if(cmd2==cmd2RcvAct)
            {
                *(int *)&pidRcvActData = *(int *)&uData;
                ui->lcdnumber_pidActual->display(pidRcvActData);
            }

        }
    }
}

//为一行的单元格创建 Items
void MainWindow::createItemsARow(int rowNum, QByteArray *protocalData)
{
    uchar preFix = 0xA5;
    uchar crc = 0;
    uchar temp = 0;
    temp = static_cast<uchar>(protocalData->at(0));
    if(static_cast<uchar>(protocalData->at(0)) == preFix)
    {
        for(int i=1; i<protocalData->length()-2; i++)
        {
            temp = static_cast<uchar>(protocalData->at(i));
            crc += static_cast<uchar>(protocalData->at(i));
        }
        temp = static_cast<uchar>(protocalData->at(protocalData->length()-2));
        if(crc != static_cast<uchar>(protocalData->at(protocalData->length()-2)))
        {
            return;
        }
        uchar len = protocalData->at(1);
        uchar cmd1 = protocalData->at(2);
        uchar cmd2 = protocalData->at(3);
        QByteArray data = protocalData->mid(4,len-6);
        QDateTime curTime = QDateTime::currentDateTime();//获取系统现在的时间
        QString time = curTime.toString("hh:mm:ss"); //设置显示格式

        uint8_t str1 = static_cast<uint8_t>(cmd1);
        QString hexStr1 = QString("%1").arg(str1, 2, 16, QLatin1Char('0')).toUpper();
        uint8_t str2 = static_cast<uint8_t>(cmd2);
        QString hexStr2 = QString("%1").arg(str2, 2, 16, QLatin1Char('0')).toUpper();
        QString testdata = ByteArrayToHexString(data).toLatin1().toUpper();

        QTableWidgetItem *item = new QTableWidgetItem(time, ctTime);
        ui->tableWidget->setItem(rowNum, colTime, item);

        item = new QTableWidgetItem(hexStr1, ctCmd1);
        ui->tableWidget->setItem(rowNum, colCmd1, item);

        item = new QTableWidgetItem(hexStr2, ctCmd2);
        ui->tableWidget->setItem(rowNum, colCmd2, item);

        item = new QTableWidgetItem(testdata, ctData);
        ui->tableWidget->setItem(rowNum, colData, item);
    }

    auto lastRowIndex = ui->tableWidget->rowCount()-1; // 最后一行的索引
    auto lastModelIndex = ui->tableWidget->model()->index(lastRowIndex, 0);
    ui->tableWidget->scrollTo(lastModelIndex);         // 滚动到最后一行
}
/******************************************************************/

/*********************************电压波形显示相关函数*********************************/
void MainWindow::on_actZoomIn_triggered()
{//放大
    ui->chartViewBar->chart()->zoom(1.2);
}

void MainWindow::on_actZoomOut_triggered()
{//缩小
    ui->chartViewBar->chart()->zoom(0.8);
}

void MainWindow::on_actZoomReset_triggered()
{ //恢复原始大小
    ui->chartViewBar->chart()->zoomReset();
}

void MainWindow::lcdVoltageInit()
{
    ui->lcdnumber_vlotage->setDigitCount(5);
    ui->lcdnumber_vlotage->setMode(QLCDNumber::Dec);
    ui->lcdnumber_vlotage->setSegmentStyle(QLCDNumber::Flat);
}

void MainWindow::iniBarChart()
{
    ui->comboBox_showPeriod->addItem("100");
    ui->comboBox_showPeriod->addItem("1000");
    ui->comboBox_showPeriod->addItem("5000");
    ui->comboBox_showPeriod->setCurrentIndex(1);

    QChart *chart =ui->chartViewBar->chart(); //获取ChartView关联的chart
    //    chart->setTitle("电压波形显示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewBar->setChart(chart);  //为ChartView设置chart
    ui->chartViewBar->setRenderHint(QPainter::Antialiasing);
    ui->chartViewBar->setViewportUpdateMode(QChartView::FullViewportUpdate);

    QLineSeries *seriesLine = new QLineSeries();
    seriesLine->setName("电压波形");
    chart->addSeries(seriesLine);

    //创建x坐标
    QDateTimeAxis *QaX = new QDateTimeAxis;
    //格式
    QaX->setFormat("mm:ss");
    QaX->setTickCount(10);
    QaX->setTitleText("time");

    //创建y坐标
    QValueAxis *QaY = new QValueAxis;
    //设置范围
    QaY->setRange(-0.1,3.4);
    QaY->setTickCount(5);

    //将线条放入表中
    chart->setAxisX(QaX,seriesLine);
    chart->setAxisY(QaY,seriesLine);
}

//将电压数据进行组包
void MainWindow::parsingData(QByteArray *protocalData)
{
    uchar preFix = 0xA5;
    uchar cmd1Rcv = 0xAB;
    uchar cmd2Rcv = 0x11;
    uchar crc = 0;
    uchar temp;
    if(static_cast<uchar>(protocalData->at(0)) == preFix)
    {
        for(int i=1; i<protocalData->length()-2; i++)
        {
            temp = static_cast<uchar>(protocalData->at(i));
            crc += static_cast<uchar>(protocalData->at(i));
        }
        if(crc != static_cast<uchar>(protocalData->at(protocalData->length()-2)))
        {
            return;
        }
        uchar cmd1 = protocalData->at(2);
        uchar cmd2 = protocalData->at(3);
        uchar data = protocalData->at(4);

        if((cmd1==cmd1Rcv) && (cmd2 ==cmd2Rcv))
        {
            uchar uData[4];
            uData[0] = static_cast<uchar>(protocalData->at(4));
            uData[1] = static_cast<uchar>(protocalData->at(5));
            uData[2] = static_cast<uchar>(protocalData->at(6));
            uData[3] = static_cast<uchar>(protocalData->at(7));
            *(int *)&receiveParsingData = *(int *)&uData;
            ui->lcdnumber_vlotage->display(receiveParsingData);
        }
    }
}

//将每一次需要显示的波形数据连接成折线图
void MainWindow::drawLine()
{
    //获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    //获取初始化的qchart
    QChart *chart =(QChart *)ui->chartViewBar->chart();

    //获取之前的ser
    QLineSeries *seriesLine = (QLineSeries *)ui->chartViewBar->chart()->series().at(0);

    seriesLine->append(currentTime.toMSecsSinceEpoch(),receiveParsingData);

    chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-1*30));
    chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(1*10));

    //获取当前时间
    QDateTime curTime = QDateTime::currentDateTime();
    QString date = curTime.toString("yyyy-MM-dd"); //设置显示格式
    QString time = curTime.toString("hh:mm:ss:zzz"); //设置显示格式
    waveArray<<date<<"\t"<<time<<"\t"<<QString::number(receiveParsingData)<<"\n";
}

//将波形数据输出到文件
void MainWindow::waveDataOutputToFile()
{
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString fileName = dlg.getSaveFileName(this, tr("Save As"), "./", tr("Text File(*.txt)"));

    if( fileName == "" )
    {
        return;
    }

    //内容保存到路径文件
    QFile file(fileName);

    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file); //IO设备对象的地址对其进行初始化

        QVector<QString>::iterator iter;
        for (iter=waveArray.begin();iter!=waveArray.end();iter++)
            out  <<  *iter << "\0";

        QMessageBox::warning(this, tr("Finish"), tr("Successfully save the file!"));

        file.close();
        waveArray.clear();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Fail to open file!"));
    }
}

/******************************************************************/


/*********************************固件升级相关函数*********************************/
void MainWindow::fwUpdateInit()
{
    ui->pushButton_startFwUpdate->setEnabled(false);
    ui->pushButton_stopFwUpdate->setEnabled(false);
    fwUpdateState = 0;
}

QByteArray MainWindow::loadFwPackData(QByteArray *protocalData)
{
    uchar cmd1 = 0xAB;
    uchar cmd2 = 0xf2;
    QByteArray  txFwData;
    uchar uData[2];
    uint16_t u16FwPackIndex = fwPackIndex+1;
    *(uint16_t *)&uData[0] = *(uint16_t *)&u16FwPackIndex;
    protocalData->insert(0,uData[0]);
    protocalData->insert(1,uData[1]);
    txFwData = loadTxMsg(cmd1, cmd2, protocalData);
    return txFwData;
}

void MainWindow::readFw()
{
    QFileDialog dlg(this);

    QString fileName = dlg.getOpenFileName(this, tr("Open"), "./", tr("Bin File(*.bin)"));

    if( fileName == "" )
    {
        return;
    }

    QFile file(fileName);
    QFileInfo fileInfo(fileName);

    fwPackIndex = 0;
    fwFileLen = fileInfo.size();
    fwPackNum = fwFileLen/fwPackLength+1;

    if(file.open(QIODevice::ReadOnly))
    {
        binRawData = file.readAll();
        ui->lineEdit_fwUpdateFile->setText(fileName);
        ui->textEdit_fwUpdateFile->append(binRawData.toHex());
        file.close();
        ui->pushButton_startFwUpdate->setEnabled(true);
        ui->pushButton_stopFwUpdate->setEnabled(false);
        fwUpdateState = fwInit;
        fwPackIndex = 0;
        fwExceedFlag = 0;
        fwExceedIndex = 0;
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Fail to open file!"));
    }
}

void MainWindow::startFwUpdate()
{
    ui->pushButton_startFwUpdate->setEnabled(false);
    ui->pushButton_stopFwUpdate->setEnabled(true);
    fwUpdateState = fwStart;

    fwStartCmd();

    fwUpdateTimeOut->start(2000);
    fwExceedIndex = 0;
    fwExceedFlag = 0;
}

void MainWindow::fwStartCmd()
{
    uchar uData0[0];
    *(uint16_t *)&uData0[0] = *(uint16_t *)&fwFileLen;
    QByteArray startCmd;
    startCmd.append(uData0[0]);
    startCmd.append(uData0[1]);
    uchar startCmd1 = 0xAB;
    uchar startCmd2 = 0xf0;
    startCmd = loadTxMsg(startCmd1, startCmd2, &startCmd);
    serialPort->write(startCmd);
}

void MainWindow::fwNumCmd()
{
    uchar cmd1 = 0xAB;
    uchar cmd2 = 0xf1;
    uchar uData1[2];
    uint16_t u16FwPackNum = fwPackNum;
    *(uint16_t *)&uData1[0] = *(uint16_t *)&u16FwPackNum;
    QByteArray  txFwData;
    txFwData.append(uData1[0]);
    txFwData.append(uData1[1]);
    txFwData = loadTxMsg(cmd1, cmd2, &txFwData);
    serialPort->write(txFwData);
}

void MainWindow::stopFwUpdate()
{
    ui->pushButton_startFwUpdate->setEnabled(true);
    ui->pushButton_stopFwUpdate->setEnabled(false);
    fwUpdateState = fwStop;
    fwUpdateTimer->stop();
}

void MainWindow::rcvFwReply(QByteArray *protocalData)
{
    uint8_t pRxData[100];
    memcpy(pRxData, protocalData->data(), protocalData->size());
    ui->textEdit_fwRcvInfo->clear();
    ui->textEdit_fwRcvInfo->setWordWrapMode(QTextOption::WordWrap);
    if(pRxData[3] == 0xF0)
    {
        if(pRxData[4] == 1)
        {
             fwExceedIndex = 0;
             fwUpdateState = fwTransfer;
             fwNumCmd();
             fwUpdateTimer->start(1000);
             fwUpdateTimeOut->start(2000);
             ui->textEdit_fwRcvInfo->insertPlainText(QString("开始升级"));
             fwExceedIndex = 0;
        }
        else if (pRxData[4] == 0)
        {
             ui->textEdit_fwRcvInfo->insertPlainText(QString("对方拒绝升级/固件已是最新版"));
        }
    }
    if(pRxData[3] == 0xF2)
    {
        uint16_t u16FwPackIndex;
        *(uint16_t *)&u16FwPackIndex = *(uint16_t *)&pRxData[4];
        if(u16FwPackIndex == fwPackIndex)
        {
            if(pRxData[6] == 1)
            {
                fwExceedIndex = 0;
                 ui->textEdit_fwRcvInfo->insertPlainText(QString("第 ["));
                 ui->textEdit_fwRcvInfo->insertPlainText(QString::number(fwPackIndex));
                 ui->textEdit_fwRcvInfo->insertPlainText(QString("]  帧传输成功"));
                 fwExceedIndex = 0;
                 fwUpdateTimer->start(20);
                 fwUpdateTimeOut->start(2000);
            }
            else if (pRxData[6] == 0)
            {
                 ui->textEdit_fwRcvInfo->insertPlainText(QString("["));
                 ui->textEdit_fwRcvInfo->insertPlainText(QString::number(fwPackIndex));
                 ui->textEdit_fwRcvInfo->insertPlainText(QString("]  帧传输失败"));
            }
        }
    }
    if(pRxData[3] == 0xF3)
    {
        if(pRxData[4] == 1)
        {
            fwUpdateState = fwComplete;
            ui->textEdit_fwRcvInfo->insertPlainText(QString("对方回复升级成功"));
            fwExceedIndex = 0;
            fwUpdateTimeOut->stop();
        }
        else if (pRxData[4] == 0)
        {
             ui->textEdit_fwRcvInfo->insertPlainText(QString("对方回复升级失败"));
        }
    }
}
/*********************************固件升级相关函数*********************************/


/*********************************共用函数*********************************/
QByteArray MainWindow::loadTxMsg(uchar cmd1, uchar cmd2, QByteArray *pTxData)
{
    uchar preFix = 0xA5;
    uchar postFix = 0x5A;
    uchar len = pTxData->length() + 6;
    uchar crc = len + cmd1 + cmd2;
    QByteArray txProtocolMsg;
    txProtocolMsg.append(preFix);
    txProtocolMsg.append(len);
    txProtocolMsg.append(cmd1);
    txProtocolMsg.append(cmd2);

    uchar temp = 0;
    for(uchar i=0; i<pTxData->length(); i++)
    {
        temp = static_cast<uchar>(pTxData->at(i));
        crc += static_cast<uchar>(pTxData->at(i));
        txProtocolMsg.append(static_cast<uchar>(pTxData->at(i)));
    }
    txProtocolMsg.append(crc);
    txProtocolMsg.append(postFix);
    return txProtocolMsg;
}

uchar MainWindow::msgProcCrc(QByteArray *protocalData)
{
    uchar pRxData[100];
    memcpy(pRxData, protocalData->data(), protocalData->size());
    uchar i = 0;
    uchar crc = 0;
    uchar size = pRxData[1];

    if ((pRxData[0] == 0xA5) && (pRxData[size - 1] == 0x5A))
    {
        for (i = 1; i <= size - 3; i++)
        {
             crc += pRxData[i];
        }
        if (crc == pRxData[size - 2])
        {
             return 0;
        }
    }
    return -1;
}

QString MainWindow::ByteArrayToHexString(QByteArray &ba)
{
    QDataStream out(&ba,QIODevice::ReadWrite);   //将str的数据 读到out里面去
    QString buf;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每次一个字节的填充到 outchar
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
        buf += str;
    }
    return buf;
}

void MainWindow::delay_ms(int msec)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<msec);
}
/******************************************************************/
