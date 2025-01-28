#pragma once

#include "../receiver_logic/serial_receiver.h"
#include "position_logger.h"
#include "rrv_configuration.h"
#include "rrv_viewer.h"
#include "skydel_plug_ins/skydel_plugin.h"
#include <QSharedPointer>
#include <memory>

/**
 * @class Rrv_Plugin
 * @brief Main plugin class implementing SkydelCoreInterface and SkydelPositionObserverInterface.
 * 
 * This class initializes the plugin, creates the UI, manages the configuration and logging,
 * and handles the connection between the receiver and the simulator.
 */
class Rrv_Plugin : public QObject,
                   public SkydelCoreInterface,
                   public SkydelPositionObserverInterface {
  Q_OBJECT

public:
  // SkydelCoreInterface
  /**
   * @brief Sets the log path.
   * 
   * @param path The path where the log file will be saved.
   */
  inline void setLogPath(const QString &path) override {
    config->serialLogPath = path;
  }

  /**
   * @brief Sets the notifier.
   * 
   * @param notifier Pointer to the SkydelNotifierInterface.
   */
  inline void setNotifier(SkydelNotifierInterface *notifier) override {
    m_skydelNotifier = notifier;
  }

  /**
   * @brief Sets the configuration.
   * 
   * @param configName The name of the configuration.
   * @param config The configuration as a QJsonObject.
   */
  inline void setConfiguration(const QString &, const QJsonObject &) override {}

  /**
   * @brief Gets the configuration.
   * 
   * @return The configuration as a QJsonObject.
   */
  inline QJsonObject getConfiguration() const override { return {}; }

  /**
   * @brief Creates the UI for the plugin.
   * 
   * @return SkydelWidgets containing the created UI.
   */
  SkydelWidgets createUI() override;

  /**
   * @brief Initializes the plugin.
   */
  void initialize() override;

  /**
   * @brief Gets the view.
   * 
   * @return Pointer to the rrv_viewer.
   */
  rrv_viewer *getView() const;

  // SkydelPositionObserverInterface
  /**
   * @brief Creates a runtime position observer.
   * 
   * @return Pointer to the created SkydelRuntimePositionObserver.
   */
  inline SkydelRuntimePositionObserver *
  createRuntimePositionObserver() override {
    positionLogger = new PositionLogger(m_skydelNotifier, config,
                                        receiverPosition, simulationPosition);

    connect(this, &Rrv_Plugin::observerConfigChangedSignal, positionLogger, 
            &PositionLogger::configChanged);
    return positionLogger;
  }

public slots:
  void observerConfigChanged() { 
    emit observerConfigChangedSignal();
  }

signals:
  void observerConfigChangedSignal();

private:
  QSharedPointer<RRVConfiguration> config; ///< Shared pointer to the configuration object.
  std::unique_ptr<SerialReceiver> receiver; ///< Unique pointer to the SerialReceiver.
  rrv_viewer* view; ///< Pointer to the rrv_viewer.
  SkydelNotifierInterface* m_skydelNotifier; ///< Pointer to the SkydelNotifierInterface.
  PositionLogger* positionLogger; ///< Pointer to the PositionLogger.
  QSharedPointer<Sdx::Ecef> receiverPosition; ///< Shared pointer to the receiver's ECEF position.
  QSharedPointer<Sdx::Ecef> simulationPosition; ///< Shared pointer to the simulator's ECEF position.
};

REGISTER_SKYDEL_PLUGIN(Rrv_Plugin)
