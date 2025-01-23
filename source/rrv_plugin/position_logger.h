#pragma once

#include <QFile>
#include <QtNetwork/QUdpSocket>
#include <QString>
#include <QSharedPointer>
#include <QDir>

#include "ecef.h"
#include "skydel_plug_ins/skydel_plugin.h"

#include "rrv_configuration.h"

// May have to change init and give him the view since it can be created and destroyed with simulations
// Runtime position observer implementation
class PositionLogger : public SkydelRuntimePositionObserver
{
  Q_OBJECT
public:
  explicit PositionLogger(SkydelNotifierInterface* skydelNotifier,
                          QSharedPointer<RRVConfiguration> config,
                          QSharedPointer<Sdx::Ecef> receiverPosition,
                          QSharedPointer<Sdx::Ecef> simulationPosition);// Called at simulation initialization phase
  ~PositionLogger();                               // Called at simluation ending phase
  void pushPosition(
    const SkydelRuntimePositionObserver::TimedPosition&) override; // Called at simluation running phase at 1000 Hz
  void connectToView(QWidget*) override;

public slots:


  void configChanged() {
    m_receiverFile.setFileName(config->receiverLogPath + QDir::separator() + "receiver_position_observer_output.csv");
    m_simulationFile.setFileName(config->simulationLogPath + QDir::separator() + "simulation_position_observer_output.csv");
  }


private:
  QString toString(const Sdx::Ecef& p);

  SkydelNotifierInterface* m_skydelNotifier;
  QSharedPointer<RRVConfiguration> config;
  QSharedPointer<Sdx::Ecef> receiverPosition;
  QSharedPointer<Sdx::Ecef> simulationPosition;
  
  QFile m_simulationFile;
  QUdpSocket m_simulationSocket;

  QFile m_receiverFile;
  QUdpSocket m_receiverSocket;
  
};
