#pragma once

#include "./proprietary_parser.h"

#define UBX_CONFIG 0x06
#define UBX_ACK 0x05

struct ubxMessage {
    uint8_t classID;
    uint8_t messageID;
    uint16_t length;
    uint8_t *payload;
    uint8_t checksum_a;
    uint8_t checksum_b;
};

class UbxGenericParser : public ProprietaryParser
{
public:
    std::pair<uint8_t, uint8_t> generateChecksum(const QByteArray &message)
    {
        uint8_t CK_A = 0, CK_B = 0;
        for (int i = 2; i < message.size(); i++) // Start from 2 to skip 0xB5 and 0x62
        {
            CK_A = CK_A + static_cast<uint8_t>(message[i]);
            CK_B = CK_B + CK_A;

            CK_A = CK_A & 0xFF;
            CK_B = CK_B & 0xFF;
        }
        return std::make_pair(CK_A, CK_B);
    }

    QByteArray getMessage(struct ubxMessage &message)
    {
        QByteArray msg;
        msg.append(0xB5);
        msg.append(0x62);
        msg.append(message.classID);
        msg.append(message.messageID);
        // length is little endian
        msg.append(message.length & 0xFF);
        msg.append((message.length >> 8) & 0xFF);
        for (int i = 0; i < message.length; i++)
        {
            msg.append(message.payload[i]);
        }
        std::pair<uint8_t, uint8_t> checksum = generateChecksum(msg);
        msg.append(checksum.first);
        msg.append(checksum.second);
        return msg;
    }

    QByteArray setGNSSConstellations(const std::vector<int> &constellations) override{}
    QByteArray setDynamicPlatformModel(const QString &model) override{}
    QByteArray setUpdateRate(const uint16_t &rate) override{}

};
