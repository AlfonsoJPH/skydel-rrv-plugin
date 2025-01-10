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

private:
    bool enabledFileLogging;
    QString fileLogPath;
    QFile fileLog;
    
signals:
    void dataReceived(const QString& data);
};


