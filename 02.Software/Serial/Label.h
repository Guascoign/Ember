#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QSerialPort>
#include <QTimer>
#include <QChartView>
#include <QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class Label; }
QT_END_NAMESPACE

class Label : public QWidget
{
    Q_OBJECT

public:
    Label(QWidget *parent = nullptr);
    ~Label();

private slots:
    void on_Serialdata_readytoread();
    void updateChart();

private:
    Ui::Label *ui;
    QSerialPort *SerialPort;
    QTimer *chartUpdateTimer;
    QtCharts::QChartView *chartView;
    QtCharts::QLineSeries *series;
    bool recording;
    int xValue;
};

#endif // LABEL_H
