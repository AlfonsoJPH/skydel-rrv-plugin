#include "receiver.h"
#include <QDir>
#include <iostream>
#include <QTextStream>

Receiver::Receiver(QObject* parent, bool enabledFileLogging,
                   QString fileLogPath, bool enabledNetworkLogging,
                   QString networkLogAddress, QString networkLogPort) :
                   QObject(parent), enabledFileLogging(enabledFileLogging), 
                   fileLogPath(fileLogPath), fileLog(fileLogPath + QDir::separator() + "rrv.log"),
                   enabledNetworkLogging(enabledNetworkLogging)
                  {
    state = false;
    this->networkLogAddress = QHostAddress(networkLogAddress);
    udpSocketLogging = new QUdpSocket(this);
    if (!udpSocketLogging->bind()) {
        qWarning() << "Failed to bind UDP socket";
        dataReceived("Invalid network log address");
    }
}

// Dump logs messages into log file
void Receiver::fileLogData(const QString& data)
{
  if (enabledFileLogging)
  {
    if (!fileLog.isOpen())
    {
      if (!fileLog.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
      {
        qWarning() << "Failed to open log file";
        return;
      }
    }

    QTextStream out(&fileLog);
    out << data;
  }
}

void Receiver::networkLogData(const QString& data)
{
    QByteArray byteArray;
    byteArray.append(data.toUtf8());
    if (udpSocketLogging->writeDatagram(byteArray, networkLogAddress, networkLogPort) == -1)
    {
      emit dataReceived("Failed to send data to network log");
      throw std::runtime_error(udpSocketLogging->errorString().toStdString().c_str());
    }

}