#ifndef THREAD_H
#define THREAD_H

#include <QObject>

class Thread : public QObject
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

signals:
};

#endif // THREAD_H
