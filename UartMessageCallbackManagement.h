#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include <string>
#include <ctime>
#include <functional>
#include <map>
#include "tinyxml2.h"
#include "UartMessageInterface.h"

using namespace std;
using namespace tinyxml2;

namespace UartMessageInterface
{

    class UartMessageCallbackManagement
    {
    public:
        typedef function<void(eDataType, const string &)> CallBackRequestGet;
        typedef function<void(eDataType, const string &, const Value &)> CallBackResponseGet;
        typedef function<void(eDataType, const string &, uint32_t)> CallBackSubscribe;
        typedef function<void(eDataType, const string &)> CallBackUnsubscribe;

        static void registerRequestGetCallBack(eDataType type, const string &name, const CallBackRequestGet &func);
        static void invokeRequestGetCallBack(eDataType type, const string &name);
        static void registerResponseGetCallBack(eDataType type, const string &name, const CallBackResponseGet &func);
        static void invokeResponseGetCallBack(eDataType type, const string &name, const Value &value);
        static void registerSubscribeCallBack(CallBackSubscribe &func);
        static void invokeSubscribeCallBack(eDataType type, const string &name, uint32_t period);
        static void registerUnsubscribeCallBack(CallBackUnsubscribe &func);
        static void invokeUnsubscribeCallBack(eDataType type, const string &name);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();

        typedef map<tuple<eDataType, string>, CallBackRequestGet> CallBackListRequestGet;
        CallBackListRequestGet _callBackListRequestGet;
        typedef map<tuple<eDataType, string>, CallBackResponseGet> CallBackListResponseGet;
        CallBackListResponseGet _callBackListResponseGet;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
    };


}; // namespace UartMessageInterface

#endif
