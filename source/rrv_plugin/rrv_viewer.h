#pragma once

#include <QWidget>
#include <QTextEdit>
#include "rrv_configuration.h"
#include "../receiver_logic/receiver.h"

namespace Ui {
class rrv_viewer;
}

class rrv_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit rrv_viewer(RRVConfiguration *config, QWidget *parent = nullptr);
    ~rrv_viewer();
    void updateDataPanelValue(const QString& dataPanelValue);
    void updateDataPanelValue();

public slots:
    void dataReceived(const QString& data);
    void receiverStateChanges(bool state);

signals:
    // Signals to update configuration  
    void portNameChanged(const QString& name);
    void baudRateChanged(int rate);
    void receiverStateChanged();

    void fileLoggingChanged(bool state);
    void logPathChanged(const QString& path);

    void networkLoggingChanged(bool state);
    void logNetworkChanged(const QString& address, const QString& port);

    
private:
    Ui::rrv_viewer *ui;
    RRVConfiguration *config;
    QString dataPanelValue;
};

