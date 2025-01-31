#pragma once

#include "ubx_base_parser.h"

#define UBX_CONFIG_GNSS 0x3E

class UbxM8Parser : public UbxGenericParser
{
public:
    QByteArray setGNSSConstellations(const std::vector<int> &constellations) override{

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
            int constellation = constellations[i];
            uint8_t repeatBlock[8] = {};
            
            // only readable
            repeatBlock[1] = 0x04;
            repeatBlock[2] = 0x08;
            repeatBlock[3] = 0x00;

            // flags
            repeatBlock[4] = 0x00; // Reserved
            repeatBlock[6] = 0x00; // Reserved
            repeatBlock[7] = 0x01; // Enable

            switch (constellation)
            {
                case GNSSConstellations::GPS:
                    repeatBlock[0] = 0x00; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::SBAS:
                    repeatBlock[0] = 0x01; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations::GALILEO:
                    repeatBlock[0] = 0x02; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::BEIDOU:
                    repeatBlock[0] = 0x03; // Type
                    repeatBlock[5] = 0x01; // sigCfgMask
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::IMES:
                    repeatBlock[0] = 0x04; // gnssId 
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations::QZSS:
                    repeatBlock[0] = 0x05; // gnssId 
                    repeatBlock[5] = 0x01; // sigCfgMask
                    break;
                case GNSSConstellations::GLONASS:
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

    QByteArray setDynamicPlatformModel(const QString &model) override{

    };

    QByteArray setUpdateRate(const uint &rate) override{

    };

    QByteArray setStartupMode(const StartUpModes &mode) override{

    };
    AvailableReceiverConfigs parseCodes(const uint8_t &id){
        AvailableReceiverConfigs code = GNSS_CONSTELLATIONS;
        switch (id)
        {
        case UBX_CONFIG_GNSS: 
            code = AvailableReceiverConfigs::GNSS_CONSTELLATIONS;
            break;        
        default:
            code = AvailableReceiverConfigs::GNSS_CONSTELLATIONS;
            break;
        }
        return code;
    };


    bool checkResponse(const QString &response, QHash<AvailableReceiverConfigs, StateMessage> &pendingConfigACKs) override{
        bool hasACK = false;
        if (!response.isEmpty())
        {
            QStringList messages = response.split("\n");
            for (uint8_t i = 0; i < messages.size(); i++)
            {
                // puede ser nmea o ubx
                if (!messages[i].startsWith("$"))
                {
                    // Check if it is an ubx message
                    if (messages[i].at(0) == 0xB5 && messages[i].at(1) == 0x62) {
                        struct ubxMessage message = {};
                        message.classID = static_cast<uint8_t>(messages[i].at(2).toLatin1());
                        message.messageID = static_cast<uint8_t>(messages[i].at(3).toLatin1());
                        message.payload = new uint8_t[2];
                        message.payload[0] = static_cast<uint8_t>(messages[i].at(6).toLatin1());
                        message.payload[1] = static_cast<uint8_t>(messages[i].at(7).toLatin1());
                        
                        // check if is an ACK/NACK
                        if (message.classID == 0x05)
                        {
                            StateMessage ack = NACK;
                            // check if it's an ACK
                            if (static_cast<uint8_t>(messages[i].at(3).toLatin1()) == 0x01)
                            {
                                ack = ACK;
                            }

                            pendingConfigACKs[parseCodes(message.payload[1])] = ack;
                            hasACK = true;
                        }
                    }
                }
            }
            
        }

        return hasACK;
        
    };

};