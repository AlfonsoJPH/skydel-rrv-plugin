#include "receiver.h"
#include <QDir>
#include <QTextStream>
#include <qDebug>

Receiver::Receiver(QObject* parent, bool enabledFileLogging,
                   QString fileLogPath) :
                   QObject(parent), enabledFileLogging(enabledFileLogging), 
                   fileLogPath(fileLogPath), fileLog(fileLogPath + QDir::separator() + "rrv.log") {
  if (enabledFileLogging)
  {
    if (!fileLog.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      throw std::runtime_error("Failed to open log file");
    }
  }
  state = false;

  fileLogData("INFO: Receiver initialized");
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
    out << data << "\n";
  }
}