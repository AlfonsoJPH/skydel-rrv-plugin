#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QLocale>
#include <QScrollBar>
#include <QComboBox>
#include <QSerialPortInfo>

rrv_viewer::rrv_viewer(RRVConfiguration *config, QWidget *parent) :
    QWidget(parent), config(config),
    ui(new Ui::rrv_viewer)
{
    ui->setupUi(this);
    ui->baudRate->clear();
    ui->baudRate->addItems({"110", "300", "600", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200", "128000", "256000"});

    ui->portName->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->portName->addItem(info.portName());
    }
    ui->portName->setCurrentText(config->portName);
    ui->fileLogState->setChecked(config->fileLogging);
    ui->baudRate->setCurrentText(QString::number(config->baudRate));
    ui->logPath->setText(config->logPath);
    ui->viewData->setReadOnly(true);

    connect(ui->portName, &QComboBox::currentTextChanged, this, [this](const QString &text) { emit portNameChanged(text); });
    connect(ui->baudRate, &QComboBox::currentTextChanged, this, [this](const QString &text) { emit baudRateChanged(text.toInt()); });
    connect(ui->logPath, &QLineEdit::textChanged, this, [this](const QString &text) { emit logPathChanged(text); });
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

