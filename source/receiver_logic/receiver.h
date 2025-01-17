#pragma once

#include <QString>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

class Receiver : public QObject {
    Q_OBJECT
public:
    explicit Receiver(QObject* parent = nullptr, bool enabledFileLogging = false,
                      QString fileLogPath = "", bool enabledNetworkLogging = false,
                      QString networkLogAddress = "", QString networkLogPort = "");

    virtual ~Receiver() = default;
    virtual bool connectReceiver() = 0;
    virtual void disconnectReceiver() = 0;
    virtual QString getData() = 0;
    void fileLogData(const QString& data);
    void setFileLogging(bool state) { enabledFileLogging = state; }
    void setNetworkLogging(bool state) { enabledNetworkLogging = state; }
    void networkLogData(const QString& data);
    bool getState() { return state; }
    void logData(const QString& data) {
        if(enabledFileLogging) fileLogData(data); 
        if(enabledNetworkLogging) networkLogData(data); 
    };

private:
    QString fileLogPath;
    QHostAddress networkLogAddress;
    quint16 networkLogPort;
    bool enabledFileLogging, enabledNetworkLogging;
    
    QFile fileLog;
    QUdpSocket *udpSocketLogging;

    bool state;

// Slots for updating receiver configuration
public slots:
    virtual void receiverStateChanged() = 0;
    void fileLoggingChanged(bool state) { enabledFileLogging = state; };
    void networkLoggingChanged(bool state) { enabledNetworkLogging = state; };
    void logPathChanged(const QString& path) { 
        fileLogPath = path; 
        fileLog.setFileName(fileLogPath + QDir::separator() + "rrv.log");    
    };
    void logNetworkChanged(const QString& adddress, const QString& port) { 
        networkLogAddress = QHostAddress(adddress); 
        networkLogPort = port.toUShort();

    };

// Signals for sending data received
signals:
    void dataReceived(const QString& data);
    void receiverStateChanges(bool state);
};


