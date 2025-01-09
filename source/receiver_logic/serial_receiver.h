#pragma once

#include "receiver.h"
#include <QSerialPort>
#include <QObject>

class SerialReceiver : public Receiver {
    Q_OBJECT
public:
    SerialReceiver(const QString& portName, int baudRate, QObject* parent = nullptr);
    bool connectReceiver() override;
    void disconnectReceiver() override;
    QString getData() override;

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void dataReceived(const QString& data);

private:
    QSerialPort m_serialPort;
    QByteArray m_readData;
    QString m_portName;
    int m_baudRate;
};