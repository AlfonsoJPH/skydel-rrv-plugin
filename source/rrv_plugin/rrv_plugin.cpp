#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>
#include <QDebug>

SkydelWidgets Rrv_Plugin::createUI()
{
  config = new RRVConfiguration(false, "/dev/ttyACM2", 9600, "");
  view = new rrv_viewer(config);
    connect(view, &rrv_viewer::portNameChanged, this, &Rrv_Plugin::portNameChanged);
    connect(view, &rrv_viewer::baudRateChanged, this, &Rrv_Plugin::baudRateChanged);
    connect(view, &rrv_viewer::fileLoggingChanged, this, &Rrv_Plugin::fileLoggingChanged);
    connect(view, &rrv_viewer::receiverStateChanged, this, &Rrv_Plugin::receiverStateChanged);

    receiver = std::make_unique<SerialReceiver>(config->portName, config->baudRate, this, config->fileLogging, config->logPath);
    connect(receiver.get(), &Receiver::dataReceived, view, &rrv_viewer::dataReceived);

    QThread* receiverThread = new QThread(this);
    receiver->moveToThread(receiverThread);
    connect(receiverThread, &QThread::started, receiver.get(), &SerialReceiver::connectReceiver);
    connect(receiverThread, &QThread::finished, receiver.get(), &SerialReceiver::disconnectReceiver);
    receiverThread->start();

  return {view};
}

void Rrv_Plugin::initialize()
{


}

rrv_viewer* Rrv_Plugin::getView() const
{
    return view;
}

