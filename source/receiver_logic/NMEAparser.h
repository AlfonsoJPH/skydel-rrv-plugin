#pragma once

#include <QString>
#include <QStringList>
#include <iostream>

enum MessagesTypes
{
    GGA, // Time, position and fix type data.
    GLL,  // Latitude, longitude, UTC time of position fix and status.
    GSA, // GPS receiver operating mode, satellites used in the position solution, and DOP values.
    GSV, // The number of GPS satellites in view satellite ID numbers, elevation, azimuth, and SNR values.
    MSS, // Signal-to-noise ratio, signal strength, frequency, and bit rate from a radio-beacon receiver.
    RMC, // Time, date, position, course and speed data.
    VTG, // Course and speed information relative to the ground.
    ZDA, // PPS timing message (synchronized to PPS).
    OK = 150 // OK to send message
};

struct UTC_Time
{
    int HOUR = 0;
    int MINUTE = 0;
    int SECOND = 0;
    int MILISECOND = 0;
};

struct GGA
{
    QString messageID = "Unknown"; // GGA protocol header
    UTC_Time utcTime; // hhmmss.sss
    QString latitude = ""; // ddmm.mmmm
    QChar nsIndicator;  // N=north or S=south
    QString longitude = ""; // dddmm.mmmm
    QChar ewIndicator; // E=east or W=west
    int positionFixIndicator; // See Table 1-4
    int satellitesInUse; // Range 0 to 12
    double hdop;// Horizontal Dilution of Precision
    double altitudeMSL; // meters
    QChar altitudeUnits; // meters
    double geoidSeparation; // meters
    QChar geoidSeparationUnits; // meters
    double ageDiffCorr; // second Null fields when DGPS is not used
    QString diffRefStationID; // 0000
    QString checksum; // *18
    bool valid_checksum;
};

struct GLL
{
    QString messageID = "Unknown"; // GLL protocol header
    QString latitude = "Unknown"; // ddmm.mmmm
    QChar nsIndicator;  // N=north or S=south
    QString longitude; // dddmm.mmmm
    QChar ewIndicator; // E=east or W=west
    UTC_Time utcTime; // hhmmss.sss
    QChar status; // A=Data valid or V=Data not valid
    QString checksum; // *18
    bool valid_checksum;
};

struct GSA
{
    QString messageID = "Unknown"; // GSA protocol header
    QString mode; // M=Manual, A=Automatic
    int fixMode = 0; // 1=Fix not available, 2=2D, 3=3D
    int prn[12]; // PRN numbers of satellites used in the position solution
    double pdop; // Position Dilution of Precision
    double hdop; // Horizontal Dilution of Precision
    double vdop; // Vertical Dilution of Precision
    QString checksum; // *18
    bool valid_checksum;
};

static bool checkMessageIntegrity(const QString& message, const QString& checksum) {
    int checksumValue = 0;
    for (int i = 1; i < message.size() && message[i] != '*'; ++i) {
        checksumValue ^= message[i].unicode();
    }
    QString checksumCalculated = QString("%1").arg(checksumValue, 2, 16, QLatin1Char('0')).toUpper();
    return checksumCalculated == checksum;
}

static UTC_Time UTCparser(QString time) {
    UTC_Time utc;
    utc.HOUR = time.mid(0, 2).toInt();
    utc.MINUTE = time.mid(2, 2).toInt();
    utc.SECOND = time.mid(4, 2).toInt();
    utc.MILISECOND = time.mid(6, 2).toInt();
    return utc;
}


