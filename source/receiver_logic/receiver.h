#pragma once

#include <QString>
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QFile>
#include <QDir>
#include "../rrv_plugin/rrv_configuration.h"

class Receiver : public QObject {
    Q_OBJECT
public:
    explicit Receiver(QObject* parent = nullptr, QSharedPointer<RRVConfiguration> config = nullptr);

    virtual ~Receiver() = default;
    virtual bool connectReceiver() = 0;
    virtual void disconnectReceiver() = 0;
    virtual QString getData() = 0;
    void fileLogData(const QString& data);
    void networkLogData(const QString& data);
    bool getState() { return state; }
    void logData(const QString& data) {
        if(config->serialFileLogging) fileLogData(data); 
        if(config->serialNetworkLogging) networkLogData(data); 
    };
    void setFileLogPath(const QString& path) { 
        config->serialLogPath = path; 
        fileLog.setFileName(config->serialLogPath + QDir::separator() + "rrv.log");    
    };

private:    
    QFile fileLog;
    QUdpSocket udpSocketLogging;
    
    QSharedPointer<RRVConfiguration> config;

    bool state;

// Slots for updating receiver configuration
public slots:
    virtual void receiverStateChanged() = 0;
    void configChanged() {
        setFileLogPath(config->serialLogPath);
    };

// Signals for sending data received
signals:
    void dataReceived(const QString& data);
    void receiverStateChanges(bool state);
};


