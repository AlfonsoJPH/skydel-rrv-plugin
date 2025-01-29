#pragma once

#include <QString>
#include <QByteArray>
#include <list>

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


class ProprietaryParser {
public:
        static QByteArray setGNSSConstellations(const std::list<GNSSConstellations> &constellations){};

        static QByteArray setDynamicPlatformModel(const QString &model){};
        
        static QByteArray setUpdateRate(const uint &rate){};

        static QByteArray setStartupMode(const StartUpModes &mode){};

        static bool checkResponse(const QString &response){};
};




