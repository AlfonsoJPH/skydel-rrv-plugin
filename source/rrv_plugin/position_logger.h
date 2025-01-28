#pragma once

#include <QDir>
#include <QFile>
#include <QSharedPointer>
#include <QString>
#include <QtNetwork/QUdpSocket>

#include "ecef.h"
#include "skydel_plug_ins/skydel_plugin.h"
#include "rrv_configuration.h"
#include "rrv_viewer.h"
/**
 * @class PositionLogger
 * @brief Implements the SkydelRuntimePositionObserver interface to log positions.
 * 
 * This class is responsible for logging the positions of the receiver and the simulator
 * during the simulation. It logs the data to a file and/or sends it over the network
 * based on the configuration settings. Also, it connects to the view to display the
 * simulated position on Ecef format.
 */
class PositionLogger : public SkydelRuntimePositionObserver {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the PositionLogger class.
   * 
   * @param skydelNotifier Pointer to the SkydelNotifierInterface.
   * @param config Shared pointer to the RRVConfiguration object.
   * @param receiverPosition Shared pointer to the receiver's ECEF position.
   * @param simulationPosition Shared pointer to the simulator's ECEF position.
   */
  explicit PositionLogger(
      SkydelNotifierInterface *skydelNotifier,
      QSharedPointer<RRVConfiguration> config,
      QSharedPointer<Sdx::Ecef> receiverPosition,
      QSharedPointer<Sdx::Ecef> simulationPosition);

  /**
   * @brief Destructor for the PositionLogger class.
   */
  ~PositionLogger();

  /**
   * @brief Gets the realtime simulation position data and pushes simulated and receiver position to the logger.
   * 
   * This function is called at the simulation running phase at 1000 Hz to log the position data.
   * 
   * @param position The timed position data to be logged (ecef and time data, as well as orientation, but only position and time is used).
   */
  void pushPosition(const SkydelRuntimePositionObserver::TimedPosition &) override;

  /**
   * @brief Connects the logger to the view.
   * 
   * This function connects the logger to the specified view.
   * 
   * @param view Pointer to the QWidget representing the view.
   */
  void connectToView(QWidget *) override;

  /**
   * @brief Converts an ECEF position to a formatted string.
   * 
   * This function converts an ECEF position to a formatted string for logging.
   * 
   * @param p The ECEF position to convert.
   * @return QString The formatted string.
   */
  QString toString(const Sdx::Ecef& p);

public slots:
  /**
   * @brief Slot to handle configuration changes.
   * 
   * This slot updates the file name for logging based on the new configuration settings.
   */
  void configChanged() {
    m_receiverFile.setFileName(config->receiverLogPath + QDir::separator() +
                               "receiver_position_observer_output.csv");
    m_simulationFile.setFileName(config->simulationLogPath + QDir::separator() +
                                 "simulation_position_observer_output.csv");
  }

private:
  SkydelNotifierInterface *m_skydelNotifier; // Pointer to the SkydelNotifierInterface.
  QSharedPointer<RRVConfiguration> config; // Shared pointer to the configuration object.
  QSharedPointer<Sdx::Ecef> receiverPosition; // Shared pointer to the receiver's ECEF position.
  QSharedPointer<Sdx::Ecef> simulationPosition; // Shared pointer to the simulator's ECEF position.
  QFile m_receiverFile; // File for logging receiver positions.
  QFile m_simulationFile; // File for logging simulation positions.
  QUdpSocket m_simulationSocket; // Socket for network logging.
  QUdpSocket m_receiverSocket; // Socket for network logging.


};
