#pragma once

#include "ubx_base_parser.h"

#define UBX_CONFIG_VALSET 0x8A
#define UBX_CONFIG_STARTUP 0x04
#define UBX_CONSTELLATIONS_SIZE 7

class UbxM9Parser : public UbxGenericParser
{
public:
    QByteArray setGNSSConstellations(const std::vector<int> &constellations) override{
        if(constellations.size() == 0){
            return {};
        }

        ubxMessage message = {};

        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_VALSET;
        message.length = 4  + 8 * constellations.size();

        uint8_t *payload = new uint8_t[message.length];

        payload[0] = 0x00;
        payload[1] = 0x01; // update only on ram
        payload[2] = 0x00; // reserved
        payload[3] = 0x00; // reserved
        
        int payloadIndex = 4;
        for(int i = 0; i < constellations.size(); i++){
            switch (i)
            {
                case GNSSConstellations::GPS:
                    payload[payloadIndex] = 0x1031001f; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // GPS ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x10310001; // L1C/A
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // L1C/A ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // GPS DISABLE
                    }
                    break;
                case GNSSConstellations::SBAS:
                    payload[payloadIndex] = 0x10310020; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // SBAS ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x10310005; // L1C/A
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // L1C/A ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // SBAS DISABLE
                    }
                    break;
                case GNSSConstellations::GALILEO:
                    payload[payloadIndex] = 0x10310021; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // SBAS ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x10310007; // E1
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // E1 ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // E1 DISABLE
                    }
                    break;
                case GNSSConstellations::BEIDOU:
                    payload[payloadIndex] = 0x10310022; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // BEIDOU ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x1031000d; // B1
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // B1 ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // BEIDOU
                    }
                    break;
                case GNSSConstellations::QZSS:
                    payload[payloadIndex] = 0x10310024; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // QZSS ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x10310012; // L1C/A
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // L1C/A ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // QZSS DISABLE
                    }
                    break;
                case GNSSConstellations::GLONASS:
                    payload[payloadIndex] = 0x10310025; // Type
                    payloadIndex += 4;

                    if(constellations[i] > -1) {
                        payload[payloadIndex++] = 0x01; // GLONASS ENABLE
                        if (constellations[i] > 0){
                            if (constellations[i] == 1){
                                payload[payloadIndex] = 0x10310018; // L1C/A
                                payloadIndex += 4;
                                payload[payloadIndex++] = 0x01; // L1C/A ENABLE
                            }
                        }
                    } else {
                        payload[payloadIndex++] = 0x00; // GLONASS DISABLE
                    }
                    break;
                default:
                    break;
            }
        }

        message.payload = payload;

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));

        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;

        return getMessage(message);
    };

    QByteArray setDynamicPlatformModel(const PlatformModels &model) override{
        ubxMessage message = {};
        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_VALSET;

        message.length = 4 + 4+ 1; // 4 bytes for the header + 4 bytes for the ID + 1 byte for value
        uint8_t *payload = new uint8_t[message.length];

        payload[0] = 0x00;
        payload[1] = 0x01; // update only on ram
        payload[2] = 0x00; // reserved
        payload[3] = 0x00; // reserved
        
        int payloadIndex = 4;
        payload[payloadIndex] = 0x20110021; // CFG-NAVSPG-DYNMODEL

        payloadIndex += 4;
        payload[payloadIndex] = model & 0xFF;

        message.payload = payload;

        std::pair<uint8_t, uint8_t> checksum = generateChecksum(getMessage(message));
        message.checksum_a = checksum.first;
        message.checksum_b = checksum.second;
        return getMessage(message);
    };

    QByteArray setUpdateRate(const uint16_t &rate) override{
        ubxMessage message = {};
        message.classID = UBX_CONFIG;
        message.messageID = UBX_CONFIG_VALSET;

        message.length = 4 + 4+ 2; // 4 bytes for the header + 4 bytes for the ID + 1 byte for value
        uint8_t *payload = new uint8_t[message.length];

        payload[0] = 0x00;
        payload[1] = 0x01; // update only on ram
        payload[2] = 0x00; // reserved
        payload[3] = 0x00; // reserved
        
        int payloadIndex = 4;
        payload[payloadIndex] = 0x30210001; // CFG-NAVSPG-DYNMODEL

        payloadIndex += 4;
        uint16_t updateRate = 1000 / rate;
        payload[payloadIndex] = updateRate;

        message.payload = payload;

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
            message1 = "Message size: " + QString::number(messages.size()) + " ";
            for (uint8_t i = 0; i < messages.size(); i++)
            {
                if (messages[i].startsWith("$") || messages[i].isEmpty())
                {
                    continue;
                }


                if (!messages[i].startsWith("$"))
                {
                   // Check if it is an ubx message
                    if (messages[i].size() > 2 && static_cast<uint8_t>(messages[i].at(0).toLatin1()) == 0x00 && static_cast<uint8_t>(messages[i].at(1).toLatin1()) == 0x62) {
                        struct ubxMessage message = {};
                        message.classID = static_cast<uint8_t>(messages[i].at(2).toLatin1());
                        // message.messageID = static_cast<uint8_t>(messages[i].at(3).toLatin1());

                        
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

};