#pragma once

#include "ubx_base_parser.h"
#include <QStringList>
#include <QString>


const QStringList platformModels = {
    "Portable",
    "",
    "Stationary",
    "Pedestrian",
    "Automotive",
    "Sea",
    "Airborne <1G",
    "Airborne <2G",
    "Airborne <4G",
    "Wrist",
    "Bike"
};

#define UBX_CONFIG_GNSS 0x3E
#define UBX_CONFIG_MODEL 0x24
#define UBX_CONFIG_RATE 0x08
#define UBX_CONFIG_STARTUP 0x04
#define UBX_CONSTELLATIONS_SIZE 7

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
        message.length = 4  + 8 * UBX_CONSTELLATIONS_SIZE;

        uint8_t *payload = new uint8_t[message.length];

        payload[0] = 0x00;
        payload[1] = 0x20;
        payload[2] = 0x20;
        payload[3] = UBX_CONSTELLATIONS_SIZE;
        
        bool hasMajorGNSS = false;
        int constellationActual = 0;
        for(int i = 0; i < constellations.size(); i++){
            uint8_t repeatBlock[8] = {};
            
            // only readable
            repeatBlock[3] = 0x00;

            // flags
            repeatBlock[5] = 0x00; // Reserved
            repeatBlock[7] = 0x01; // Reserved 

            if (constellations.size() > i && constellations[i] >= 0)
            {
                repeatBlock[4] = 0x01; // Enable
            }
            else
            {
                repeatBlock[4] = 0x00; // Disable
            }


            switch (i)
            {
                case GNSSConstellations::GPS:
                    repeatBlock[0] = 0x00; // Type
                    repeatBlock[1] = 0x08; // Min Channels
                    repeatBlock[2] = 0x10; // Max Channels
                    repeatBlock[6] = 0x01; // L1C/A
                    if (constellations.size() > i && constellations[i] > 0 && constellations[i] == 2) {
                            repeatBlock[6] = 0x10; // L2C
                    }

                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::SBAS:
                    repeatBlock[0] = 0x01; // Type
                    repeatBlock[1] = 0x01; // Min Channels
                    repeatBlock[2] = 0x03; // Max Channels
                    repeatBlock[6] = 0x01; // L1C/A
                    break;
                case GNSSConstellations::GALILEO:
                    repeatBlock[0] = 0x02; // Type
                    repeatBlock[1] = 0x04; // Min Channels
                    repeatBlock[2] = 0x08; // Max Channels
                    repeatBlock[6] = 0x01; // E1
                    if (constellations.size() > i && constellations[i] > 0 && constellations[i] == 2) {
                            repeatBlock[6] = 0x20; // E5b
                    }
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::BEIDOU:
                    repeatBlock[0] = 0x03; // Type
                    repeatBlock[1] = 0x08; // Min Channels
                    repeatBlock[2] = 0x10; // Max Channels
                    repeatBlock[6] = 0x01; // B1
                    if (constellations.size() > i && constellations[i] > 0 && constellations[i] == 2) {
                            repeatBlock[6] = 0x10; // B2
                    }
                    hasMajorGNSS = true;
                    break;
                case GNSSConstellations::IMES:
                    repeatBlock[0] = 0x04; // gnssId 
                    repeatBlock[1] = 0x00; // Min Channels
                    repeatBlock[2] = 0x08; // Max Channels
                    repeatBlock[6] = 0x03; // L1 (According to UBX M8 0x01)
                    break;
                case GNSSConstellations::QZSS:
                    repeatBlock[0] = 0x05; // gnssId 
                    repeatBlock[1] = 0x00; // Min Channels
                    repeatBlock[2] = 0x03; // Max Channels
                    repeatBlock[6] = 0x05; // L1C/A (According to UBX M8 0x01) 
                    if (constellations.size() > i && constellations[i] > 0){
                        if (constellations[i] == 2) {
                            repeatBlock[6] = 0x06; // L1S (According to UBX M8 0x04)
                        } else if (constellations[i] == 3) {
                            repeatBlock[6] = 0x10; // L2C
                        }
                    }
                    break;
                case GNSSConstellations::GLONASS:
                    repeatBlock[0] = 0x06; // gnssId 
                    repeatBlock[1] = 0x08; // Min Channels
                    repeatBlock[2] = 0x0D; // Max Channels
                    repeatBlock[6] = 0x01; // L1
                    if (constellations.size() > i && constellations[i] > 0  && constellations[i] == 2) {
                        repeatBlock[6] = 0x10; // L2
                    }
                    hasMajorGNSS = true;
                    break;
                default:
                    continue;
                    break;
            }

            // Sets the repeat block in the payload
            for(int j = 0; j < 8; j++){
                payload[4 + 8 * constellationActual + j] = repeatBlock[j];
            }
            constellationActual++;
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
        ubxMessage message = {};
        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_MODEL;
        message.length = 36;

        std::vector<uint8_t> payload(message.length, 0);
        payload[0] = 0xFF;
        payload[1] = 0xFF;
        payload[2] = getPlatformModelIndex(model);
        payload[3] = 0x03; // FixMode
        payload[4] = 0x00; // FixedAlt
        payload[5] = 0x00; // FixedAlt
        payload[6] = 0x00; // FixedAlt
        payload[7] = 0x00; // FixedAlt
        payload[8] = 0x10; // fixedAltVar
        payload[9] = 0x27; // fixedAltVar
        payload[10] = 0x00; // fixedAltVar
        payload[11] = 0x00; // fixedAltVar
        payload[12] = 0x05; // MinElev
        payload[13] = 0x00; // DrLimit
        payload[14] = 0xFA; // pDop
        payload[15] = 0x00; // pDop
        payload[16] = 0xFA; // tDop
        payload[17] = 0x00; // tDop
        payload[18] = 0x64; // pAcc
        payload[19] = 0x00; // pAcc
        payload[20] = 0x5E; // tAcc
        payload[21] = 0x01; // tAcc
        payload[22] = 0x00; // staticHoldThresh
        payload[23] = 0x3C; // dgnssTimeout
        payload[24] = 0x00; // cnoThreshNumSVs
        payload[25] = 0x00; // cnoThresh
        payload[26] = 0x00; // reserved1
        payload[27] = 0x00; // reserved1
        payload[28] = 0x00; // staticHoldMaxDist
        payload[29] = 0x00; // staticHoldMaxDist
        payload[30] = 0x03; // utcStandard
        payload[31] = 0x00; // reserved2
        payload[32] = 0x00; // reserved2
        payload[33] = 0x00; // reserved2
        payload[34] = 0x00; // reserved2
        payload[35] = 0x00; // reserved2
        


        message.payload = payload.data();

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));
        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;
        return getMessage(message);
    };

    QByteArray setUpdateRate(const uint16_t &rate) override{
        ubxMessage message = {};
        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_RATE;
        message.length = 6;
        uint16_t measRate = 1000/rate;

        std::vector<uint8_t> payload(message.length, 0x00);        
        
        payload[0] = measRate & 0xFF;
        payload[1] = (measRate >> 8) & 0xFF;
        payload[2] = 0x01;
        payload[4] = 0x00;

        message.payload = payload.data();

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));
        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;
        return getMessage(message);
    };

    QByteArray setStartupMode(const StartUpModes &mode) override{
        ubxMessage message = {};
        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_STARTUP;
        message.length = 4;

        std::vector<uint8_t> payload(message.length, 0);
        switch (mode)
        {
        case StartUpModes::WARM:
            payload[0] = 0x00;
            payload[1] = 0x01;
            break;
        case StartUpModes::COLD:
            payload[0] = 0xFF;
            payload[1] = 0xFF;
            break;
        default: // HOT
            payload[0] = 0x00;
            payload[1] = 0x00;
            break;
        }
        
        payload[2] = 0x01;
        message.payload = payload.data();

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));

        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;

        return getMessage(message);
    };

    AvailableReceiverConfigs parseCodes(const uint8_t &id){
        AvailableReceiverConfigs code = GNSS_CONSTELLATIONS;
        switch (id)
        {
        case UBX_CONFIG_GNSS: 
            code = AvailableReceiverConfigs::GNSS_CONSTELLATIONS;
            break;
        case UBX_CONFIG_MODEL:
            code = AvailableReceiverConfigs::PLATFORM_MODEL;
            break;
        case UBX_CONFIG_RATE:
            code = AvailableReceiverConfigs::UPDATE_RATE;
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
                if (!messages[i].isEmpty() && !messages[i].startsWith("$"))
                {
                    // Check if it is an ubx message
                    if (messages[i].size() > 2 && static_cast<uint8_t>(messages[i].at(0).toLatin1()) == 0x00 && static_cast<uint8_t>(messages[i].at(1).toLatin1()) == 0x62) {
                        struct ubxMessage message = {};
                        message.classID = static_cast<uint8_t>(messages[i].at(2).toLatin1());

                        // check if is an ACK/NACK
                        if (messages[i].size() > 3)
                        {
                            StateMessage ack = NACK;
                            // check if it's an ACK
                            if (static_cast<uint8_t>(messages[i].at(3).toLatin1()) == 0x01)
                            {
                                ack = ACK;
                            }

                            pendingConfigACKs[AvailableReceiverConfigs::GNSS_CONSTELLATIONS] = ack;
                            hasACK = true;
                        }
                    }
                }
            }
            
        }

        return hasACK;
        
    };

    QStringList getAvailablePlatformModels() const override{
        QStringList result = platformModels;
        result.removeAt(result.indexOf(""));
        return result;
    }

    int getPlatformModelIndex(const QString &model) const override{
        return platformModels.indexOf(model);
    }
    
};