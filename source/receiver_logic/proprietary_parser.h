#pragma once

#include <QString>
#include <QByteArray>
#include <vector>
enum AvailableReceiverConfigs {
  GNSS_CONSTELLATIONS,
  PLATFORM_MODEL,
  UPDATE_RATE,
  STARTUP_MODE
};

enum GNSSConstellations {
    GPS,
    GLONASS,
    GALILEO,
    BEIDOU,
    QZSS,
    IRNSS,
    SBAS,
    IMES
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
        
        virtual QByteArray setGNSSConstellations(const std::vector<int> &constellations){ //returns 6
          return QByteArray(1, 0x06);
        }

        virtual QByteArray setDynamicPlatformModel(const QString &model){};
        
        virtual QByteArray setUpdateRate(const uint &rate){};

        virtual QByteArray setStartupMode(const StartUpModes &mode){};

        virtual bool checkResponse(const QString &response, QHash<AvailableReceiverConfigs, StateMessage> &pendingConfigACKs){};
};  





