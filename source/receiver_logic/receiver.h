#pragma once

#include "../rrv_plugin/rrv_configuration.h"
#include <QDir>
#include <QFile>
#include <QObject>
#include <QString>
#include <QtNetwork/QUdpSocket>

/**
 * # General Receiver class
 */
/**
 * @class Receiver
 * @brief Abstract base class for handling receiver operations.
 *
 * This class provides the basic interface and functionality for connecting to a
 * receiver, logging data to a file, and sending data over a network using UDP.
 */
class Receiver : public QObject {
  Q_OBJECT
public:
  /**
   * The configuration object that specifies the receiver parameters like port
   * name, baud rate, and logging options.
   */
  QSharedPointer<RRVConfiguration> config;

  /**
   * @brief Constructor for the Receiver class.
   * @param parent The parent QObject.
   * @param config The shared pointer to the configuration object.
   */
  explicit Receiver(QObject *parent = nullptr,
                    QSharedPointer<RRVConfiguration> config = nullptr);

  /**
   * @brief Virtual destructor for the Receiver class.
   */
  virtual ~Receiver() = default;

  /**
   * @brief Virtual function to connect the receiver.
   *
   * This function must be implemented by derived classes to handle the
   * connection logic.
   *
   * @return True if the connection is successful, false otherwise.
   */
  virtual bool connectReceiver() = 0;

  /**
   * @brief Virtual function to disconnect the receiver.
   *
   * This function must be implemented by derived classes to handle the
   * disconnection logic.
   */
  virtual void disconnectReceiver() = 0;

  /**
   * @brief Pure virtual function to get data from the receiver.
   *
   * This function must be implemented by derived classes to handle data
   * retrieval.
   *
   * @return The data received from the receiver as a QString.
   */
  virtual QString getData() = 0;

  /**
   * @brief Logs on a file an event if the configuration is set to log on file.
   *
   * File name is rrv.log, the path is set in the configuration file.
   * The file path can´t be changed without restarting the receiver.
   * @param data The data to log
   * @see networkLogData
   * @see logData
   * @see config
   * @see setFileLogPath
   */
  void fileLogData(const QString &data);

  /**
   * @brief Logs on a network an event if the configuration is set to log on
   * network.
   *
   * The data is sent to the network address and port set in the configuration
   * file.
   *
   * @param data The data to log
   * @see fileLogData
   * @see logData
   * @see config
   */
  void networkLogData(const QString &data);

  /**
   * @brief Get the state of the receiver (if it is connected by this object or
   * not)
   *
   * @return bool The state of the receiver
   */
  bool getState() { return state; }

  /**
   * @brief Logs the data on file and network if the configuration is set to log
   * on file and network. The file path can´t be changed without restarting the
   * receiver.
   *
   * @param data The data to log
   * @see fileLogData
   * @see networkLogData
   */
  void logData(const QString &data) {
    if (config->serialFileLogging)
      fileLogData(data);
    if (config->serialNetworkLogging)
      networkLogData(data);
  };

  /**
   * @brief Set the path of the log file.
   *
   * The file path can't be changed without restarting the receiver.
   * The file name is `rrv.log`.
   *
   * @param path The path where the log file will be saved.
   * @see config
   * @see fileLog
   */
  void setFileLogPath(const QString &path) {
    config->serialLogPath = path;
    fileLog.setFileName(config->serialLogPath + QDir::separator() + "rrv.log");
  };

private:
  QFile fileLog;               // File for logging
  QUdpSocket udpSocketLogging; // Socket for network logging
  bool state; // true if receiver is connected to this object, false if
              // disconnected

  /**
   * ## Slots
   */
public slots:
  /**
   * @brief Slot to update the receiver state.
   *
   * This slot must be implemented by derived classes to handle state changes.
   */
  virtual void receiverStateChanged() = 0;

  /**
   * @brief Slot to update the receiver configuration.
   *
   * This slot sets the file log path since it is the only configuration that
   * needs to be processed.
   */
  void configChanged() { setFileLogPath(config->serialLogPath); };

  /**
   * ## Signals for sending data received and receiver state changes
   */
signals:
  /**
   * Sends the data received from the receiver
   * Triggers when the receiver receives data
   *
   * @param data The data received
   * @see getData
   */
  void dataReceived(const QString &data);

  /**
   * Sends the new state of the receiver
   * Triggers when the receiver is connected or disconnected
   *
   * @param state The state of the receiver
   * @see connectReceiver
   * @see disconnectReceiver
   */
  void receiverStateChanges(bool state);
};
