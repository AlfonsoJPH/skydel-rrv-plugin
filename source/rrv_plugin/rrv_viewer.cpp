#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QLocale>
rrv_viewer::rrv_viewer(RRVConfiguration *config, QWidget *parent) :
    QWidget(parent), config(config),
    ui(new Ui::rrv_viewer)
{
    ui->setupUi(this);

    ui->portName->setText(config->portName);
    ui->fileLogState->setChecked(config->fileLogging);
    ui->baudRate->setText(QLocale(). toString(config->baudRate));
    
    connect(ui->portName, &QLineEdit::textChanged, this, [this](const QString &text) { emit portNameChanged(text); });
    
    connect(ui->baudRate, &QLineEdit::textChanged, this, [this](const QString &text) { emit baudRateChanged(text.toInt()); });
    connect(ui->fileLogState, &QCheckBox::stateChanged, this, [this](int state) { emit fileLoggingChanged(state); });
    connect(ui->receiverState, &QPushButton::clicked, this, [this]() 
    { 
        emit receiverStateChanged();
        ui->receiverState->setText(ui->receiverState->text() == "Start" ? "Stop" : "Start");
    });
}

rrv_viewer::~rrv_viewer()
{
    delete ui;
}

void rrv_viewer::updateDataPanelValue(QString value)
{
    ui->viewData->setText(value);
}

void rrv_viewer::dataReceived(QString data)
{
    updateDataPanelValue(data);
}

