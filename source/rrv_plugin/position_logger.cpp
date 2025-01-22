#include "position_logger.h"

#include <QDir>
#include <QTextStream>

#include <stdexcept>


PositionLogger::PositionLogger(SkydelNotifierInterface* skydelNotifier,
                               QSharedPointer<RRVConfiguration> config) :
                                m_skydelNotifier(skydelNotifier),
                                config(config),
                                m_simulationFile(config->simulationLogPath + QDir::separator() + "simulation_position_observer_output.csv"),

                                m_receiverFile(config->receiverLogPath + QDir::separator() + "receiver_position_observer_output.csv")
{
  if (config->simulationFileLogging)
  {
    if (!m_simulationFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      throw std::runtime_error("SimulationPositionLogger - Can't open file.");
    }
  }

  if (config->receiverFileLogging)
  {
    if (!m_receiverFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      throw std::runtime_error("ReceiverPositionLogger - Can't open file.");
    }
  }


  m_lastReceiverPosition = Sdx::Ecef(0,0,0);
  m_skydelNotifier->notify("PositionLogger initialized");
}

PositionLogger::~PositionLogger()
{
}

void PositionLogger::pushPosition(const TimedPosition& p)
{
  Sdx::Ecef simulation_p = Sdx::Ecef(p.position.x, p.position.y, p.position.z);
  
  QString formatedSimulationPosition = QString::number(p.time) + ",Simulation," + toString(simulation_p);
  QString formatedReceiverPosition = QString::number(p.time) + ",Receiver," + toString(m_lastReceiverPosition);
  if ((p.time % 1000) == 0)
  {
  emit updatePosition(simulation_p, m_lastReceiverPosition);
  }

  if (config->simulationFileLogging)
  {
    QByteArray byteArray;
    byteArray.append(formatedSimulationPosition.toUtf8());
    if (m_simulationSocket.writeDatagram(byteArray, config->simulationNetworkLogAddress, config->simulationNetworkLogPort) == -1)
    {
      throw std::runtime_error(m_simulationSocket.errorString().toStdString().c_str());
    }
  }

  if (config->simulationFileLogging)
  {
    if (!m_simulationFile.isOpen())
    {
      if (!m_simulationFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
      {
        throw std::runtime_error("PositionLogger - Can't open simulation file.");
      }
    }
    QTextStream stream(&m_simulationFile);
    stream << formatedSimulationPosition;

    if (stream.status() == QTextStream::WriteFailed)
    {
      throw std::runtime_error("PositionLogger - Failed to write in simulation file.");
    }
  }

  if (config->receiverNetworkLogging)
  {
    QByteArray byteArray;
    byteArray.append(formatedReceiverPosition.toUtf8());
    if (m_receiverSocket.writeDatagram(byteArray, config->receiverNetworkLogAddress, config->receiverNetworkLogPort) == -1)
    {
      throw std::runtime_error(m_receiverSocket.errorString().toStdString().c_str());
    }
  }

  if (config->receiverFileLogging)
  {
    if (!m_receiverFile.isOpen())
    {
      if (!m_receiverFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
      {
        throw std::runtime_error("PositionLogger - Can't open receiver file.");
      }
    }
    QTextStream stream(&m_receiverFile);
    stream << formatedReceiverPosition;

    if (stream.status() == QTextStream::WriteFailed)
    {
      throw std::runtime_error("PositionLogger - Failed to write in receiver file.");
    }
  }


}

void PositionLogger::connectToView(QWidget* view)
{
}

QString PositionLogger::toString(const Sdx::Ecef& p)
{
  QString formattedData;
  QTextStream stream(&formattedData);
  stream.setRealNumberPrecision(15);
  stream << p.x << ',' << p.y << ',' << p.z;

  return formattedData;
}
