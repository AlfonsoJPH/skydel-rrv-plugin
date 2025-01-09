#include "rrv_plugin.h"
#include "rrv_viewer.h"
#include <QThread>

SkydelWidgets Rrv_Plugin::createUI()
{
  auto view = new rrv_viewer();

  return {view};
}
void Rrv_Plugin::initialize() {
}

