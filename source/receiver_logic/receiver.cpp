#include "receiver.h"
#include <QDir>
#include <iostream>
#include <QTextStream>

Receiver::Receiver(QObject* parent, QSharedPointer<RRVConfiguration> config) :
                   QObject(parent), fileLog(config->receiverLogPath + QDir::separator() + "rrv.log")
                  {
    state = false;
    this->config = config;
}

// Dump logs messages into log file
void Receiver::fileLogData(const QString& data)
{
  if (config->serialFileLogging)
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
    if (udpSocketLogging.writeDatagram(byteArray, config->serialNetworkLogAddress, config->serialNetworkLogPort) == -1)
    {
       emit dataReceived(udpSocketLogging.errorString().toStdString().c_str());
    }

}