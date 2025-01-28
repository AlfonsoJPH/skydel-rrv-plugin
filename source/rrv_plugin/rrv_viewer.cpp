#include "rrv_viewer.h"
#include "../receiver_logic/NMEAparser.h"
#include "ecef.h"
#include "lla.h"
#include "ui_rrv_viewer.h"
#include <QComboBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QLocale>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QSpinBox>
#include <QString>
#include <QtMath>

rrv_viewer::rrv_viewer(QSharedPointer<RRVConfiguration> config,
                       QSharedPointer<Sdx::Ecef> receiverPosition,
                       QSharedPointer<Sdx::Ecef> simulationPosition,
                       QWidget *parent)
    : QWidget(parent), config(config), ui(new Ui::rrv_viewer) {
  this->receiverPosition = receiverPosition;
  this->simulationPosition = simulationPosition;
  ui->setupUi(this);
  ui->baudRate->clear();
  ui->baudRate->addItems({"110", "300", "600", "1200", "2400", "4800", "9600",
                          "14400", "19200", "38400", "57600", "115200",
                          "128000", "256000"});

  ui->portName->clear();
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->portName->addItem(info.portName());
  }
  ui->portName->setCurrentText(config->serialPortName);
  ui->baudRate->setCurrentText(QString::number(config->baudRate));

  ui->serialLogPath->setText(config->serialLogPath);
  ui->serialFileLogState->setChecked(config->serialFileLogging);

  ui->serialLogAddressValue->setText(
      config->serialNetworkLogAddress.toString());
  ui->serialLogPortValue->setValue(config->serialNetworkLogPort);
  ui->serialNetworkLogState->setChecked(config->serialNetworkLogging);

  ui->simulationLogPath->setText(config->simulationLogPath);
  ui->simulationFileLogState->setChecked(config->simulationFileLogging);
  ui->simulationLogAddressValue->setText(
      config->simulationNetworkLogAddress.toString());
  ui->simulationLogPortValue->setValue(config->simulationNetworkLogPort);
  ui->simulationNetworkLogState->setChecked(config->simulationNetworkLogging);

  ui->receiverLogPath->setText(config->receiverLogPath);
  ui->receiverFileLogState->setChecked(config->receiverFileLogging);
  ui->receiverLogAddressValue->setText(
      config->receiverNetworkLogAddress.toString());
  ui->receiverLogPortValue->setValue(config->receiverNetworkLogPort);
  ui->receiverNetworkLogState->setChecked(config->receiverNetworkLogging);

  ui->viewData->setReadOnly(true);

  connect(ui->portName, &QComboBox::currentTextChanged, this,
          [this](const QString &text) { this->config->serialPortName = text; });

  ui->baudRate->addItem("Other");
  connect(ui->baudRate, &QComboBox::currentTextChanged, this,
          [this](const QString &text) {
            int value = 9600;
            if (text == "Other") {
              bool ok = false;
              value =
                  QInputDialog::getInt(this, tr("Specify Baud Rate"),
                                       tr("Baud Rate:"), 0, 0, 1000000, 1, &ok);
              while (!ok) {
                value = QInputDialog::getInt(this, tr("Specify Baud Rate"),
                                             tr("Baud Rate:"), 0, 0, 1000000, 1,
                                             &ok);
              }
              ui->baudRate->setCurrentText(QString::number(value));
            } else {
              value = text.toInt();
            }

            this->config->baudRate = value;
          });
  connect(ui->serialLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->serialLogPath->setText(path);
    this->config->serialLogPath = path;
    emit serialConfigChanged();
  });
  connect(ui->serialLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->serialLogPath = text;
            emit serialConfigChanged();
          });
  connect(ui->serialFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->config->serialFileLogging = state; });

  connect(ui->serialLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->serialNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->serialLogPortValue, QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          [this](int value) { this->config->serialNetworkLogPort = value; });

  connect(ui->serialNetworkLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->config->serialNetworkLogging = state; });

  connect(ui->receiverState, &QPushButton::clicked, this,
          [this]() { emit receiverStateChanged(); });

  connect(ui->receiverLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->receiverLogPath->setText(path);
    this->config->receiverLogPath = path;
    emit observerConfigChanged();
  });
  connect(ui->receiverLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->receiverLogPath = text;
            emit observerConfigChanged();
          });
  connect(ui->receiverFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->config->receiverFileLogging = state; });
  connect(ui->receiverLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->receiverNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->receiverLogPortValue, QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          [this](int value) { this->config->receiverNetworkLogPort = value; });
  connect(ui->receiverNetworkLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->config->receiverNetworkLogging = state; });

  connect(ui->simulationLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->simulationLogPath->setText(path);
    this->config->simulationLogPath = path;
    emit observerConfigChanged();
  });
  connect(ui->simulationLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->simulationLogPath = text;
            emit observerConfigChanged();
          });
  connect(ui->simulationFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->config->simulationFileLogging = state; });
  connect(ui->simulationLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->config->simulationNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->simulationLogPortValue,
          QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            this->config->simulationNetworkLogPort = value;
          });
  connect(
      ui->simulationNetworkLogState, &QCheckBox::stateChanged, this,
      [this](int state) { this->config->simulationNetworkLogging = state; });
}

