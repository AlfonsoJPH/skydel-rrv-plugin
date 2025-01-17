#pragma once

#include <QString>

class RRVConfiguration {
public:
    QString portName;
    int baudRate;
    QString logPath;
    bool fileLogging;
    QString networkLogAddress;
    QString networkLogPort;
    bool networkLogging;

    RRVConfiguration(bool fileLogging = false, QString portName = "", int baudRate = 9600, QString logPath = "",
                     bool networkLogging = false, QString networkLogAddress = "", QString networkLogPort = "")
        : fileLogging(fileLogging), portName(portName), baudRate(baudRate), logPath(logPath),
          networkLogging(networkLogging), networkLogAddress(networkLogAddress), networkLogPort(networkLogPort) {}
};

