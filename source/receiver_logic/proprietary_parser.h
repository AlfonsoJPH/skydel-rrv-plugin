#pragma once

#include <QString>
#include <QByteArray>
#include <vector>

#define MAX_NUMBER_OF_CONSTELLATIONS 8

enum AvailableReceiverConfigs {
  GNSS_CONSTELLATIONS,
  PLATFORM_MODEL,
  UPDATE_RATE,
  STARTUP_MODE
};

enum GNSSConstellations {
    GPS,
    SBAS,
    GALILEO,
    BEIDOU,
    QZSS,
    GLONASS,
    IMES,
    NAVIC
};

enum PlatformModels {
    PORTABLE,
    NAN,
    STATIONARY,
    PEDESTRIAN,
    AUTOMOTIVE,
    SEA,
    AIRBORNE_1G,
    AIRBORNE_2G,
    AIRBORNE_4G,
    WRIST_WATCH,
    BIKE,
    ROBOTIC_LAWN_MOWER,
    ELECTRIC_KICK_SCOOTER
};

enum StartUpModes {
    COLD,
    WARM,
    HOT
};

enum StateMessage {
  PENDING,
  ACK,
  NACK
};

class ProprietaryParser {
public:
        
        virtual QByteArray setGNSSConstellations(const std::vector<int> &constellations){};

        virtual QByteArray setDynamicPlatformModel(const PlatformModels &model){};
        
        virtual QByteArray setUpdateRate(const uint16_t &rate){};

        virtual QByteArray setStartupMode(const StartUpModes &mode){};

        virtual bool checkResponse(const QString &response, QHash<AvailableReceiverConfigs, StateMessage> &pendingConfigACKs){};
};  

class receiverConfiguration {
  public:
    bool GNSSConstellationChanged;
    bool platformModelChanged;
    bool updateRateChanged;
    bool startupModeChanged;

    std::vector<int> GNSSConstellations;
    QString platformModel;
    uint updateRate;
    StartUpModes startupMode;

    receiverConfiguration() {
      GNSSConstellationChanged = false;
      platformModelChanged = false;
      updateRateChanged = false;
      startupModeChanged = false;
      GNSSConstellations = std::vector<int>(MAX_NUMBER_OF_CONSTELLATIONS, -1);
      platformModel = PORTABLE;
      updateRate = -1;
      startupMode = COLD;
    }

    int size() {
      return 4;
    }
};