static MessagesTypes getMessageType(const QStringList& message){
    try
    {
        QString ID = message[0];
        if (ID.endsWith("GGA")) return MessagesTypes::GGA;
        if (ID.endsWith("GLL")) return MessagesTypes::GLL;
        if (ID.endsWith("GSA")) return MessagesTypes::GSA;
        if (ID.endsWith("GSV")) return MessagesTypes::GSV;
        if (ID.endsWith("MSS")) return MessagesTypes::MSS;
        if (ID.endsWith("RMC")) return MessagesTypes::RMC;
        if (ID.endsWith("VTG")) return MessagesTypes::VTG;
        if (ID.endsWith("ZDA")) return MessagesTypes::ZDA;
        return MessagesTypes::OK;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

static bool GGAparser(const QStringList& message, struct GGA& data) {
    struct GGA gga = {};	
    bool valid = true;
    try
    {
        if (message.size() >= 15) {
            gga.messageID = message[0];
            gga.utcTime = UTCparser(message[1]);
            gga.latitude = message[2]; // can be empty
            if (message[3].size() > 0) {
                gga.nsIndicator = message[3][0]; // can be empty
            }
            gga.longitude = message[4]; // can be empty
            if (message[5].size() > 0) {
                gga.ewIndicator = message[5][0]; // can be empty
            }
            // has to be a number between 0 and 8
            bool success = false;
            int fixIndicator = message[6].toInt(&success);
            if (success)
            {
                gga.positionFixIndicator = fixIndicator;
                if (fixIndicator <= 0 || fixIndicator >= 8)
                {
                    valid = false;
                }
            }
            else
            {
                valid = false;
            }

            // has to be a number between 0 and 12
            gga.positionFixIndicator = fixIndicator;
            if (message[7].isEmpty())
            {
                valid = false;
            }
            else
            {
                int satellitesInUse = message[7].toInt(&success);
                if (success)
                {
                    gga.satellitesInUse = satellitesInUse;
                    if (satellitesInUse <= 0 || satellitesInUse >= 12)
                    {
                        valid = false;
                    }
                }
                else
                {
                    valid = false;
                }
                gga.satellitesInUse = satellitesInUse;
            }
            gga.hdop = message[8].toDouble(&success);
            if (!success)
            {
                valid = false;
            }
            gga.altitudeMSL = message[9].isEmpty() ? 0.0 : message[9].toDouble(); // can be empty
            gga.altitudeUnits = message[10].isEmpty() ? ' ' : message[10][0]; // can be empty
            gga.geoidSeparation = message[11].isEmpty() ? 0.0 : message[11].toDouble(); // can be empty
            gga.geoidSeparationUnits = message[12].isEmpty() ? ' ' : message[12][0]; // can be empty
            gga.ageDiffCorr = message[13].isEmpty() ? 0.0 : message[13].toDouble(); // can be empty
            gga.diffRefStationID = message[14].isEmpty() ? "Unknown" : message[14]; // can be empty
            // gga.checksum = message[15];
            // if (gga.checksum.size() != 3)
            // {
            //     valid = false;
            // }

            // if(checkMessageIntegrity(message.join(","), gga.checksum))
            // {
            //     gga.valid_checksum = true;
            // }
            // else
            // {
            //     gga.valid_checksum = false;
            //     valid = false;
            // }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    data = gga;

    return valid;
}

static bool GLLparser(const QStringList& message, struct GLL& data) {
    struct GLL gll = {};
    bool valid = true;
    try
    {
        if (message.size() >= 7) {
            gll.messageID = message[0];
            gll.latitude = message[1]; // can be empty
            if (message[2].size() > 0) {
                gll.nsIndicator = message[2][0]; // can be empty
            }
            gll.longitude = message[3]; // can be empty
            if (message[4].size() > 0) {
                gll.ewIndicator = message[4][0]; // can be empty
            }
            gll.utcTime = UTCparser(message[5]);
            gll.status = message[6][0]; // can be empty
            // gll.checksum = message[7];
            // if (gll.checksum.size() != 3)
            // {
            //     valid = false;
            // }

            // if(checkMessageIntegrity(message.join(","), gll.checksum))
            // {
            //     gll.valid_checksum = true;
            // }
            // else
            // {
            //     gll.valid_checksum = false;
            //     valid = false;
            // }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        valid = false;
    }
    data = gll;

    return valid;
}

static bool GSAparser(const QStringList& message, struct GSA& data) {
    struct GSA gsa = {};
    bool valid = true;
    try
    {
        if (message.size() >= 18) {
            gsa.messageID = message[0];
            gsa.mode = message[1];
            gsa.fixMode = message[2].toInt();
            for (int i = 3; i < 15; ++i) {
                gsa.prn[i - 3] = message[i].toInt();
            }
            gsa.pdop = message[15].toDouble();
            gsa.hdop = message[16].toDouble();
            gsa.vdop = message[17].toDouble();
            // gsa.checksum = message[18];
            // if (gsa.checksum.size() != 3)
            // {
            //     valid = false;
            // }

            // if(checkMessageIntegrity(message.join(","), gsa.checksum))
            // {
            //     gsa.valid_checksum = true;
            // }
            // else
            // {
            //     gsa.valid_checksum = false;
            //     valid = false;
            // }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        valid = false;
    }
    data = gsa;

    return valid;
}