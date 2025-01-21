#pragma once

#include <QWidget>
#include <QTextEdit>
#include "rrv_configuration.h"
#include "../receiver_logic/receiver.h"
#include "ecef.h"

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

public slots:
    void dataReceived(const QString& data);
    void receiverStateChanges(bool state);
    void observerDataReceived(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p);

signals:
    // Signals to update configuration  
    void portNameChanged(const QString& name);
    void baudRateChanged(int rate);
    void receiverStateChanged();

    void serialFileLoggingChanged(bool state);
    void serialLogPathChanged(const QString& path);
    void serialNetworkLoggingChanged(bool state);
    void serialLogNetworkChanged(const QString& address, const QString& port);

    
    void simulationFileLoggingChanged(bool state);
    void simulationLogPathChanged(const QString& path);

    void simulationNetworkLoggingChanged(bool state);
    void simulationLogNetworkChanged(const QString& address, const QString& port);

    
    void receiverFileLoggingChanged(bool state);
    void receiverLogPathChanged(const QString& path);

    void receiverNetworkLoggingChanged(bool state);
    void receiverLogNetworkChanged(const QString& address, const QString& port);

    void validECEF(const Sdx::Ecef& receiver_p);
    
private:
    Ui::rrv_viewer *ui;
    RRVConfiguration *config;
};

