#include <iostream>
#include "UartMessageInterface.h"

using namespace std;

namespace UartMessageInterface
{
    eDataType str2EnumDataType(const char *input)
    {
        string str(input);
        if (str == string("SENSOR_ALL"))
            return SensorAll;
        if (str == string("TEMPERATURE"))
            return SensorTemperature;
        if (str == string("CO2"))
            return SensorCO2;
        if (str == string("HUMIDITY"))
            return SensorHumidity;
        if (str == string("CONDUCTIVITY"))
            return SensorConductivity;
        if (str == string("CONTROL_ALL"))
            return ControlAll;
        if (str == string("CONTROL0"))
            return Control0;
        if (str == string("CONTROL1"))
            return Control1;
        if (str == string("CONTROL2"))
            return Control2;
        if (str == string("DATETIME"))
            return DateTime;
        return Invalid;
    }

    string enum2Str(eDataType input)
    {
        switch (input)
        {
        case SensorAll:
            return "SENSOR_ALL";
        case SensorTemperature:
            return "TEMPERATURE";
        case SensorCO2:
            return "CO2";
        case SensorHumidity:
            return "HUMIDITY";
        case SensorConductivity:
            return "CONDUCTIVITY";
        case ControlAll:
            return "CONTROL_ALL";
        case Control0:
            return "CONTROL0";
        case Control1:
            return "CONTROL1";
        case Control2:
            return "CONTROL2";
        case DateTime:
            return "DATETIME";
        case Invalid:
        default:
            return "-";
        }
    }

    string getCurrentTime()
    {
        char sResult[32] = {
            0,
        };
        time_t result = time(nullptr);
        strftime(sResult, sizeof(sResult), "%FT%T", localtime(&result));

        // cout << sResult << endl;
        return sResult;
    }

    uint8_t getCheckSum(const string &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        uint8_t checkSum = (sum ^ 0xFF) + 1;

        // cout << "CheckSum : " << (int)checkSum << endl;
        return checkSum;
    }

    void appendCheckSum(string &message)
    {
        message.push_back((char)getCheckSum(message));
    }

    bool verityCheckSum(const string &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        // cout << "Verity : " << (sum == 0) << endl;
        return (sum == 0);
    }

}; // namespace UartMessageInterface