#include "serial_receiver.h"
#include <QDebug>
#include <QFile>

bool SerialReceiver::connectReceiver() {
  m_serialPort.setPortName(config->serialPortName);
  m_serialPort.setBaudRate(config->baudRate);
  if (!m_serialPort.open(QIODevice::ReadWrite)) {
    logData(QString("ERROR: Failed to open port %1, error: %2")
                .arg(config->serialPortName)
                .arg(m_serialPort.errorString()));
    qWarning() << "Failed to open port" << config->baudRate
               << ", error:" << m_serialPort.errorString();
    return false;
  }
  logData(QString("INFO: Connected to port %1").arg(config->baudRate));
  emit receiverStateChanges(true);

  return true;
}

void SerialReceiver::disconnectReceiver() {
  if (m_serialPort.isOpen()) {
    logData(QString("INFO: Disconnected from port %1").arg(config->baudRate));
    m_serialPort.close();

  } else {
    logData(
        QString("ERROR: Tried to disconnect when receiver is not connected."));
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
    logData(QString("ERROR: Critical error: %1\nDISCONNECTING FROM RECEIVER")
                .arg(m_serialPort.errorString()));
    disconnectReceiver();
  }
}

// Handles when the receiver state changes
void SerialReceiver::receiverStateChanged() {
  if (m_serialPort.isOpen()) {
    disconnectReceiver();
  } else {
    connectReceiver();
  }
}