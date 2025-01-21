#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>
#include <QDebug>
#include <QSerialPortInfo>

SkydelWidgets Rrv_Plugin::createUI()
{
  QString portName = QSerialPortInfo::availablePorts().isEmpty() ? "" : QSerialPortInfo::availablePorts()[0].portName();
  config = new RRVConfiguration(false, portName, 9600, QDir::homePath(), false, "127.0.0.1", "8080",
                                false, QDir::homePath(), false, "127.0.0.1", "8081",
                                false, QDir::homePath(), false, "127.0.0.1", "8081");
  //Create receiver
  receiver = std::make_unique<SerialReceiver>(config->serialPortName, config->baudRate, this);

  view = new rrv_viewer(config);
  //Connect view to config
  connect(view, &rrv_viewer::portNameChanged, this, &Rrv_Plugin::portNameChanged);
  connect(view, &rrv_viewer::baudRateChanged, this, &Rrv_Plugin::baudRateChanged);
  connect(view, &rrv_viewer::serialFileLoggingChanged, this, &Rrv_Plugin::serialFileLoggingChanged);
  connect(view, &rrv_viewer::serialLogPathChanged, this, &Rrv_Plugin::serialLogPathChanged);
  connect(view, &rrv_viewer::serialLogNetworkChanged, this, &Rrv_Plugin::serialLogNetworkChanged);
  
  
  //Connect receiver to view
  connect(receiver.get(), &Receiver::receiverStateChanges, view, &rrv_viewer::receiverStateChanges);
  connect(receiver.get(), &Receiver::dataReceived, view, &rrv_viewer::dataReceived);
  connect(view, &rrv_viewer::receiverStateChanged, receiver.get(), &Receiver::receiverStateChanged);
  connect(view, &rrv_viewer::portNameChanged, receiver.get(), &SerialReceiver::portNameChanged);
  connect(view, &rrv_viewer::baudRateChanged, receiver.get(), &SerialReceiver::baudRateChanged);
  connect(view, &rrv_viewer::serialFileLoggingChanged, receiver.get(), &Receiver::fileLoggingChanged);
  connect(view, &rrv_viewer::serialNetworkLoggingChanged, receiver.get(), &Receiver::networkLoggingChanged);
  connect(view, &rrv_viewer::serialLogPathChanged, receiver.get(), &Receiver::logPathChanged);
  connect(view, &rrv_viewer::serialLogNetworkChanged, receiver.get(), &Receiver::logNetworkChanged);
  
  //Move receiver to separate thread
  QThread* receiverThread = new QThread(this);
  receiver->moveToThread(receiverThread);
  connect(receiverThread, &QThread::finished, receiver.get(), &SerialReceiver::disconnectReceiver);
  connect(this, &Rrv_Plugin::destroyed, receiverThread, &QThread::quit);

  //Start receiver thread
  receiverThread->start();


  //Connect observers to view
  connect(this, &Rrv_Plugin::observerPosition, view, &rrv_viewer::observerDataReceived);
  connect(view, &rrv_viewer::validECEF, this, &Rrv_Plugin::updatedReceiverPosition);

  //Connect configuration to observers
  connect(view, &rrv_viewer::simulationFileLoggingChanged, this, &Rrv_Plugin::simulationFileLoggingChanged);
  connect(view, &rrv_viewer::simulationLogPathChanged, this, &Rrv_Plugin::simulationLogPathChanged);
  connect(view, &rrv_viewer::simulationNetworkLoggingChanged, this, &Rrv_Plugin::simulationNetworkLoggingChanged);
  connect(view, &rrv_viewer::simulationLogNetworkChanged, this, &Rrv_Plugin::simulationLogNetworkChanged);


  connect(view, &rrv_viewer::receiverFileLoggingChanged, this, &Rrv_Plugin::receiverFileLoggingChanged);
  connect(view, &rrv_viewer::receiverLogPathChanged, this, &Rrv_Plugin::receiverLogPathChanged);
  connect(view, &rrv_viewer::receiverNetworkLoggingChanged, this, &Rrv_Plugin::receiverNetworkLoggingChanged);
  connect(view, &rrv_viewer::receiverLogNetworkChanged, this, &Rrv_Plugin::receiverLogNetworkChanged);

  return {view};
}

void Rrv_Plugin::initialize()
{
}

rrv_viewer* Rrv_Plugin::getView() const
{
    return view;
}

