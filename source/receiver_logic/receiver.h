#pragma once

#include <QString>
#include <QObject>

class Receiver : public QObject {
    Q_OBJECT
public:
    explicit Receiver(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~Receiver() = default;
    virtual bool connectReceiver() = 0;
    virtual void disconnectReceiver() = 0;
    virtual QString getData() = 0;

signals:
    void dataReceived(const QString& data);
};


