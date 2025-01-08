#pragma once

#include "skydel_plug_ins/skydel_plugin.h"

// Plugin implementation
class Rrv_Plugin : public QObject, public SkydelCoreInterface
{
  Q_OBJECT

public:
  // SkydelCoreInterface
  inline void setLogPath(const QString&) override {}
  inline void setNotifier(SkydelNotifierInterface*) override {}
  inline void setConfiguration(const QString&, const QJsonObject&) override {}
  inline QJsonObject getConfiguration() const override { return {}; }
  inline SkydelWidgets createUI() override { return {}; }
  inline void initialize() override {}
};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
