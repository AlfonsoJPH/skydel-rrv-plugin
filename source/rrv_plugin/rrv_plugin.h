#pragma once

#include "skydel_plug_ins/skydel_plugin.h"
#include "../receiver_logic/serial_receiver.h"
#include "rrv_viewer.h"
#include "rrv_configuration.h"
#include <memory>

// Plugin implementation
class Rrv_Plugin : public QObject, public SkydelCoreInterface
{
  Q_OBJECT

public:
  // SkydelCoreInterface
  inline void setLogPath(const QString& path) override { config->logPath = path; }
  inline void setNotifier(SkydelNotifierInterface*) override {}
  inline void setConfiguration(const QString&, const QJsonObject&) override {}
  inline QJsonObject getConfiguration() const override { return {}; }
  SkydelWidgets createUI() override;
  void initialize() override;

  rrv_viewer* getView() const;


public slots:
  // Slots to update configuration
  void portNameChanged(const QString& name) { config->portName = name; }
  void baudRateChanged(int rate) { config->baudRate = rate; }
  void fileLoggingChanged(bool state) { 
    receiver->setFileLogging(state); 
    config->fileLogging = state;  
  }
  void logPathChanged(const QString& path) { config->logPath = path; }
  void logNetworkChanged(const QString& adddress, const QString& port) { 
    config->networkLogAddress = adddress; 
    config->networkLogPort = port; 
  }

private:
  RRVConfiguration *config;
  std::unique_ptr<SerialReceiver> receiver;
  rrv_viewer* view;
};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
