#pragma once

#include <QString>
#include "./propetary_parser.h"

#define UBX_CONFIG 0x06
#define ACK 0x05
#define NACK 0x07

struct ubxMessage {
    uint8_t classID;
    uint8_t messageID;
    uint16_t length;
    uint8_t *payload;
    uint8_t checksum_a;
    uint8_t checksum_b;
}

class UbxGenericParser : public ProprietaryParser
{
public:
    static std::pair<uint8_t, uint8_t> generateChecksum(const QByteArray &message)
    {
        uint8_t CK_A = 0, CK_B = 0;
        for (int i = 0; i < message.size(); i++)
        {
            CK_A = CK_A + static_cast<uint8_t>(message[i]);
            CK_B = CK_B + CK_A;
        }
        return std::make_pair(CK_A, CK_B);
    }

    static QByteArray getMessage(struct ubxMessage &message)
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
        msg.append(checksum_a);
        msg.append(checksum_b);
        return msg;
    }

    static bool checkResponse(const QString &response) override{
        if (response.contains())
        {
            return false;
        }
        else if (response.contains("ACK"))
        {
            return true;
        }

        return false;
        
    }
    
};
