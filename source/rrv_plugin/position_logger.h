#pragma once

#include <QFile>
#include <QtNetwork/QUdpSocket>
#include <QString>
#include "ecef.h"

#include "skydel_plug_ins/skydel_plugin.h"
// May have to change init and give him the view since it can be created and destroyed with simulations
// Runtime position observer implementation
class PositionLogger : public SkydelRuntimePositionObserver
{
  Q_OBJECT
public:
  explicit PositionLogger(SkydelNotifierInterface* skydelNotifier,
                          const QHostAddress& simulationAddress,
                          uint16_t simulationPort,
                          bool enableSimulationNetworkLogging,
                          bool enableSimulationFileLogging,
                          const QString& simulationLogPath,

                          const QHostAddress& receiverAddress,
                          uint16_t receiverPort,
                          bool enableReceiverNetworkLogging,
                          bool enableReceiverFileLogging,
                          const QString& receiverLogPath);// Called at simulation initialization phase
  ~PositionLogger();                               // Called at simluation ending phase
  void pushPosition(
    const SkydelRuntimePositionObserver::TimedPosition&) override; // Called at simluation running phase at 1000 Hz
  void connectToView(QWidget*) override;

signals:
  void updatePosition(const Sdx::Ecef& simulator_p, const Sdx::Ecef& receiver_p);
public slots:
  void updateReceiverPosition(const Sdx::Ecef& receiver_p) { 
    m_lastReceiverPosition.x = receiver_p.x; 
    m_lastReceiverPosition.y = receiver_p.y;
    m_lastReceiverPosition.z = receiver_p.z; 
  }

  void setSimulationFileLogging(bool state) { m_enableSimulationFileLogging = state; }
  void setSimulationLogPath(const QString& path) { m_simulationLogPath = path; }
  void setSimulationNetworkLogging(bool state) { m_enableSimulationNetworkLogging = state; }
  void setSimulationLogNetwork(const QString& address, const QString& port) { 
    m_simulationAddress = QHostAddress(address); 
    m_simulationPort = port.toUShort();
  }

  void setReceiverFileLogging(bool state) { m_enableReceiverFileLogging = state; }
  void setReceiverLogPath(const QString& path) { m_receiverLogPath = path; }
  void setReceiverNetworkLogging(bool state) { m_enableReceiverNetworkLogging = state; }
  void setReceiverLogNetwork(const QString& address, const QString& port) { 
    m_receiverAddress = QHostAddress(address); 
    m_receiverPort = port.toUShort();
  }

private:
  QString toString(const Sdx::Ecef& p);

  SkydelNotifierInterface* m_skydelNotifier;

  bool m_enableSimulationFileLogging;
  bool m_enableSimulationNetworkLogging;
  QHostAddress m_simulationAddress;
  uint16_t m_simulationPort;
  QString m_simulationLogPath;
  QFile m_simulationFile;
  QUdpSocket m_simulationSocket;

  bool m_enableReceiverFileLogging;
  bool m_enableReceiverNetworkLogging;
  QHostAddress m_receiverAddress;
  uint16_t m_receiverPort;
  QString m_receiverLogPath;
  QFile m_receiverFile;
  QUdpSocket m_receiverSocket;
  
  Sdx::Ecef m_lastReceiverPosition;
};
