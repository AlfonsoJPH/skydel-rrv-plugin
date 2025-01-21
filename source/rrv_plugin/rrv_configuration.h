#pragma once

#include <QString>

class RRVConfiguration {
public:
    QString serialPortName;
    int baudRate;
    QString serialLogPath;
    bool serialFileLogging;
    QString serialNetworkLogAddress;
    QString serialNetworkLogPort;
    bool serialNetworkLogging;
    
    QString simulationLogPath;
    bool simulationFileLogging;
    QString simulationNetworkLogAddress;
    QString simulationNetworkLogPort;
    bool simulationNetworkLogging;

    QString receiverLogPath;
    bool receiverFileLogging;
    QString receiverNetworkLogAddress;
    QString receiverNetworkLogPort;
    bool receiverNetworkLogging;
    
    RRVConfiguration(bool serialFileLogging = false, QString serialPortName = "", int baudRate = 9600, QString serialLogPath = "",
             bool serialNetworkLogging = false, QString serialNetworkLogAddress = "", QString serialNetworkLogPort = "",
             bool simulationFileLogging = false, QString simulationLogPath = "", bool simulationNetworkLogging = false,
             QString simulationNetworkLogAddress = "", QString simulationNetworkLogPort = "",
             bool receiverFileLogging = false, QString receiverLogPath = "", bool receiverNetworkLogging = false,
             QString receiverNetworkLogAddress = "", QString receiverNetworkLogPort = "")
      : serialFileLogging(serialFileLogging), serialPortName(serialPortName), baudRate(baudRate), serialLogPath(serialLogPath),
        serialNetworkLogging(serialNetworkLogging), serialNetworkLogAddress(serialNetworkLogAddress), serialNetworkLogPort(serialNetworkLogPort),
        simulationFileLogging(simulationFileLogging), simulationLogPath(simulationLogPath), simulationNetworkLogging(simulationNetworkLogging),
        simulationNetworkLogAddress(simulationNetworkLogAddress), simulationNetworkLogPort(simulationNetworkLogPort),
        receiverFileLogging(receiverFileLogging), receiverLogPath(receiverLogPath), receiverNetworkLogging(receiverNetworkLogging),
        receiverNetworkLogAddress(receiverNetworkLogAddress), receiverNetworkLogPort(receiverNetworkLogPort) {}
};

