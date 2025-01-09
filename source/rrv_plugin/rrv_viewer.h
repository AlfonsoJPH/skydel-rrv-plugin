#pragma once

#include <QWidget>
#include <QTextEdit>
#include "../receiver_logic/serial_receiver.h"

namespace Ui {
class rrv_viewer;
}

class rrv_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit rrv_viewer(QWidget *parent = 0);
    ~rrv_viewer();
    void updateDataPanelValue(QString value);

private:
    Ui::rrv_viewer *ui;
    std::unique_ptr<SerialReceiver> receiver;

};

