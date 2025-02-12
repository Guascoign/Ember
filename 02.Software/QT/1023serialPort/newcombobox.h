#ifndef NEWCOMBOBOX_H
#define NEWCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QMouseEvent>

class NewComboBox : public QComboBox
{
    Q_OBJECT
public:
    NewComboBox(QWidget*parent=0);
    void mousePressEvent(QMouseEvent *event);
    ~NewComboBox();
signals:
    void clicked();
};

#endif // NEWCOMBOBOX_H
