#pragma once

#include "skydel_plug_ins/skydel_plugin.h"
#include "../receiver_logic/serial_receiver.h"
#include "rrv_viewer.h"
#include "rrv_configuration.h"
#include "position_logger.h"
#include <memory>

// Plugin implementation
class Rrv_Plugin : public QObject, public SkydelCoreInterface, public SkydelPositionObserverInterface
{
  Q_OBJECT

public:
  // SkydelCoreInterface
  inline void setLogPath(const QString& path) override { config->serialLogPath = path; }
  inline void setNotifier(SkydelNotifierInterface* notifier) override { m_skydelNotifier = notifier; }
  inline void setConfiguration(const QString&, const QJsonObject&) override {}
  inline QJsonObject getConfiguration() const override { return {}; }
  SkydelWidgets createUI() override;
  void initialize() override;

  rrv_viewer* getView() const;

  // SkydelPositionObserverInterface
  inline SkydelRuntimePositionObserver* createRuntimePositionObserver() override
  {
    positionLogger = new PositionLogger(m_skydelNotifier,
                                        QHostAddress(config->simulationNetworkLogAddress),
                                        config->simulationNetworkLogPort.toUShort(),
                                        config->simulationNetworkLogging,
                                        config->simulationFileLogging,
                                        config->simulationLogPath,
                                        QHostAddress(config->receiverNetworkLogAddress),
                                        config->receiverNetworkLogPort.toUShort(),
                                        config->receiverNetworkLogging,
                                        config->receiverFileLogging,
                                        config->receiverLogPath);

    //Connect observers to view
    connect(positionLogger, &PositionLogger::updatePosition, this, &Rrv_Plugin::observerDataReceived);
    connect(this, &Rrv_Plugin::updatedReceiverPositionSignal, positionLogger, &PositionLogger::updateReceiverPosition);

    //Connect configuration to observers
    connect(this, &Rrv_Plugin::simulationFileLoggingChangedSignal, positionLogger, &PositionLogger::setSimulationFileLogging);
    connect(this, &Rrv_Plugin::simulationLogPathChangedSignal, positionLogger, &PositionLogger::setSimulationLogPath);
    connect(this, &Rrv_Plugin::simulationNetworkLoggingChangedSignal, positionLogger, &PositionLogger::setSimulationNetworkLogging);
    connect(this, &Rrv_Plugin::simulationLogNetworkChangedSignal, positionLogger, &PositionLogger::setSimulationLogNetwork);

    connect(this, &Rrv_Plugin::receiverFileLoggingChangedSignal, positionLogger, &PositionLogger::setReceiverFileLogging);
    connect(this, &Rrv_Plugin::receiverLogPathChangedSignal, positionLogger, &PositionLogger::setReceiverLogPath);
    connect(this, &Rrv_Plugin::receiverNetworkLoggingChangedSignal, positionLogger, &PositionLogger::setReceiverNetworkLogging);
    connect(this, &Rrv_Plugin::receiverLogNetworkChangedSignal, positionLogger, &PositionLogger::setReceiverLogNetwork);

    return positionLogger;
  }

public slots:
  // Slots to update configuration
  // May refactor to reduce slots, serial logging anad observer logging  right now are being forwarded
  void portNameChanged(const QString& name) { config->serialPortName = name; }
  void baudRateChanged(int rate) { config->baudRate = rate; }
  void serialFileLoggingChanged(bool state) { config->serialFileLogging = state; }
  void serialLogPathChanged(const QString& path) { config->serialLogPath = path; }
  void serialLogNetworkChanged(const QString& adddress, const QString& port) { 
    config->serialNetworkLogAddress = adddress; 
    config->serialNetworkLogPort = port; 
  }

  void simulationFileLoggingChanged(bool state) { 
    config->simulationFileLogging = state; 
    emit simulationFileLoggingChangedSignal(state);
  }
  void simulationLogPathChanged(const QString& path) { 
    config->simulationLogPath = path; 
    emit simulationLogPathChangedSignal(path);
  }
  void simulationNetworkLoggingChanged(bool state) { 
    config->simulationNetworkLogging = state; 
    emit simulationNetworkLoggingChangedSignal(state);
  }
  void simulationLogNetworkChanged(const QString& adddress, const QString& port) { 
    config->simulationNetworkLogAddress = adddress; 
    config->simulationNetworkLogPort = port; 
    emit simulationLogNetworkChangedSignal(adddress, port);
  }

  void receiverFileLoggingChanged(bool state) { 
    config->receiverFileLogging = state; 
    emit receiverFileLoggingChangedSignal(state); 
  }
  void receiverLogPathChanged(const QString& path) { 
    config->receiverLogPath = path; 
    emit receiverLogPathChangedSignal(path);
  }
  void receiverNetworkLoggingChanged(bool state) { 
    config->receiverNetworkLogging = state; 
    emit receiverNetworkLoggingChangedSignal(state);
  }
  void receiverLogNetworkChanged(const QString& adddress, const QString& port) { 
    config->receiverNetworkLogAddress = adddress; 
    config->receiverNetworkLogPort = port;
    emit receiverLogNetworkChangedSignal(adddress, port);
  }

  void observerDataReceived(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p) { 
    emit observerPosition(simulation_p, receiver_p); 
  }
  void updatedReceiverPosition(const Sdx::Ecef& receiver_p) { 
    emit updatedReceiverPositionSignal(receiver_p); 
  }

signals: // bypass viewer copies using redirection of signals
  void simulationFileLoggingChangedSignal(bool state);
  void simulationLogPathChangedSignal(const QString& path);
  void simulationNetworkLoggingChangedSignal(bool state);
  void simulationLogNetworkChangedSignal(const QString& address, const QString& port);

  void receiverFileLoggingChangedSignal(bool state);
  void receiverLogPathChangedSignal(const QString& path);
  void receiverNetworkLoggingChangedSignal(bool state);
  void receiverLogNetworkChangedSignal(const QString& address, const QString& port);

  void observerPosition(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p);
  void updatedReceiverPositionSignal(const Sdx::Ecef& receiver_p);

private:
  RRVConfiguration *config;
  std::unique_ptr<SerialReceiver> receiver;
  rrv_viewer* view;
  SkydelNotifierInterface* m_skydelNotifier;
  PositionLogger *positionLogger;
};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
