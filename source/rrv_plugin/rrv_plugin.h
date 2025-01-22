#pragma once

#include "skydel_plug_ins/skydel_plugin.h"
#include "../receiver_logic/serial_receiver.h"
#include "rrv_viewer.h"
#include "rrv_configuration.h"
#include "position_logger.h"
#include <memory>
#include <QSharedPointer>

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
                                        config);

    //Connect observers to view
    connect(positionLogger, &PositionLogger::updatePosition, this, &Rrv_Plugin::observerDataReceived);
    connect(this, &Rrv_Plugin::updatedReceiverPositionSignal, positionLogger, &PositionLogger::updateReceiverPosition);

    //Connect configuration to observers
    connect(this, &Rrv_Plugin::observerConfigChangedSignal, positionLogger, &PositionLogger::configChanged);

    return positionLogger;
  }

public slots:
  // Slots to update configuration
  void observerConfigChanged() { 
    emit observerConfigChangedSignal();
  }

  void observerDataReceived(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p) { 
    emit observerPosition(simulation_p, receiver_p); 
  }
  void updatedReceiverPosition(const Sdx::Ecef& receiver_p) { 
    emit updatedReceiverPositionSignal(receiver_p); 
  }

signals: // bypass viewer copies using redirection of signals
  void observerConfigChangedSignal();

  void observerPosition(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p);
  void updatedReceiverPositionSignal(const Sdx::Ecef& receiver_p);

private:
  QSharedPointer<RRVConfiguration> config;
  std::unique_ptr<SerialReceiver> receiver;
  rrv_viewer* view;
  SkydelNotifierInterface* m_skydelNotifier;
  PositionLogger *positionLogger;
};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
