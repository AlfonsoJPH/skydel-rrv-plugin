#pragma once

#include "../receiver_logic/receiver.h"
#include "../rrv_plugin/rrv_configuration.h"
#include "ecef.h"
#include "rrv_configuration.h"
#include <QSharedPointer>
#include <QTextEdit>
#include <QWidget>

namespace Ui {
class rrv_viewer;
}

/**
 * @brief The rrv_viewer class
 *
 * This class is responsible for displaying the RRV plugin's user interface.
 * It displays the configuration options for the serial port, baud rate, and
 * logging options for serial receiver and observers. It also displays the 
 * receiver and simulation positions.
 */
class rrv_viewer : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Construct a new rrv_viewer object
   *
   * @param config The shared pointer to the RRVConfiguration object
   * @param receiverPosition The shared pointer to the receiver's ECEF position
   * @param simulationPosition The shared pointer to the simulation's ECEF position
   * @param parent The parent QWidget
   */
  explicit rrv_viewer(QSharedPointer<RRVConfiguration> config = nullptr,
                      QSharedPointer<Sdx::Ecef> receiverPosition = nullptr,
                      QSharedPointer<Sdx::Ecef> simulationPosition = nullptr,
                      QWidget *parent = nullptr);

  /**
   * @brief Destroy the rrv viewer object
   * 
   */
  ~rrv_viewer();

  /**
   * @brief Update the data panel value
   * 
   * @param dataPanelValue The value to update the data panel with the data logged by the receiver
   */
  void updateDataPanelValue(const QString &dataPanelValue);

public slots:
  /**
   * @brief Slot to update the receiver state
   * 
   * Triggers when the receiver sends data to the observer
   * Proccess and show the data of the receiver, parse the NMEA message
   * and update the information of summary panel.
   * Also updates the receiver´s position of the shared pointer.
   * @param state The state of the receiver
   */
  void dataReceived(const QString &data);

  /**
   * @brief Slot to manage when connect/disconnect receiver
   * 
   * Triggers when the user clicks the connect/disconnect button and
   * receivers notify the new state.
   * @param state 
   * @see receiverStateChanged
   * @see Receiver#connectReceiver
   * @see Receiver#disconnectReceiver
   */
  void receiverStateChanges(bool state);

signals:
  // Signals to update configuration
  /**
   * @brief Signal to notify the serial configuration has changed
   * 
   * Triggers when the user changes the serial configuration, port
   * name and baud rate won´t be updated until the user connects
   * or reconnects the receiver.
   */
  void serialConfigChanged();

  /**
   * @brief Signal to notify the observer that configuration has changed
   * 
   */
  void observerConfigChanged();

  /**
   * @brief Signal to ask the receiver to change its state
   * 
   * Triggers when the user clicks the connect/disconnect button
   * @see receiverStateChanged
   * @see Receiver#connectReceiver
   * @see Receiver#disconnectReceiver
   */
  void receiverStateChanged();

private:
  Ui::rrv_viewer *ui;
  QSharedPointer<RRVConfiguration> config;
  QSharedPointer<Sdx::Ecef> receiverPosition, simulationPosition;
};
