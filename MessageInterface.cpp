#include <iostream>
#include "MessageInterface.h"

using namespace std;

namespace UartMessageInterface
{
    UartMessageCallbackManagement *UartMessageCallbackManagement::_Instance = NULL;

    eSensorType strSensor(const char *input)
    {
        string str(input);
        if (str == string("ALL"))
            return SensorAll;
        if (str == string("TEMPERATURE"))
            return SensorTemperature;
        if (str == string("CO2"))
            return SensorCO2;
        if (str == string("HUMIDITY"))
            return SensorHumidity;
        if (str == string("CONDUCTIVITY"))
            return SensorConductivity;
        return SensorInvalid;
    }

    string strSensor(eSensorType input)
    {
        switch (input)
        {
        case SensorAll:
            return "ALL";
        case SensorTemperature:
            return "TEMPERATURE";
        case SensorCO2:
            return "CO2";
        case SensorHumidity:
            return "HUMIDITY";
        case SensorConductivity:
            return "CONDUCTIVITY";
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

    UartMessageCallbackManagement &UartMessageCallbackManagement::getInstance()
    {
        if (_Instance == NULL)
        {
            _Instance = new UartMessageCallbackManagement;
        }
        return *_Instance;
    }

    void UartMessageCallbackManagement::registerRequestGetSensorCallBack(eSensorType sensorType, const string &name, const CallBackRequestGetSensor &func)
    {
        _callBackListRequestGetSensor.insert(make_pair(make_tuple(sensorType, name), func));
    }

    void UartMessageCallbackManagement::registerResponseGetSensorCallBack(eSensorType sensorType, const string &name, const CallBackResponseGetSensor &func)
    {
        _callBackListResponseGetSensor.insert(make_pair(make_tuple(sensorType, name), func));
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

    void UartSensorMessage::appendRequest(eSensorType sensorType, const string &name)
    {
        XMLElement *child = NULL;
        switch (sensorType)
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

        const XMLAttribute *count = _command->FindAttribute("COUNT");
        if (count == NULL)
        {
            _command->SetAttribute("COUNT", 1);
        }
        else
        {
            _command->SetAttribute("COUNT", count->UnsignedValue() + 1);
        }
    }

    string UartSensorMessage::sendMessage()
    {
        XMLPrinter xmlPrint;
        _xmlDoc.Print(&xmlPrint);
        string buf = xmlPrint.CStr();
        appendCheckSum(buf);

        // Serial.println(buf);
        cout << buf << endl;

        return buf;
    }

    bool isUartMessage(const string &message)
    {
        // TODO
        return true;
    }

    void processUartMessage(const string &message)
    {
        if (isUartMessage(message) == false)
        {
            cout << "This message is not Uart message" << endl;
            return;
        }

        if (verityCheckSum(message) == false)
        {
            cout << "CheckSum error" << endl;
            return;
        }

        XMLDocument xmlDoc;
        xmlDoc.Parse(message.c_str(), message.size() - 1);

        XMLPrinter xmlPrint;
        xmlDoc.Print(&xmlPrint);
        string buf = xmlPrint.CStr();
        cout << buf << endl;

        XMLNode *root = xmlDoc.FirstChild();
        handleRequestMessage(root);
        handleResponseMessage(root);
    }

    void handleRequestMessage(const XMLNode *root)
    {
        if (string(root->Value()) != string("REQUEST"))
            return;

        cout << "REQUEST Message arrived" << endl;

        const XMLElement *command = root->FirstChildElement();
        if (string(command->Value()) != string("GET_SENSOR"))
        {
            uint32_t count = command->UnsignedAttribute("COUNT");
            if (count == 0)
                return;

            const XMLElement *firstElem = command->FirstChildElement();
            dispatchRequestGenSensor(firstElem);

            for (uint32_t i = 1; i < count; i++)
            {
                const XMLElement *nextElem = firstElem->NextSiblingElement();
                dispatchRequestGenSensor(nextElem);
            }
        }
    }

    void dispatchRequestGenSensor(const XMLElement *elem)
    {
        eSensorType type = strSensor(elem->Name());
        string name = elem->Attribute("NAME");
        
        map<tuple<eSensorType, string>, UartMessageCallbackManagement::CallBackRequestGetSensor>::iterator iter = 
        UartMessageCallbackManagement::getInstance()._callBackListRequestGetSensor.find(make_tuple(type, name));

        if(iter == UartMessageCallbackManagement::getInstance()._callBackListRequestGetSensor.end()) return;
        iter->second(type, name);
    }

    void handleResponseMessage(const XMLNode *root)
    {
        if (string(root->Value()) != string("RESPONSE"))
            return;

        cout << "RESPONSE Message arrived" << endl;
    }

}; // namespace UartMessageInterface