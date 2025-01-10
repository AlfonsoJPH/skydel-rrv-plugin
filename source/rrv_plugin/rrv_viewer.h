#pragma once

#include <QWidget>
#include <QTextEdit>
#include "rrv_configuration.h"

namespace Ui {
class rrv_viewer;
}

class rrv_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit rrv_viewer(RRVConfiguration *config, QWidget *parent = 0);
    ~rrv_viewer();
    void updateDataPanelValue(QString value);

public slots:
    void dataReceived(QString data);

signals:
    void portNameChanged(const QString& name);
    void baudRateChanged(int rate);
    void fileLoggingChanged(bool state);
    void receiverStateChanged();
    
private:
    Ui::rrv_viewer *ui;
    RRVConfiguration *config;
    

};

