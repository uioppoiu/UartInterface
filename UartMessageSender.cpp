#include <iostream>
#include "UartMessageSender.h"

using namespace std;

namespace UartMessageInterface
{
    UartMessageSender::UartMessageSender(eMessageType messageType, eCommandType commandType)
        : _seqId(0), _command(NULL)
    {
        XMLElement *root = NULL;
        switch (messageType)
        {
        case Request:
            root = _xmlDoc.NewElement("REQUEST");
            break;
        case Response:
            root = _xmlDoc.NewElement("RESPONSE");
            break;
        case Notification:
            root = _xmlDoc.NewElement("NOTIFICATION");
            break;
        case Acknowledge:
            root = _xmlDoc.NewElement("ACKNOWLEDGE");
            break;
        default:
            throw invalid_argument("Invalid MessageType");
        }
        _xmlDoc.InsertFirstChild(root);

        switch (commandType)
        {
        case Get:
            _command = _xmlDoc.NewElement("GET");
            break;
        case Set:
            _command = _xmlDoc.NewElement("SET");
            break;
        case Subscribe:
            _command = _xmlDoc.NewElement("SUBSCRIBE");
            break;
        case Unsubscribe:
            _command = _xmlDoc.NewElement("UNSUBSCRIBE");
            break;
        default:
            throw invalid_argument("Invalid CommandType");
        }
        root->InsertEndChild(_command);
    }

    void UartMessageSender::appendRequestAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _command->DeleteChildren();

        if (type == SensorAll)
            _command->SetText("SENSOR_ALL");
        if (type == ControlAll)
            _command->SetText("CONTROL_ALL");
    }

    void UartMessageSender::appendRequest(eDataType dataType, const string &name)
    {
        XMLElement *child = NULL;
        switch (dataType)
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
        case Control1:
            child = _xmlDoc.NewElement("CONTROL1");
            break;
        case Control2:
            child = _xmlDoc.NewElement("CONTROL2");
            break;
        case DateTime:
            child = _xmlDoc.NewElement("DATETIME");
            break;
        case SensorAll:
        case ControlAll:
            appendRequestAll(dataType);
            return;
        default:
            return;
        }

        child->SetAttribute("NAME", name.c_str());
        _command->InsertEndChild(child);
    }

    void UartMessageSender::appendSubscribe(eDataType type, const string &name, uint32_t period)
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
        case Control1:
            child = _xmlDoc.NewElement("CONTROL1");
            break;
        case Control2:
            child = _xmlDoc.NewElement("CONTROL2");
            break;
        case DateTime:
            child = _xmlDoc.NewElement("DATETIME");
            break;
        case SensorAll:
        case ControlAll:
            appendSubscribeAll(type, period);
            return;
        default:
            return;
        }

        child->SetAttribute("NAME", name.c_str());
        child->SetAttribute("PERIOD", period);
        _command->InsertEndChild(child);
    }

    void UartMessageSender::appendSubscribeAll(eDataType type, uint32_t period)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _command->DeleteChildren();

        XMLElement *child = NULL;
        if (type == SensorAll)
            child = _xmlDoc.NewElement("SENSOR_ALL");
        else if (type == ControlAll)
            child = _xmlDoc.NewElement("CONTROL_ALL");

        child->SetAttribute("PERIOD", period);

        _command->InsertEndChild(child);
    }

    void UartMessageSender::appendUnsubscribe(eDataType type, const string &name)
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
        case Control1:
            child = _xmlDoc.NewElement("CONTROL1");
            break;
        case Control2:
            child = _xmlDoc.NewElement("CONTROL2");
            break;
        case DateTime:
            child = _xmlDoc.NewElement("DATETIME");
            break;
        case SensorAll:
        case ControlAll:
            appendUnsubscribeAll(type);
            return;
        default:
            return;
        }

        child->SetAttribute("NAME", name.c_str());
        _command->InsertEndChild(child);
    }

    void UartMessageSender::appendUnsubscribeAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _command->DeleteChildren();

        XMLElement *child = NULL;
        if (type == SensorAll)
            child = _xmlDoc.NewElement("SENSOR_ALL");
        else if (type == ControlAll)
            child = _xmlDoc.NewElement("CONTROL_ALL");

        _command->InsertEndChild(child);
    }

    string UartMessageSender::sendMessage()
    {
        // Add SEQUENCE ID (ROOT에 attribute 있으면 안됨)
        // XMLElement *root = _xmlDoc.RootElement();
        // root->SetAttribute("SEQUENCE ID", _seqId);

        XMLPrinter xmlPrint;
        _xmlDoc.Print(&xmlPrint);
        string buf = xmlPrint.CStr();
        appendCheckSum(buf);

        // Serial.println(buf);
        cout << buf << endl;

        return buf;
    }

}; // namespace UartMessageInterface