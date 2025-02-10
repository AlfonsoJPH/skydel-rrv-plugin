#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QThread>

Rrv_Plugin::~Rrv_Plugin() {
  if (receiverThread) {
    receiverThread->quit();
    receiverThread->wait();
    delete receiverThread;
  }
}

SkydelWidgets Rrv_Plugin::createUI() {
  QString portName = QSerialPortInfo::availablePorts().isEmpty()
                         ? ""
                         : QSerialPortInfo::availablePorts()[0].portName();
  receiverPosition = QSharedPointer<Sdx::Ecef>::create(0, 0, 0);
  simulationPosition = QSharedPointer<Sdx::Ecef>::create(0, 0, 0);
  config = QSharedPointer<RRVConfiguration>::create(
      false, portName, 9600, QDir::homePath(), false, QHostAddress("127.0.0.1"),
      8080, false, QDir::homePath(), false, QHostAddress("127.0.0.1"), 8081,
      false, QDir::homePath(), false, QHostAddress("127.0.0.1"), 8081);
  // Create receiver
  receiver = std::make_unique<SerialReceiver>(this, config);

  view = new rrv_viewer(config, receiverPosition, simulationPosition);
  // Connect view to config
  connect(view, &rrv_viewer::serialConfigChanged, receiver.get(),
          &Receiver::configChanged);
  connect(view, &rrv_viewer::observerConfigChanged, this,
          &Rrv_Plugin::observerConfigChanged);
  // Connect receiver to view
  connect(view, &rrv_viewer::receiverStateChanged, receiver.get(),
          &SerialReceiver::receiverStateChanged);
  connect(receiver.get(), &Receiver::receiverStateChanges, view,
          &rrv_viewer::receiverStateChanges);
  connect(receiver.get(), &Receiver::dataReceived, view,
          &rrv_viewer::dataReceived);

  // Move receiver to separate thread
  receiverThread = new QThread(this);
  receiver->moveToThread(receiverThread);
  connect(receiverThread, &QThread::finished, receiver.get(),
          &SerialReceiver::disconnectReceiver);
  connect(this, &Rrv_Plugin::destroyed, receiverThread, &QThread::quit);

  // Start receiver thread
  receiverThread->start();

  return {view};
}

void Rrv_Plugin::initialize() {}

rrv_viewer *Rrv_Plugin::getView() const { return view; }
