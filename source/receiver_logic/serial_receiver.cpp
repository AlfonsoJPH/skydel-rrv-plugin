#include "serial_receiver.h"
#include <QDebug>
#include <QFile>

SerialReceiver::SerialReceiver(const QString& portName, int baudRate, QObject* parent, bool enabledFileLogging, QString fileLogPath)
    : Receiver(parent, enabledFileLogging, fileLogPath), m_portName(portName), m_baudRate(baudRate) {}

bool SerialReceiver::connectReceiver() {
    m_serialPort.setPortName(m_portName);
    m_serialPort.setBaudRate(m_baudRate);
    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open port" << m_portName << ", error:" << m_serialPort.errorString();
        fileLogData(QString("ERROR: Failed to open port %1, error: %2").arg(m_portName).arg(m_serialPort.errorString()));
        return false;
    }
    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialReceiver::handleReadyRead);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SerialReceiver::handleError);
    return true;
}

void SerialReceiver::disconnectReceiver() {
    if (m_serialPort.isOpen()) {
        fileLogData(QString("INFO: Disconnected from port %1").arg(m_portName));
        m_serialPort.close();
    } else {
        fileLogData(QString("ERROR: Tried to disconnect when receiver is not connected."));
    }
}

QString SerialReceiver::getData() {
    return QString::fromUtf8(m_readData);
}

void SerialReceiver::handleReadyRead() {
    m_readData.append(m_serialPort.readAll());
    emit dataReceived(QString::fromUtf8(m_readData));
    fileLogData(QString("INFO: Data received: %1").arg(QString::fromUtf8(m_readData)));
}

void SerialReceiver::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        qWarning() << "Critical Error:" << m_serialPort.errorString();
        fileLogData(QString("ERROR: Critical error: %1\nDISCONNECTING FROM RECEIVER").arg(m_serialPort.errorString()));
        disconnectReceiver();
    }
}