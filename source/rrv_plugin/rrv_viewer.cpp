#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QThread>

rrv_viewer::rrv_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rrv_viewer)
{
    ui->setupUi(this);
    receiver = std::make_unique<SerialReceiver>("/dev/ttyACM0, 9600, this);
    connect(receiver.get(), &SerialReceiver::dataReceived, this, &rrv_viewer::updateDataPanelValue);

    QThread* receiverThread = new QThread(this);
    receiver->moveToThread(receiverThread);
    connect(receiverThread, &QThread::started, receiver.get(), &SerialReceiver::connectReceiver);
    connect(receiverThread, &QThread::finished, receiver.get(), &SerialReceiver::disconnectReceiver);

    receiverThread->start();

}

rrv_viewer::~rrv_viewer()
{
    delete ui;
}

void rrv_viewer::updateDataPanelValue(QString value)
{
    ui->viewData->setText(value);
}