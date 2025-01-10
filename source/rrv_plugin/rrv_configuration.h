#pragma once

#include <QString>

class RRVConfiguration {
public:
    bool fileLogging;
    QString portName;
    int baudRate;
    QString logPath;

    RRVConfiguration(bool fileLogging, QString portName, int baudRate, QString logPath)
        : fileLogging(fileLogging), portName(portName), baudRate(baudRate), logPath(logPath) {}
};

