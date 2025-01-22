#pragma once

#include <QWidget>
#include <QTextEdit>
#include "rrv_configuration.h"
#include "../receiver_logic/receiver.h"
#include "ecef.h"
#include <QSharedPointer>
#include "../rrv_plugin/rrv_configuration.h"

namespace Ui {
class rrv_viewer;
}

class rrv_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit rrv_viewer(QSharedPointer<RRVConfiguration> config = nullptr, QWidget *parent = nullptr);
    ~rrv_viewer();
    void updateDataPanelValue(const QString& dataPanelValue);

public slots:
    void dataReceived(const QString& data);
    void receiverStateChanges(bool state);
    void observerDataReceived(Sdx::Ecef simulation_p, Sdx::Ecef receiver_p);

signals:
    // Signals to update configuration  
    void serialConfigChanged();
    void observerConfigChanged();
    void receiverStateChanged();

    void validECEF(const Sdx::Ecef& receiver_p);
    
private:
    Ui::rrv_viewer *ui;
    QSharedPointer<RRVConfiguration> config;
};

