#include "ncombobox.h"


NComboBox::NComboBox(QWidget *parent):QComboBox(parent)
{

}
NComboBox::~NComboBox()
{

}

void NComboBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QComboBox::mousePressEvent(event);
}
