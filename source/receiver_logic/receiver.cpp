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

  fileLogData("INFO: Receiver initialized");
}

void Receiver::fileLogData(const QString& data)
{
  if (enabledFileLogging)
  {
    QTextStream out(&fileLog);
    out << data << "\n";
  }
}