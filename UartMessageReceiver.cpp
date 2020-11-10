#include <iostream>
#include "UartMessageReceiver.h"

using namespace std;

namespace UartMessageInterface
{
    UartMessageReceiver::UartMessageReceiver(const string &message)
    {
        if (isUartMessage(message) == false)
        {
            throw invalid_argument("This message is not Uart message");
        }

        if (verityCheckSum(message) == false)
        {
            throw invalid_argument("CheckSum error");
        }

        int32_t retVal = _xmlDoc.Parse(message.c_str(), message.size() - 1);
        if(retVal != XML_SUCCESS)
        {
            throw invalid_argument("Parsing error");
        }

        // XMLPrinter xmlPrint;
        // _xmlDoc.Print(&xmlPrint);
        // string buf = xmlPrint.CStr();
        // cout << buf << endl;
    }

    bool UartMessageReceiver::isMessageValid()
    {
        return true; // TODO 내용 검사
    }

    bool UartMessageReceiver::isUartMessage(const string &message)
    {
        return true; // TODO
    }

    void UartMessageReceiver::processMessage()
    {
        const XMLElement *root = _xmlDoc.RootElement();
        if (string(root->Name()) == string("REQUEST"))
        {
            handleRequestMessage(root);
        }
        else if ((string(root->Name()) == string("RESPONSE")) ||
                 (string(root->Name()) == string("NOTIFICATION")))
        {
            handleResponseMessage(root);
        }
    }

    void UartMessageReceiver::handleRequestMessage(const XMLElement *root)
    {
        cout << "REQUEST Message arrived" << endl;

        const XMLElement *command = root->FirstChildElement();
        if (string(command->Value()) == string("GET"))
        {
            XMLElement *elem = const_cast<XMLElement *>(command->FirstChildElement());
            while (elem != NULL)
            {
                invokeRequestGet(elem);
                elem = const_cast<XMLElement *>(elem->NextSiblingElement());
            }
        }
        if (string(command->Value()) == string("SUBSCRIBE"))
        {
            XMLElement *elem = const_cast<XMLElement *>(command->FirstChildElement());
            while (elem != NULL)
            {
                invokeSubscribe(elem);
                elem = const_cast<XMLElement *>(elem->NextSiblingElement());
            }
        }
        if (string(command->Value()) == string("UNSUBSCRIBE"))
        {
            XMLElement *elem = const_cast<XMLElement *>(command->FirstChildElement());
            while (elem != NULL)
            {
                invokeRequestGet(elem);
                elem = const_cast<XMLElement *>(elem->NextSiblingElement());
            }
        }
    }

    void UartMessageReceiver::invokeRequestGet(const XMLElement *element)
    {
        if (element == NULL)
            return;

        eDataType type = str2EnumDataType(element->Name());
        string name = element->Attribute("NAME");
        UartMessageCallbackManagement::invokeRequestGetCallBack(type, name);
    }

    void UartMessageReceiver::invokeSubscribe(const XMLElement *element)
    {
        if (element == NULL)
            return;

        eDataType dataType = str2EnumDataType(element->Name());
        string name = element->Attribute("NAME");
        uint32_t period = element->UnsignedAttribute("PERIOD");

        UartMessageCallbackManagement::invokeSubscribeCallBack(dataType, name, period);
    }

    void UartMessageReceiver::invokeUnsubscribe(const XMLElement *element)
    {
        if (element == NULL)
            return;

        eDataType dataType = str2EnumDataType(element->Name());
        string name = element->Attribute("NAME");

        UartMessageCallbackManagement::invokeUnsubscribeCallBack(dataType, name);
    }

    void UartMessageReceiver::handleResponseMessage(const XMLElement *root)
    {
        cout << "RESPONSE/NOTIFICATION Message arrived" << endl;

        const XMLElement *command = root->FirstChildElement();
        if (string(command->Value()) == string("GET"))
        {
            XMLElement *elem = const_cast<XMLElement *>(command->FirstChildElement());
            while (elem != NULL)
            {
                invokeResponseGet(elem);
                elem = const_cast<XMLElement *>(elem->NextSiblingElement());
            }
        }
    }

    void UartMessageReceiver::invokeResponseGet(const XMLElement *element)
    {
        if (element == NULL)
            return;

        eDataType dataType = str2EnumDataType(element->Name());
        string name = element->Attribute("NAME");
        string valueType = element->Attribute("TYPE");
        if (valueType == "DOUBLE")
        {
            Value v;
            v.type = Double;
            v.value.val_double = element->DoubleAttribute("VALUE");
            UartMessageCallbackManagement::invokeResponseGetCallBack(dataType, name, v);
        }
        else // if(valueType == "INTEGER")
        {
            Value v;
            v.type = Integer;
            v.value.val_int = element->UnsignedAttribute("VALUE");
            UartMessageCallbackManagement::invokeResponseGetCallBack(dataType, name, v);
        }
    }

}; // namespace UartMessageInterface