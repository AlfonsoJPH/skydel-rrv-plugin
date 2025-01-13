#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QLocale>
#include <QScrollBar>
rrv_viewer::rrv_viewer(RRVConfiguration *config, Receiver *receiver, QWidget *parent) :
    QWidget(parent), config(config),
    ui(new Ui::rrv_viewer)
{
    ui->setupUi(this);

    ui->portName->setText(config->portName);
    ui->fileLogState->setChecked(config->fileLogging);
    ui->baudRate->setText(QLocale(). toString(config->baudRate));
    ui->logPath->setText(config->logPath);
    ui->viewData->setReadOnly(true);
    connect(ui->portName, &QLineEdit::textChanged, this, [this](const QString &text) { emit portNameChanged(text); });
    
    connect(ui->baudRate, &QLineEdit::textChanged, this, [this](const QString &text) { emit baudRateChanged(text.toInt()); });
    connect(ui->logPath, &QLineEdit::textChanged, this, [this](const QString &text) { emit logPathChanged(text); });
    connect(ui->fileLogState, &QCheckBox::stateChanged, this, [this](int state) { emit fileLoggingChanged(state); });

    connect(ui->receiverState, &QPushButton::clicked, this, [this]() 
    { 
        emit receiverStateChanged();
        ui->receiverState->setText(ui->receiverState->text() == "Start" ? "Stop" : "Start");
    });

    connect(receiver, &Receiver::dataReceived, this, &rrv_viewer::dataReceived);
}

rrv_viewer::~rrv_viewer()
{
    delete ui;
}

void rrv_viewer::updateDataPanelValue()
{
    ui->viewData->appendPlainText(dataPanelValue);
    ui->viewData->verticalScrollBar()->setValue(ui->viewData->verticalScrollBar()->maximum());
    QCoreApplication::processEvents();
}
void rrv_viewer::updateDataPanelValue(const QString& dataPanelValue)
{
    ui->viewData->appendPlainText(dataPanelValue);
    ui->viewData->verticalScrollBar()->setValue(ui->viewData->verticalScrollBar()->maximum());
    QCoreApplication::processEvents();
}

