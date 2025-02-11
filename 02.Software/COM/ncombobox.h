#ifndef NCOMBOBOX_H
#define NCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QMouseEvent>
//重写QComboBox实现点击下拉框触发信号
class NComboBox : public QComboBox
{
    Q_OBJECT
public:
    NComboBox(QWidget*parent=0);
    void mousePressEvent(QMouseEvent *event);
    ~NComboBox();
signals:
    void clicked();
};

#endif // NCOMBOBOX_H
