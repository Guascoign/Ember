#include "newcombobox.h"

NewComboBox::NewComboBox(QWidget *parent):QComboBox(parent)
{

}
NewComboBox::~NewComboBox()
{

}

void NewComboBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QComboBox::mousePressEvent(event);
}