rrv_viewer::~rrv_viewer() { delete ui; }

void rrv_viewer::updateDataPanelValue(const QString &dataPanelValue) {
  ui->viewData->appendPlainText(dataPanelValue);
  ui->viewData->verticalScrollBar()->setValue(
      ui->viewData->verticalScrollBar()->maximum());
  QCoreApplication::processEvents();
}
void rrv_viewer::receiverStateChanges(bool state) {
  if (state) {
    ui->receiverState->setText("Disconnect");
  } else {
    ui->receiverState->setText("Connect");
  }
}
void rrv_viewer::dataReceived(const QString &data) {
  QStringList messages = data.split("\n");
  updateDataPanelValue(data);
  // remove newline at the end of the message
  messages.removeLast();
  struct GGA lastValidGGA;
  struct GSA lastValidGSA;

  try {
    for (int i = 0; i < messages.size(); i++) {
      QString message = messages[i];

      QStringList messageList = message.split(",");
      MessagesTypes type = getMessageType(messageList);
      if (type == MessagesTypes::GGA) {
        struct GGA gga;
        if (GGAparser(messageList, gga) && !gga.latitude.isEmpty() &&
            !gga.longitude.isEmpty()) {
          lastValidGGA = gga;
        }
        if (lastValidGGA.messageID == "Unknwon") {
          lastValidGGA = gga;
        }
      } else if (type == MessagesTypes::GSA) {
        struct GSA gsa;
        if (GSAparser(messageList, gsa)) {
          lastValidGSA = gsa;
        }
        if (lastValidGSA.messageID == "Unknwon") {
          lastValidGSA = gsa;
        }
      }
    }

    if (lastValidGGA.messageID != "Unknwon" &&
        !lastValidGGA.latitude.isEmpty() && !lastValidGGA.longitude.isEmpty()) {
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
      double latitudeSecondValue =
          lastValidGGA.latitude.toDouble() - latitudeFirstValue * 100;
      double latitudeValue = latitudeFirstValue + latitudeSecondValue / 60.0;
      if (lastValidGGA.nsIndicator == 'S') {

        latitudeValue *= -1.0;
      }
      QString latitude = QString::number(latitudeValue, 'f', 6);

      int longitudeFirstValue = lastValidGGA.longitude.toDouble() / 100;
      double longitudeSecondValue =
          lastValidGGA.longitude.toDouble() - longitudeFirstValue * 100.0;
      double longitudeValue = longitudeFirstValue + longitudeSecondValue / 60.0;
      if (lastValidGGA.ewIndicator == 'W') {
        longitudeValue *= -1.0;
      }
      QString longitude = QString::number(longitudeValue, 'f', 6);

      QString altitudeMSL = QString::number(lastValidGGA.altitudeMSL, 'f', 2);
      double altitudeEllipsoidValue =
          lastValidGGA.altitudeMSL + lastValidGGA.geoidSeparation;
      QString altitudeEllipsoid =
          QString::number((altitudeEllipsoidValue), 'f', 2);

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

      receiverPosition->x = ecefCoordinates.x;
      receiverPosition->y = ecefCoordinates.y;
      receiverPosition->z = ecefCoordinates.z;

      if (lastValidGSA.messageID != "Unknwon" && lastValidGSA.fixMode != 0) {
        QString PDOP = QString::number(lastValidGSA.pdop, 'f', 2);
        QString VDOP = QString::number(lastValidGSA.vdop, 'f', 2);
        ui->PDOPValue->setText(PDOP);
        ui->VDOPValue->setText(VDOP);
        QString fixMode = QString::number(lastValidGSA.fixMode);
        if (lastValidGSA.fixMode > 1)
          fixMode = fixMode + "D";
        ui->fixValue->setText(fixMode);
      }
      ui->xValueSimulator->setText(
          QString::number(simulationPosition->x, 'f', 2));
      ui->yValueSimulator->setText(
          QString::number(simulationPosition->y, 'f', 2));
      ui->zValueSimulator->setText(
          QString::number(simulationPosition->z, 'f', 2));

      ui->xValueReceiver->setText(QString::number(receiverPosition->x, 'f', 2));
      ui->yValueReceiver->setText(QString::number(receiverPosition->y, 'f', 2));
      ui->zValueReceiver->setText(QString::number(receiverPosition->z, 'f', 2));
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}