#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QLocale>
#include <QScrollBar>
#include <QComboBox>
#include <QSerialPortInfo>
#include <QInputDialog>
#include <QFileDialog>
#include "../receiver_logic//NMEAparser.h"
#include <QString>

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

    ui->baudRate->addItem("Other");
    connect(ui->baudRate, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        int value = 9600;
        if (text == "Other") {
            bool ok = false;
            value = QInputDialog::getInt(this, tr("Specify Baud Rate"),
                                             tr("Baud Rate:"), 0, 0, 1000000, 1, &ok);
            while (!ok) {
                value = QInputDialog::getInt(this, tr("Specify Baud Rate"),
                                             tr("Baud Rate:"), 0, 0, 1000000, 1, &ok);
            }
            ui->baudRate->setCurrentText(QString::number(value));
        } else {
            value = text.toInt();
        }

        emit baudRateChanged(value);
    });
    connect(ui->logPathButton, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
        ui->logPath->setText(path);
        emit logPathChanged(path);
    });
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

void rrv_viewer::dataReceived(const QString& data)
{
    QStringList messages = data.split("\n");
    updateDataPanelValue(data);
    // remove newline at the end of the message
    messages.removeLast();
    struct GGA lastValidGGA;
    struct GSA lastValidGSA;


    try{
        for(int i = 0; i < messages.size(); i++) {
            QString message = messages[i];

            QStringList messageList = message.split(",");
            MessagesTypes type = getMessageType(messageList);
            if (type == MessagesTypes::GGA)
            {
                struct GGA gga;
                if (GGAparser(messageList, gga))
                {
                    lastValidGGA = gga;
                }
                if (lastValidGGA.messageID == "Unknwon")
                {
                    lastValidGGA = gga;
                }
            }
            else if (type == MessagesTypes::GSA)
            {
                struct GSA gsa;
                if (GSAparser(messageList, gsa))
                {
                    lastValidGSA = gsa;
                }
                if (lastValidGSA.messageID == "Unknwon")
                {
                    lastValidGSA = gsa;
                }
            }
        }

        if (lastValidGGA.messageID != "Unknwon")
        {
            QString hour = QString::number(lastValidGGA.utcTime.HOUR);
            if (hour.size() == 1) {
                hour = "0" + hour;
            }
            QString minute = QString::number(lastValidGGA.utcTime.MINUTE);
            if (minute.size() == 1) {
                minute = "0" + minute;
            }
            QString second = QString::number(lastValidGGA.utcTime.SECOND);
            if (second.size() == 1) {
                second = "0" + second;
            }

            int latitudeFirstValue = lastValidGGA.latitude.toFloat() / 100;
            float latitudeSecondValue = lastValidGGA.latitude.toFloat() - latitudeFirstValue * 100;
            float latitudeValue = latitudeFirstValue + latitudeSecondValue / 60.0;
            if (lastValidGGA.nsIndicator == 'S') {

                latitudeValue *= -1.0;
            }
            QString latitude = QString::number(latitudeValue, 'f', 6);

            int longitudeFirstValue = lastValidGGA.longitude.toFloat() / 100;
            float longitudeSecondValue = lastValidGGA.longitude.toFloat() - longitudeFirstValue * 100.0;
            float longitudeValue = longitudeFirstValue + longitudeSecondValue / 60.0;
            if (lastValidGGA.ewIndicator == 'W') {
                longitudeValue *= -1.0;
            }
            QString longitude = QString::number(longitudeValue, 'f', 6);

            QString altitudeMSL = QString::number(lastValidGGA.altitudeMSL, 'f', 2);
            QString altitudeEllipsoid = QString::number((lastValidGGA.altitudeMSL + lastValidGGA.geoidSeparation), 'f', 2);
            
            ui->positionValue->setText(latitude + ", " + longitude);
            ui->altitudeMSLValue->setText(altitudeMSL);
            ui->altitudeEllipsoidValue->setText(altitudeEllipsoid);
            ui->utcValue->setText(hour + ":" + minute + ":" + second);
            ui->HDOPValue->setText(QString::number(lastValidGGA.hdop, 'f', 2));
        }

        if(lastValidGSA.messageID != "Unknwon")
        {
            QString PDOP = QString::number(lastValidGSA.pdop, 'f', 2);
            QString VDOP = QString::number(lastValidGSA.vdop, 'f', 2);
            ui->PDOPValue->setText(PDOP);
            ui->VDOPValue->setText(VDOP);
            QString fixMode = QString::number(lastValidGSA.fixMode);
            if (lastValidGSA.fixMode > 1) fixMode = fixMode + "D";            
            ui->fixValue->setText(fixMode);
        }

        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}