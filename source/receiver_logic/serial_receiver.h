#pragma once

#include "../rrv_plugin/rrv_configuration.h"
#include "receiver.h"
#include <QObject>
#include <QSerialPort>

/**
 * @class SerialReceiver
 * @brief A class that represents a serial receiver, inheriting from the
 * Receiver class.
 *
 * The SerialReceiver class is responsible for handling serial communication
 * using QSerialPort. It connects to the serial port, reads data, handles
 * errors, and manages the receiver state.
 *
 * @param parent The parent QObject, default is nullptr.
 * @param config A shared pointer to the RRVConfiguration object, default is
 * nullptr.
 */
class SerialReceiver : public Receiver {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the SerialReceiver class.
   * @param parent The parent QObject.
   * @param config The shared pointer to the configuration object.
   */
  SerialReceiver(QObject *parent = nullptr,
                 QSharedPointer<RRVConfiguration> config = nullptr)
      : Receiver(parent, config) {
    connect(&m_serialPort, &QSerialPort::readyRead, this,
            &SerialReceiver::handleReadyRead);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this,
            &SerialReceiver::handleError);
  }

  /**
   * @brief Connects the receiver to the serial port if it is not busy.
   *
   * @return true If the connection is successful.
   * @return false If the connection fails (is busy or an error occurs).
   */
  bool connectReceiver() override;

  /**
   * @brief Disconnects the receiver from the serial port.
   */
  void disconnectReceiver() override;

  /**
   * @brief Gets data from the serial port, pops the data from the serial port
   * buffer.
   *
   * @return QString The data read from the serial port.
   */
  QString getData() override;

public slots:
  /**
   * @brief Triggers when data is available to read, logs the serial port data
   * and sends it through the dataReceive signal.
   */
  void handleReadyRead();

  /**
   * @brief Triggers when an error occurs, logs the error and disconnects the
   * receiver.
   *
   * @param error The error that occurred.
   */
  void handleError(QSerialPort::SerialPortError error);

  /**
   * @brief Triggers when the receiver state is asked to change, connects or
   * disconnects the receiver.
   */
  void receiverStateChanged() override;

private:
  QSerialPort m_serialPort;
};