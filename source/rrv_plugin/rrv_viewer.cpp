#include "rrv_viewer.h"
#include "ui_rrv_viewer.h"
#include <QLocale>
#include <QScrollBar>
#include <QComboBox>
#include <QSpinBox>
#include <QSerialPortInfo>
#include <QInputDialog>
#include <QFileDialog>
#include "../receiver_logic//NMEAparser.h"
#include <QString>
#include "lla.h"
#include "ecef.h"
#include <QtMath>

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
    ui->baudRate->setCurrentText(QString::number(config->baudRate));

    ui->logPath->setText(config->logPath);
    ui->fileLogState->setChecked(config->fileLogging);

    ui->logAddressValue->setText(config->networkLogAddress);
    ui->logPortValue->setValue(config->networkLogPort.toUShort());
    ui->networkLogState->setChecked(config->networkLogging);

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
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
        ui->logPath->setText(path);
        emit logPathChanged(path);
    });
    connect(ui->logPath, &QLineEdit::textChanged, this, [this](const QString &text) { emit logPathChanged(text); });
    connect(ui->fileLogState, &QCheckBox::stateChanged, this, [this](int state) { emit fileLoggingChanged(state); });

    connect(ui->logAddressValue, &QLineEdit::textChanged, this, [this](const QString &text) { emit logNetworkChanged(text, QString::number(ui->logPortValue->value())); });
    connect(ui->logPortValue, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { emit logNetworkChanged(ui->logAddressValue->text(), QString::number(value)); });
    connect(ui->networkLogState, &QCheckBox::stateChanged, this, [this](int state) { emit networkLoggingChanged(state); });
    
    connect(ui->receiverState, &QPushButton::clicked, this, [this]() 
    { 
        emit receiverStateChanged();
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
void rrv_viewer::receiverStateChanges(bool state)
{
    if (state)
    {
        ui->receiverState->setText("Disconnect");
    }
    else
    {
        ui->receiverState->setText("Connect");
    }
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
                if (GGAparser(messageList, gga) && !gga.latitude.isEmpty() && !gga.longitude.isEmpty())
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

        if (lastValidGGA.messageID != "Unknwon" && !lastValidGGA.latitude.isEmpty() && !lastValidGGA.longitude.isEmpty())
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

            int latitudeFirstValue = lastValidGGA.latitude.toDouble() / 100;
            double latitudeSecondValue = lastValidGGA.latitude.toDouble() - latitudeFirstValue * 100;
            double latitudeValue = latitudeFirstValue + latitudeSecondValue / 60.0;
            if (lastValidGGA.nsIndicator == 'S') {

                latitudeValue *= -1.0;
            }
            QString latitude = QString::number(latitudeValue, 'f', 6);

            int longitudeFirstValue = lastValidGGA.longitude.toDouble() / 100;
            double longitudeSecondValue = lastValidGGA.longitude.toDouble() - longitudeFirstValue * 100.0;
            double longitudeValue = longitudeFirstValue + longitudeSecondValue / 60.0;
            if (lastValidGGA.ewIndicator == 'W') {
                longitudeValue *= -1.0;
            }
            QString longitude = QString::number(longitudeValue, 'f', 6);

            QString altitudeMSL = QString::number(lastValidGGA.altitudeMSL, 'f', 2);
            double altitudeEllipsoidValue = lastValidGGA.altitudeMSL + lastValidGGA.geoidSeparation;
            QString altitudeEllipsoid = QString::number((altitudeEllipsoidValue), 'f', 2);
            
            ui->positionValue->setText(latitude + ", " + longitude);
            ui->altitudeMSLValue->setText(altitudeMSL);
            ui->altitudeEllipsoidValue->setText(altitudeEllipsoid);
            ui->utcValue->setText(hour + ":" + minute + ":" + second);
            ui->HDOPValue->setText(QString::number(lastValidGGA.hdop, 'f', 2));

            // Geodetic to ECEF
            Sdx::Lla llaCoordinates;
            llaCoordinates.lat = qDegreesToRadians(latitudeValue);
            llaCoordinates.lon = qDegreesToRadians(longitudeValue);
            llaCoordinates.alt = altitudeEllipsoidValue;

            Sdx::Ecef ecefCoordinates;
            llaCoordinates.toEcef(ecefCoordinates);

            ui->xValueReceiver->setText(QString::number(ecefCoordinates.x, 'f', 2));
            ui->yValueReceiver->setText(QString::number(ecefCoordinates.y, 'f', 2));
            ui->zValueReceiver->setText(QString::number(ecefCoordinates.z, 'f', 2));
            if(lastValidGSA.messageID != "Unknwon" && lastValidGSA.fixMode != 0)
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

        

        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}