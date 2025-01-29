#pragma once

#include "ubx_base_parser.h"

#define UBX_CONFIG_GNSS 0x3E

class UbxM8Parser : public UbxGenericParser
{
public:
    static QByteArray setGNSSConstellations(const std::list<GNSSConstellations> &constellations) override{

        if(constellations.size() == 0){
            return {};
        }

        ubxMessage message = {};

        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_GNSS;
        message.length = 4  + 8 * constellations.size();

        uint8_t *payload = new uint8_t[message.length];

        payload[0] = 0x00;
        payload[1] = 0x00;
        payload[2] = 0x00;
        payload[3] = constellations.size();

        bool hasMajorGNSS = false;
        for(int i = 0; i < constellations.size(); i++){
            GNSSConstellations constellation = constellations[i];
            uint8_t repeatBlock[8] = {};
            
            // only readable
            repeatBlock[1] = 0x00;
            repeatBlock[2] = 0x00;
            repeatBlock[3] = 0x00;

            // flags
            repeatBlock[4] = 0x00; // Reserved
            repeatBlock[6] = 0x00; // Reserved
            repeatBlock[7] = 0x01; // Enable

            switch (constellation)
            {
                case GNSSConstellations.GPS:
                    repeatBlock[0] = 0x00; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations.SBAS:
                    repeatBlock[0] = 0x01; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations.GALILEO:
                    repeatBlock[0] = 0x02; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations.BEIDOU:
                    repeatBlock[0] = 0x03; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations.IMES:
                    repeatBlock[0] = 0x04; // gnssId 
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations.QZSS:
                    repeatBlock[0] = 0x05; // gnssId 
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations.GLONASS:
                    repeatBlock[0] = 0x06; // gnssId 
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                default:
                    break;
            }

            // Sets the repeat block in the payload
            for(int j = 0; j < 8; j++){
                payload[4 + 8 * i + j] = repeatBlock[j];
            }
        }
        if (!hasMajorGNSS)
        {
            return {};
        }

        message.payload = payload;

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));

        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;

        return getMessage(message);
        
        
    };

    static QByteArray setDynamicPlatformModel(const QString &model) override{

    };

    static QByteArray setUpdateRate(const uint &rate) override{

    };

    static QByteArray setStartupMode(const StartUpModes &mode) override{

    };
};

