#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include <string>
#include <ctime>
#include <functional>
#include <map>
#include "tinyxml2.h"
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

using namespace std;
using namespace tinyxml2;

namespace UartMessageInterface
{
    class UartMessageSender
    {
    public:
        UartMessageSender(eMessageType messageType, eCommandType commandType);

        // Request
        void appendRequest(eDataType type, const string &name); // All 이면 return;
        void appendRequestAll(eDataType type);                  // 나머지 비우고 All로

        // Response
        template<typename VALUE_TYPE>
        void appendResponse(eDataType type, const string &name, const VALUE_TYPE &value, eValueType valueType)
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
            case Control0:
                child = _xmlDoc.NewElement("CONTROL0");
                break;
            default:
                return;
            }

            child->SetAttribute("NAME", name.c_str());
            if (valueType == Double)
            {
                child->SetAttribute("TYPE", "DOUBLE");
                child->SetAttribute("VALUE", value);
            }
            else // Integer
            {
                child->SetAttribute("TYPE", "INTEGER");
                child->SetAttribute("VALUE", value);
            }

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

        // Subscribe
        void appendSubscribe(eDataType type, const string &name, uint32_t period); // All 이면 return;
        void appendSubscribeAll(eDataType type, uint32_t period);                  // 나머지 비우고 All로

        // Unsubscribe
        void appendUnsubscribe(eDataType type, const string &name); // All 이면 return;
        void appendUnsubscribeAll(eDataType type);                  // 나머지 비우고 All로

        string sendMessage();

    private:
        uint32_t _seqId;
        XMLDocument _xmlDoc;
        XMLElement *_command;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
