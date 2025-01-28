#pragma once

#include <QString>
#include <QtNetwork/QHostAddress>

/**
 * @brief Configuration class for the RRV plugin
 *
 * This class holds the configuration for the RRV plugin, including the serial
 * port name, baud rate, and logging options.
 */
class RRVConfiguration {
public:
  QString serialPortName; // Serial port name
  int baudRate; // Baud rate
  QString serialLogPath; // Path to log file of the serial receiver
  bool serialFileLogging; // Flag to enable file logging of the serial receiver
  QHostAddress serialNetworkLogAddress; // Network address for logging of the serial receiver
  ushort serialNetworkLogPort; // Network port for logging of the serial receiver
  bool serialNetworkLogging; // Flag to enable network logging of the serial receiver

  QString simulationLogPath; // Path to log file of the simulation observer
  bool simulationFileLogging; // Flag to enable file logging of the simulation observer
  QHostAddress simulationNetworkLogAddress; // Network address for logging of the simulation observer
  ushort simulationNetworkLogPort; // Network port for logging of the simulation observer
  bool simulationNetworkLogging; // Flag to enable network logging of the simulation observer

  QString receiverLogPath; // Path to log file of the receiver observer
  bool receiverFileLogging; // Flag to enable file logging of the receiver observer
  QHostAddress receiverNetworkLogAddress; // Network address for logging of the receiver observer
  ushort receiverNetworkLogPort; // Network port for logging of the receiver observer
  bool receiverNetworkLogging; // Flag to enable network logging of the receiver observer

  /**
   * @brief Construct a new RRVConfiguration object that contains the configuration for the RRV plugin.
   * 
   * @param serialFileLogging Flag to enable file logging of the serial receiver
   * @param serialPortName Serial port name
   * @param baudRate Baud rate
   * @param serialLogPath Path to log file of the serial receiver
   * @param serialNetworkLogging Flag to enable network logging of the serial receiver
   * @param serialNetworkLogAddress Network address for logging of the serial receiver
   * @param serialNetworkLogPort Network port for logging of the serial receiver
   * @param simulationFileLogging Flag to enable file logging of the simulation observer
   * @param simulationLogPath Path to log file of the simulation observer
   * @param simulationNetworkLogging Flag to enable network logging of the simulation observer
   * @param simulationNetworkLogAddress Network address for logging of the simulation observer
   * @param simulationNetworkLogPort Network port for logging of the simulation observer
   * @param receiverFileLogging Flag to enable file logging of the receiver observer
   * @param receiverLogPath Path to log file of the receiver observer
   * @param receiverNetworkLogging Flag to enable network logging of the receiver observer
   * @param receiverNetworkLogAddress Network address for logging of the receiver observer
   * @param receiverNetworkLogPort Network port for logging of the receiver observer
   */
  RRVConfiguration(
      bool serialFileLogging = false, QString serialPortName = "",
      int baudRate = 9600, QString serialLogPath = "",
      bool serialNetworkLogging = false,
      QHostAddress serialNetworkLogAddress = QHostAddress("127.0.0.1"),
      ushort serialNetworkLogPort = 8080, bool simulationFileLogging = false,
      QString simulationLogPath = "", bool simulationNetworkLogging = false,
      QHostAddress simulationNetworkLogAddress = QHostAddress("127.0.0.1"),
      ushort simulationNetworkLogPort = 8081, bool receiverFileLogging = false,
      QString receiverLogPath = "", bool receiverNetworkLogging = false,
      QHostAddress receiverNetworkLogAddress = QHostAddress("127.0.0.1"),
      ushort receiverNetworkLogPort = 8081)
      : serialFileLogging(serialFileLogging), serialPortName(serialPortName),
        baudRate(baudRate), serialLogPath(serialLogPath),
        serialNetworkLogging(serialNetworkLogging),
        serialNetworkLogAddress(serialNetworkLogAddress),
        serialNetworkLogPort(serialNetworkLogPort),
        simulationFileLogging(simulationFileLogging),
        simulationLogPath(simulationLogPath),
        simulationNetworkLogging(simulationNetworkLogging),
        simulationNetworkLogAddress(simulationNetworkLogAddress),
        simulationNetworkLogPort(simulationNetworkLogPort),
        receiverFileLogging(receiverFileLogging),
        receiverLogPath(receiverLogPath),
        receiverNetworkLogging(receiverNetworkLogging),
        receiverNetworkLogAddress(receiverNetworkLogAddress),
        receiverNetworkLogPort(receiverNetworkLogPort) {}
};
