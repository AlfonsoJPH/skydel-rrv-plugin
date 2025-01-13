#pragma once

#include <QString>
#include <QObject>
#include <QFile>

class Receiver : public QObject {
    Q_OBJECT
public:
    explicit Receiver(QObject* parent = nullptr, bool enabledFileLogging = false,
                      QString fileLogPath = "");
    virtual ~Receiver() = default;
    virtual bool connectReceiver() = 0;
    virtual void disconnectReceiver() = 0;
    virtual QString getData() = 0;
    void fileLogData(const QString& data);
    void setFileLogging(bool state) { enabledFileLogging = state; }
    bool getState() { return state; }
    
private:
    bool enabledFileLogging;
    QString fileLogPath;
    QFile fileLog;
    bool state;

// Slots for updating receiver configuration
public slots:
    virtual void receiverStateChanged() = 0;
    virtual void fileLoggingChanged(bool state) = 0;
    void logPathChanged(const QString& path) { fileLogPath = path; };

// Signals for sending data received
signals:
    void dataReceived(const QString& data);
};


