#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"

rrv_viewer::rrv_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rrv_viewer)
{
    ui->setupUi(this);
}

rrv_viewer::~rrv_viewer()
{
    delete ui;
}

void rrv_viewer::updateDataPanelValue(QString value)
{
    ui->viewData->setText(value);
}