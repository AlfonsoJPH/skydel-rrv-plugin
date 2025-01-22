#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>
#include <QDebug>
#include <QSerialPortInfo>

SkydelWidgets Rrv_Plugin::createUI()
{
    QString portName = QSerialPortInfo::availablePorts().isEmpty() ? "" : QSerialPortInfo::availablePorts()[0].portName();
  
  config = QSharedPointer<RRVConfiguration>::create(false, portName, 9600, QDir::homePath(), false, QHostAddress("127.0.0.1"), 8080,
                                                    false, QDir::homePath(), false, QHostAddress("127.0.0.1"), 8081,
                                                    false, QDir::homePath(), false, QHostAddress("127.0.0.1"), 8081);


  //Create receiver
  receiver = std::make_unique<SerialReceiver>(this, config);

  view = new rrv_viewer(config);
  //Connect view to config
  connect(view, &rrv_viewer::serialConfigChanged, receiver.get(), &Receiver::configChanged);
  connect(view, &rrv_viewer::observerConfigChanged, this, &Rrv_Plugin::observerConfigChanged);
  
  
  //Connect receiver to view
  connect(receiver.get(), &Receiver::receiverStateChanges, view, &rrv_viewer::receiverStateChanges);
  connect(receiver.get(), &Receiver::dataReceived, view, &rrv_viewer::dataReceived);

  
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


  return {view};
}

void Rrv_Plugin::initialize()
{
}

rrv_viewer* Rrv_Plugin::getView() const
{
    return view;
}

