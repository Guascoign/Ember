#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>      // 提供系统中存在的串口信息
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QFileDevice>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QVector>
#include <QValueAxis>
#include <QDebug>
#include <QSplineSeries>
#include <QElapsedTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT




public:
/*********************************串口基本设置*********************************/
    int rcvPackets = 0;
    int sendPackets = 0;

/*********************************协议收发*********************************/
    int tableRowCnt = 0;
    //用于表格设置
    enum FieldColNum {
        colTime,
        colCmd1,
        colCmd2,
        colData,
    };
    enum CellType {
        ctTime,
        ctCmd1,
        ctCmd2,
        ctData,
    };

/*********************************波形显示*********************************/
    float receiveParsingData;    
    int showPeriod = 0;
    QVector<QString> waveArray;

/*********************************pid调试*********************************/
    float pidRcvTarData;
    float pidRcvActData;

/*********************************固件升级*********************************/
    enum fwUpdateStateEnum {
        fwInit,
        fwStart,
        fwNum,
        fwStop,
        fwTransfer,
        fwComplete,
    };
    QByteArray binRawData;
    int fwFileLen;
    char fwUpdateState;
    int fwPackLength = 16;
    int fwPackNum = 0;
    uint16_t fwPackIndex = 0;
    uchar fwExceedFlag;
    uchar fwExceedTimes = 3;
    uchar fwExceedIndex;
    QTimer *fwUpdateTimer = new QTimer();
    QTimer *fwUpdateTimeOut = new QTimer();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*********************************串口基本设置相关函数*********************************/
    void    serialPortInit();      //串口初始化
    void    windowInit();          //显示窗口初始化
    void    refreshCom();          //刷新串口
    void    rxTxNumLcdInit();      //发送与接收数量lcd初始化
/******************************************************************/

/*********************************协议收发相关函数*********************************/
    void    createItemsARow(int rowNum, QByteArray *protocalData); //表格新建一行
    void    createChart();  //创建图表
    void    tableInit();           //表格初始化
/******************************************************************/

/*********************************电压波形显示相关函数*********************************/
    void    iniBarChart();          //柱状图初始化  
    void    parsingData(QByteArray *protocalData);
    void    lcdVoltageInit();      //电压值lcd初始化
/******************************************************************/

/*********************************pid调参相关函数*********************************/
    void        pidParsingData(QByteArray *protocalData);
    QByteArray  protocalDataSet(uchar cmd, double doubleData);
/******************************************************************/

/*********************************固件升级相关函数*********************************/
    void        fwUpdateInit();
    QByteArray loadFwPackData(QByteArray *protocalData);    
    void        fwStartCmd();
    void        fwNumCmd();
/******************************************************************/

/*********************************共用函数*********************************/
QByteArray loadTxMsg(uchar cmd1, uchar cmd2, QByteArray *pTxData);
void delay_ms(int msec);
uchar msgProcCrc(QByteArray *protocalData);
QString ByteArrayToHexString(QByteArray &ba);
/******************************************************************/

private:
    Ui::MainWindow *ui;
    QSerialPort* serialPort;

public slots:
    void comboBox_buadClicked(); //comboBox被点击

private slots:
/*********************************串口基本设置相关函数*********************************/
    void sendData();            //发送串口数据
    void receiveData();         //接收串口数据
    void openSerialport();      //串口开启
    void closeSerialport();     //串口关闭
    void setBuad(int);          //设置波特率
    void clearRcv();            //清楚接收缓存
    void handleSerialError(QSerialPort::SerialPortError serialPortErr); //串口异常捕获
    void lcdShowRcvPacketNum();     //lcd显示接收帧数
    void lcdShowSendPacketNum();    //lcd显示发送帧数
    void rcvDataOutputToFile();     //串口输出内容输出到文件
/******************************************************************/

/*********************************协议收发相关函数*********************************/
    void on_btnConvert_clicked();   //转换按钮被点击
    void on_btnClear_clicked();     //清除转化的按钮被点击
    void sendProtocalHexData();     //以hex格式发送串口数据
/******************************************************************/

/*********************************电压波形显示相关函数*********************************/
    void waveDataOutputToFile();
    void drawLine();
    void on_actZoomIn_triggered();
    void on_actZoomOut_triggered();
    void on_actZoomReset_triggered();
/******************************************************************/

/*********************************pid调参相关函数*********************************/
    void showPidWave();
    void pidInit();
    void pidKpSet(double sendKp);
    void pidKiSet(double sendKi);
    void pidKdSet(double sendKd);
    void pidTarSet(double sendTar);
/******************************************************************/

/*********************************固件升级相关函数*********************************/
    void readFw();
    void startFwUpdate();
    void stopFwUpdate();
    void rcvFwReply(QByteArray *protocalData);
/******************************************************************/

};

#endif // MAINWINDOW_H
