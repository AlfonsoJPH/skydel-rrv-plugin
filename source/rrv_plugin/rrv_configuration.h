#pragma once

#include <QString>
#include <QtNetwork/QHostAddress>
class RRVConfiguration {
public:
    QString serialPortName;
    int baudRate;
    QString serialLogPath;
    bool serialFileLogging;
    QHostAddress serialNetworkLogAddress;
    ushort serialNetworkLogPort;
    bool serialNetworkLogging;
    
    QString simulationLogPath;
    bool simulationFileLogging;
    QHostAddress simulationNetworkLogAddress;
    ushort simulationNetworkLogPort;
    bool simulationNetworkLogging;

    QString receiverLogPath;
    bool receiverFileLogging;
    QHostAddress receiverNetworkLogAddress;
    ushort receiverNetworkLogPort;
    bool receiverNetworkLogging;
    
    RRVConfiguration(bool serialFileLogging = false, QString serialPortName = "", int baudRate = 9600, QString serialLogPath = "",
             bool serialNetworkLogging = false, QHostAddress serialNetworkLogAddress = QHostAddress("127.0.0.1"), ushort serialNetworkLogPort = 8080,
             bool simulationFileLogging = false, QString simulationLogPath = "", bool simulationNetworkLogging = false,
             QHostAddress simulationNetworkLogAddress = QHostAddress("127.0.0.1"), ushort simulationNetworkLogPort = 8081,
             bool receiverFileLogging = false, QString receiverLogPath = "", bool receiverNetworkLogging = false,
             QHostAddress receiverNetworkLogAddress = QHostAddress("127.0.0.1"), ushort receiverNetworkLogPort = 8081)
      : serialFileLogging(serialFileLogging), serialPortName(serialPortName), baudRate(baudRate), serialLogPath(serialLogPath),
        serialNetworkLogging(serialNetworkLogging), serialNetworkLogAddress(serialNetworkLogAddress), serialNetworkLogPort(serialNetworkLogPort),
        simulationFileLogging(simulationFileLogging), simulationLogPath(simulationLogPath), simulationNetworkLogging(simulationNetworkLogging),
        simulationNetworkLogAddress(simulationNetworkLogAddress), simulationNetworkLogPort(simulationNetworkLogPort),
        receiverFileLogging(receiverFileLogging), receiverLogPath(receiverLogPath), receiverNetworkLogging(receiverNetworkLogging),
        receiverNetworkLogAddress(receiverNetworkLogAddress), receiverNetworkLogPort(receiverNetworkLogPort) {}
};

