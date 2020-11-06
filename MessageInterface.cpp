#include <iostream>
#include "MessageInterface.h"

using namespace std;

namespace UartMessageInterface
{
    UartMessageCallbackManagement *UartMessageCallbackManagement::_Instance = NULL;

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

    UartMessageCallbackManagement &UartMessageCallbackManagement::getInstance()
    {
        if (_Instance == NULL)
        {
            _Instance = new UartMessageCallbackManagement;
        }
        return *_Instance;
    }

    void UartMessageCallbackManagement::registerRequestGetSensorCallBack(eSensorType type, const string &name, const CallBackRequestGetSensor &func)
    {
        _callBackListRequestGetSensor.insert(make_pair(make_tuple(type, name), func));
    }

    void UartMessageCallbackManagement::registerResponseGetSensorCallBack(eSensorType type, const string &name, const CallBackResponseGetSensor &func)
    {
        _callBackListResponseGetSensor.insert(make_pair(make_tuple(type, name), func));
    }

    UartSensorMessage::UartSensorMessage(uint32_t sequenceId, eMessageType messageType, eCommandType commandType)
        : _command(NULL)
    {
        XMLNode *root = NULL;
        switch (messageType)
        {
        case Request:
            root = _xmlDoc.NewElement("REQUEST");
            break;
        case Response:
            root = _xmlDoc.NewElement("RESPONSE");
            break;
        case Acknowledge:
            root = _xmlDoc.NewElement("ACKNOWLEDGE");
            break;
        case Subscribe:
            root = _xmlDoc.NewElement("SUBSCRIBE");
            break;
        default:
            throw invalid_argument("Invalid MessageType");
        }

        _xmlDoc.InsertFirstChild(root);

        XMLElement *seqId = _xmlDoc.NewElement("SEQUENCE_ID");
        seqId->SetText(sequenceId);
        root->InsertEndChild(seqId);

        _command = _xmlDoc.NewElement("GET_SENSOR");
        root->InsertEndChild(_command);
    }

    void UartSensorMessage::appendRequestAll()
    {
        _command->DeleteChildren();
        _command->SetText("ALL");
    }

    void UartSensorMessage::appendRequest(eSensorType type, const string &name)
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
        _command->InsertEndChild(child);
    }

    void UartSensorMessage::sendMessage()
    {
        XMLPrinter xmlPrint;
        _xmlDoc.Print(&xmlPrint);
        string buf = xmlPrint.CStr();
        appendCheckSum(buf);

        // Serial.println(buf);
        cout << buf << endl;
    }

    void processUartMessage(const string &message)
    {
        if(verityCheckSum(message) == false)
        {
            cout << "CheckSum error" << endl;
        }
        
        XMLDocument xmlDoc;
        xmlDoc.Parse(message.c_str(), message.size() - 1);
    }

}; // namespace UartMessageInterface