#pragma once

#include "receiver.h"
#include <QSerialPort>
#include <QObject>
#include "../rrv_plugin/rrv_configuration.h"

class SerialReceiver : public Receiver {
    Q_OBJECT
public:
    SerialReceiver(QObject* parent = nullptr, QSharedPointer<RRVConfiguration> config = nullptr) : Receiver(parent, config) {
        connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialReceiver::handleReadyRead);
        connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SerialReceiver::handleError);
    };
    bool connectReceiver() override;
    void disconnectReceiver() override;
    QString getData() override;

public slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void receiverStateChanged() override;

private:
    QSerialPort m_serialPort;
    QSharedPointer<RRVConfiguration> config;    
};