#include "serial_receiver.h"
#include <QDebug>
#include <QFile>

SerialReceiver::SerialReceiver(const QString& portName, int baudRate, QObject* parent, 
                               bool enabledFileLogging, QString fileLogPath, 
                               bool enabledNetworkLogging, QString networkLogAddress, 
                               QString networkLogPort)
                               : Receiver(parent, enabledFileLogging, fileLogPath, 
                                          enabledNetworkLogging, networkLogAddress, 
                                          networkLogPort), 
                               m_portName(portName), m_baudRate(baudRate) {
        connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialReceiver::handleReadyRead);
        connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SerialReceiver::handleError);

    }

bool SerialReceiver::connectReceiver() {
    m_serialPort.setPortName(m_portName);
    m_serialPort.setBaudRate(m_baudRate);
    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        logData(QString("ERROR: Failed to open port %1, error: %2").arg(m_portName).arg(m_serialPort.errorString()));
        qWarning() << "Failed to open port" << m_portName << ", error:" << m_serialPort.errorString();
        return false;
    }
    logData(QString("INFO: Connected to port %1").arg(m_portName));
    emit receiverStateChanges(true);

    return true;
}

void SerialReceiver::disconnectReceiver() {
    if (m_serialPort.isOpen()) {
        logData(QString("INFO: Disconnected from port %1").arg(m_portName));
        m_serialPort.close();

    } else {
        logData(QString("ERROR: Tried to disconnect when receiver is not connected."));
    }
    m_serialPort.setPortName("");
    m_serialPort.setBaudRate(QSerialPort::UnknownBaud);
    emit receiverStateChanges(false);
}

QString SerialReceiver::getData() {
    return QString::fromUtf8(m_serialPort.readAll());
}

// Handles when data is ready to be read
void SerialReceiver::handleReadyRead() {
    QString data = getData();
    emit dataReceived(data);
    logData(QString("INFO: Data received: \n%1").arg(data));
}

// Handles when an error occurs
void SerialReceiver::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        qWarning() << "Critical Error:" << m_serialPort.errorString();
        logData(QString("ERROR: Critical error: %1\nDISCONNECTING FROM RECEIVER").arg(m_serialPort.errorString()));
        disconnectReceiver();
    }
}

// Handles when the receiver state changes
void SerialReceiver::receiverStateChanged() {
    if (m_serialPort.isOpen()) {
        disconnectReceiver();
    } else {
        connectReceiver();
        emit receiverStateChanges(true);
    }
}