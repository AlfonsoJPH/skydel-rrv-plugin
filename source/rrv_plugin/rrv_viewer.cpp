#include "rrv_viewer.h"
#include "../receiver_logic/nmea_parser.h"
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

rrv_viewer::rrv_viewer(QSharedPointer<RRVConfiguration> pluginConfig,
                       QSharedPointer<Sdx::Ecef> receiverPosition,
                       QSharedPointer<Sdx::Ecef> simulationPosition,
                       QWidget *parent)
    : QWidget(parent), pluginConfig(pluginConfig), ui(new Ui::rrv_viewer) {
  this->receiverPosition = receiverPosition;
  this->simulationPosition = simulationPosition;
  receiverConfig.m_parser = new UbxM8Parser();

  ui->setupUi(this);
  ui->platformModelComboBox->clear();
  ui->platformModelComboBox->addItems(receiverConfig.m_parser->getAvailablePlatformModels());

  ui->baudRate->clear();
  ui->baudRate->addItems({"110", "300", "600", "1200", "2400", "4800", "9600",
                          "14400", "19200", "38400", "57600", "115200",
                          "128000", "256000"});
  ui->portName->clear();
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->portName->addItem(info.portName());
  }
  ui->portName->setCurrentText(pluginConfig->serialPortName);
  ui->baudRate->setCurrentText(QString::number(pluginConfig->baudRate));

  ui->serialLogPath->setText(pluginConfig->serialLogPath);
  ui->serialFileLogState->setChecked(pluginConfig->serialFileLogging);

  ui->serialLogAddressValue->setText(
      pluginConfig->serialNetworkLogAddress.toString());
  ui->serialLogPortValue->setValue(pluginConfig->serialNetworkLogPort);
  ui->serialNetworkLogState->setChecked(pluginConfig->serialNetworkLogging);

  ui->simulationLogPath->setText(pluginConfig->simulationLogPath);
  ui->simulationFileLogState->setChecked(pluginConfig->simulationFileLogging);
  ui->simulationLogAddressValue->setText(
      pluginConfig->simulationNetworkLogAddress.toString());
  ui->simulationLogPortValue->setValue(pluginConfig->simulationNetworkLogPort);
  ui->simulationNetworkLogState->setChecked(pluginConfig->simulationNetworkLogging);

  ui->receiverLogPath->setText(pluginConfig->receiverLogPath);
  ui->receiverFileLogState->setChecked(pluginConfig->receiverFileLogging);
  ui->receiverLogAddressValue->setText(
      pluginConfig->receiverNetworkLogAddress.toString());
  ui->receiverLogPortValue->setValue(pluginConfig->receiverNetworkLogPort);
  ui->receiverNetworkLogState->setChecked(pluginConfig->receiverNetworkLogging);

  ui->viewData->setReadOnly(true);

  // Connect signals of the interface
  connect(ui->portName, &QComboBox::currentTextChanged, this,
          [this](const QString &text) { this->pluginConfig->serialPortName = text; });

  ui->baudRate->addItem("Other");
  connect(ui->baudRate, &QComboBox::currentTextChanged, this,
          [this](const QString &text) {
            int value = 9600;
            if (text == "Other") {
              bool ok = false;
              foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
                ui->portName->addItem(info.portName());
              }
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

            this->pluginConfig->baudRate = value;
          });
  connect(ui->serialLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->serialLogPath->setText(path);
    this->pluginConfig->serialLogPath = path;
    emit serialConfigChanged();
  });
  connect(ui->serialLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->serialLogPath = text;
            emit serialConfigChanged();
          });
  connect(ui->serialFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->pluginConfig->serialFileLogging = state; });

  connect(ui->serialLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->serialNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->serialLogPortValue, QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          [this](int value) { this->pluginConfig->serialNetworkLogPort = value; });

  connect(ui->serialNetworkLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->pluginConfig->serialNetworkLogging = state; });

  connect(ui->receiverState, &QPushButton::clicked, this,
          [this]() { emit receiverStateChanged(); });

  connect(ui->receiverLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->receiverLogPath->setText(path);
    this->pluginConfig->receiverLogPath = path;
    emit observerConfigChanged();
  });
  connect(ui->receiverLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->receiverLogPath = text;
            emit observerConfigChanged();
          });
  connect(ui->receiverFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->pluginConfig->receiverFileLogging = state; });
  connect(ui->receiverLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->receiverNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->receiverLogPortValue, QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          [this](int value) { this->pluginConfig->receiverNetworkLogPort = value; });
  connect(ui->receiverNetworkLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->pluginConfig->receiverNetworkLogging = state; });

  connect(ui->simulationLogPathButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->simulationLogPath->setText(path);
    this->pluginConfig->simulationLogPath = path;
    emit observerConfigChanged();
  });
  connect(ui->simulationLogPath, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->simulationLogPath = text;
            emit observerConfigChanged();
          });
  connect(ui->simulationFileLogState, &QCheckBox::stateChanged, this,
          [this](int state) { this->pluginConfig->simulationFileLogging = state; });
  connect(ui->simulationLogAddressValue, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            this->pluginConfig->simulationNetworkLogAddress = QHostAddress(text);
          });
  connect(ui->simulationLogPortValue,
          QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            this->pluginConfig->simulationNetworkLogPort = value;
          });
  connect(
      ui->simulationNetworkLogState, &QCheckBox::stateChanged, this,
      [this](int state) { this->pluginConfig->simulationNetworkLogging = state; });

  // Receiver messages configuration
  connect(ui->sendReceiverConfigButton, &QPushButton::clicked, this,
      [this]() {
      emit receiverConfigChanged(this->receiverConfig);
      receiverConfig.GNSSConstellationChanged = false;
      receiverConfig.updateRateChanged = false;
      receiverConfig.platformModelChanged = false;
      receiverConfig.startupModeChanged = false;

    }
  );

  connect(ui->gpsConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        if(ui->gpsConstellationSignal1->isChecked()) {
          this->receiverConfig.GNSSConstellations[0] = 1;
        } else if(ui->gpsConstellationSignal2->isChecked()) {
          this->receiverConfig.GNSSConstellations[0] = 2;
        }
      } else {
        this->receiverConfig.GNSSConstellations[0] = -1;
      }
    }
  );

  connect(ui->gpsConstellationSignal1, &QCheckBox::toggled, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (ui->gpsConstellationCheckBox){
        if (state) {
          this->receiverConfig.GNSSConstellations[0] = 1;
        } else {
          this->receiverConfig.GNSSConstellations[0] = 2;
        }
      }
    }
  );

  connect(ui->sbasConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
          this->receiverConfig.GNSSConstellations[1] = 1;
      } else {
        this->receiverConfig.GNSSConstellations[1] = -1;
      }
    }
  );

  connect(ui->galileoConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        if(ui->galileoConstellationSignal1->isChecked()) {
          this->receiverConfig.GNSSConstellations[2] = 1;
        } else if(ui->galileoConstellationSignal2->isChecked()) {
          this->receiverConfig.GNSSConstellations[2] = 2;
        }
      } else {
        this->receiverConfig.GNSSConstellations[2] = -1;
      }
    }
  );

  connect(ui->galileoConstellationSignal1, &QCheckBox::toggled, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (ui->galileoConstellationCheckBox){
        if (state) {
          this->receiverConfig.GNSSConstellations[2] = 1;
        } else {
          this->receiverConfig.GNSSConstellations[2] = 2;
        }
      }
    }
  );

  connect(ui->beidouConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        if(ui->beidouConstellationSignal1->isChecked()) {
          this->receiverConfig.GNSSConstellations[3] = 1;
        } else if(ui->beidouConstellationSignal2->isChecked()) {
          this->receiverConfig.GNSSConstellations[3] = 2;
        }
      } else {
        this->receiverConfig.GNSSConstellations[3] = -1;
      }
    }
  );

  connect(ui->beidouConstellationSignal1, &QCheckBox::toggled, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (ui->beidouConstellationCheckBox){
        if (state) {
          this->receiverConfig.GNSSConstellations[3] = 1;
        } else {
          this->receiverConfig.GNSSConstellations[3] = 2;
        }
      }
    }
  );

  connect(ui->imesConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
          this->receiverConfig.GNSSConstellations[4] = 1;
      } else {
        this->receiverConfig.GNSSConstellations[4] = -1;
      }
    }
  );

  connect(ui->glonassConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        if(ui->glonassConstellationSignal1->isChecked()) {
          this->receiverConfig.GNSSConstellations[5] = 1;
        } else if(ui->glonassConstellationSignal2->isChecked()) {
          this->receiverConfig.GNSSConstellations[5] = 2;
        }
      } else {
        this->receiverConfig.GNSSConstellations[5] = -1;
      }
    }
  );

  connect(ui->glonassConstellationSignal1, &QCheckBox::toggled, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (ui->glonassConstellationCheckBox){
        if (state) {
          this->receiverConfig.GNSSConstellations[5] = 1;
        } else {
          this->receiverConfig.GNSSConstellations[5] = 2;
        }
      }
    }
  );
  connect(ui->qzssConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        if(ui->qzssConstellationSignal1->isChecked()) {
          this->receiverConfig.GNSSConstellations[6] = 1;
        } else if(ui->qzssConstellationSignal2->isChecked()) {
          this->receiverConfig.GNSSConstellations[6] = 2;
        }
      } else {
        this->receiverConfig.GNSSConstellations[6] = -1;
      }
    }
  );

  connect(ui->qzssConstellationSignal1, &QCheckBox::toggled, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (ui->qzssConstellationCheckBox){
        if (state) {
          this->receiverConfig.GNSSConstellations[6] = 1;
        } else {
          this->receiverConfig.GNSSConstellations[6] = 2;
        }
      }
    }
  );
  connect(ui->navicConstellationCheckBox, &QCheckBox::stateChanged, this,
    [this](int state) {
      this->receiverConfig.GNSSConstellationChanged = true;
      if (state) {
        this->receiverConfig.GNSSConstellations[7] = 1;
      } else {
        this->receiverConfig.GNSSConstellations[7] = -1;
      }
    }
  );

  connect(ui->baudRateValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
    [this](double value) {
      this->receiverConfig.updateRateChanged = true;
      this->receiverConfig.updateRate = value;
    }
  );

  connect(ui->platformModelComboBox, &QComboBox::currentTextChanged, this,
    [this](const QString &text) {
      this->receiverConfig.platformModelChanged = true;
      this->receiverConfig.platformModel = text;
    }
  );

  connect(ui->coldStartButton, &QPushButton::clicked, this,
    [this]() {
      this->receiverConfig.startupModeChanged = true;
      this->receiverConfig.startupMode = COLD;
  });

  connect(ui->warmStartButton, &QPushButton::clicked, this,
    [this]() {
      this->receiverConfig.startupModeChanged = true;
      this->receiverConfig.startupMode = WARM;
  });

  connect(ui->hotStartButton, &QPushButton::clicked, this,
    [this]() {
      this->receiverConfig.startupModeChanged = true;
      this->receiverConfig.startupMode = HOT;
  });

  connect(ui->portName, &QComboBox::currentTextChanged, this,
    [this](const QString &text) {
      this->receiverConfig.platformModelChanged = true;
      this->receiverConfig.platformModel = text;
    });


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
void rrv_viewer::on_reloadPortsButton_clicked()
{
  ui->portName->clear();
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->portName->addItem(info.portName());
  }
}
