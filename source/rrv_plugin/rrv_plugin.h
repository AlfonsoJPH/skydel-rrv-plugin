#pragma once

#include "skydel_plug_ins/skydel_plugin.h"
#include "../receiver_logic/serial_receiver.h"
#include "rrv_viewer.h"
#include <memory>

// Plugin implementation
class Rrv_Plugin : public QObject, public SkydelCoreInterface
{
  Q_OBJECT

public:
  // SkydelCoreInterface
  inline void setLogPath(const QString& path) override { logPath = path; }
  inline void setNotifier(SkydelNotifierInterface*) override {}
  inline void setConfiguration(const QString&, const QJsonObject&) override {}
  inline QJsonObject getConfiguration() const override { return {}; }
  SkydelWidgets createUI() override;
  void initialize() override;

private:
  QString logPath;
  std::unique_ptr<SerialReceiver> receiver;

};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
