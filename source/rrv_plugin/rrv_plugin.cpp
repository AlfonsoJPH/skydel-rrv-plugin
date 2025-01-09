#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>

SkydelWidgets Rrv_Plugin::createUI()
{
  auto view = new rrv_viewer();

  view->updateDataPanelValue(logPath);

  return {view};
}

void Rrv_Plugin::initialize() {
    receiver = std::make_unique<SerialReceiver>("/dev/ttyACM2", 9600, this, true, logPath);

    QThread* receiverThread = new QThread(this);
    receiver->moveToThread(receiverThread);
    connect(receiverThread, &QThread::started, receiver.get(), &SerialReceiver::connectReceiver);
    connect(receiverThread, &QThread::finished, receiver.get(), &SerialReceiver::disconnectReceiver);

    receiverThread->start();

}

