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
    void dataReceived(const QString& data){updateDataPanelValue(data);};

signals:
    // Signals to update configuration  
    void portNameChanged(const QString& name);
    void baudRateChanged(int rate);
    void fileLoggingChanged(bool state);
    void receiverStateChanged();
    void logPathChanged(const QString& path);
    
private:
    Ui::rrv_viewer *ui;
    RRVConfiguration *config;
    QString dataPanelValue;
};

