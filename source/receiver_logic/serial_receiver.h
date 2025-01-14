#pragma once

#include "receiver.h"
#include <QSerialPort>
#include <QObject>

class SerialReceiver : public Receiver {
    Q_OBJECT
public:
    SerialReceiver(const QString& portName, int baudRate, QObject* parent = nullptr, bool enabledFileLogging = false, QString fileLogPath = "");
    bool connectReceiver() override;
    void disconnectReceiver() override;
    QString getData() override;

public slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void receiverStateChanged() override;
    void portNameChanged(QString name) { m_portName = name; };
    void baudRateChanged(int rate) { m_baudRate = rate; };

private:
    QSerialPort m_serialPort;
    QString m_portName;
    int m_baudRate;
};