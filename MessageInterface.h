#ifndef _MESSAGE_INTERFACE_H_
#define _MESSAGE_INTERFACE_H_

#include <string>
#include <ctime>
#include <functional>
#include <map>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace UartMessageInterface
{
    string getCurrentTime();

    uint8_t getCheckSum(const string &message);
    void appendCheckSum(string &message);
    bool verityCheckSum(const string &message);

    typedef enum
    {
        SensorAll = 0,
        SensorTemperature,
        SensorCO2,
        SensorHumidity,
        SensorConductivity,
    } eSensorType;

    typedef enum
    {
        Request = 0,
        Response,
        Acknowledge,
        Subscribe,
    } eMessageType;

    typedef enum
    {
        GetSensor = 0,
        GetControl,
        SetControl
    } eCommandType;

    class UartMessageCallbackManagement
    {
    public:
        static UartMessageCallbackManagement *_Instance;

        typedef function<void(eSensorType, const string &)> CallBackRequestGetSensor;
        map<tuple<eSensorType, string>, CallBackRequestGetSensor> _callBackListRequestGetSensor;
        typedef function<void(eSensorType, const string &, const string &)> CallBackResponseGetSensor;
        map<tuple<eSensorType, string>, CallBackResponseGetSensor> _callBackListResponseGetSensor;

        static UartMessageCallbackManagement &getInstance();
        void registerRequestGetSensorCallBack(eSensorType type, const string &name, const CallBackRequestGetSensor &func);
        void registerResponseGetSensorCallBack(eSensorType type, const string &name, const CallBackResponseGetSensor &func);
    };

    class UartSensorMessage
    {
    public:
        UartSensorMessage(uint32_t sequenceId, eMessageType messageType, eCommandType commandType);

        void appendRequestAll();
        void appendRequest(eSensorType type, const string &name);

        template <typename ValueType>
        void appendResponse(eSensorType type, const string &name, const ValueType &value)
        {
            XMLElement *child = NULL;
            switch (type)
            {
            case SensorTemperature:
                child = _xmlDoc.NewElement("TEMPERATURE");
                break;
            case SensorCO2:
                child = _xmlDoc.NewElement("CO2");
                break;
            case SensorHumidity:
                child = _xmlDoc.NewElement("HUMIDITY");
                break;
            case SensorConductivity:
                child = _xmlDoc.NewElement("CONDUCTIVITY");
                break;
            default:
                return;
            }

            child->SetAttribute("NAME", name.c_str());
            child->SetAttribute("VALUE", value);
            _command->InsertEndChild(child);
        }

        void sendMessage();

    private:
        XMLDocument _xmlDoc;
        XMLElement *_command;
        UartSensorMessage();
    };

    void processUartMessage(const string &message);

}; // namespace UartMessageInterface

#endif