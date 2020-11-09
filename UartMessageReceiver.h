#ifndef _UART_MESSAGE_RECEIVER_H_
#define _UART_MESSAGE_RECEIVER_H_

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
    class UartMessageReceiver
    {
    public:
        UartMessageReceiver(const string &message);
        bool isMessageValid();
        void processMessage();
        
    private:
        XMLDocument _xmlDoc;
        UartMessageReceiver();

        bool isUartMessage(const string &message);
        
        void handleRequestMessage(const XMLElement *root);
        void invokeRequestGet(const XMLElement *element);
        void invokeSubscribe(const XMLElement *element);
        void invokeUnsubscribe(const XMLElement *element);
        void handleResponseMessage(const XMLElement *root);
        void invokeResponseGet(const XMLElement *element);

        // TODO Set
    };

}; // namespace UartMessageInterface

#endif
