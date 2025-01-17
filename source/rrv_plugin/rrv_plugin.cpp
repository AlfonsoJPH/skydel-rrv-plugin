#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>
#include <QDebug>
#include <QSerialPortInfo>

SkydelWidgets Rrv_Plugin::createUI()
{
  QString portName = QSerialPortInfo::availablePorts().isEmpty() ? "" : QSerialPortInfo::availablePorts()[0].portName();
  config = new RRVConfiguration(false, portName, 9600, QDir::homePath(), false, "127.0.0.1", "8080");
  //Create receiver
  receiver = std::make_unique<SerialReceiver>(config->portName, config->baudRate, this, config->fileLogging, config->logPath);

  view = new rrv_viewer(config);
  //Connect view to config
  connect(view, &rrv_viewer::portNameChanged, this, &Rrv_Plugin::portNameChanged);
  connect(view, &rrv_viewer::baudRateChanged, this, &Rrv_Plugin::baudRateChanged);
  connect(view, &rrv_viewer::fileLoggingChanged, this, &Rrv_Plugin::fileLoggingChanged);
  connect(view, &rrv_viewer::logPathChanged, this, &Rrv_Plugin::logPathChanged);
  
  
  
  //Connect receiver to view
  connect(receiver.get(), &Receiver::receiverStateChanges, view, &rrv_viewer::receiverStateChanges);
  connect(receiver.get(), &Receiver::dataReceived, view, &rrv_viewer::dataReceived);
  connect(view, &rrv_viewer::receiverStateChanged, receiver.get(), &Receiver::receiverStateChanged);
  connect(view, &rrv_viewer::portNameChanged, receiver.get(), &SerialReceiver::portNameChanged);
  connect(view, &rrv_viewer::baudRateChanged, receiver.get(), &SerialReceiver::baudRateChanged);
  connect(view, &rrv_viewer::fileLoggingChanged, receiver.get(), &Receiver::fileLoggingChanged);
  connect(view, &rrv_viewer::networkLoggingChanged, receiver.get(), &Receiver::networkLoggingChanged);
  connect(view, &rrv_viewer::logPathChanged, receiver.get(), &Receiver::logPathChanged);

  //Move receiver to separate thread
  QThread* receiverThread = new QThread(this);
  receiver->moveToThread(receiverThread);
  connect(receiverThread, &QThread::finished, receiver.get(), &SerialReceiver::disconnectReceiver);
  connect(this, &Rrv_Plugin::destroyed, receiverThread, &QThread::quit);

  //Start receiver thread
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

