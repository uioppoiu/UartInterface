#include <iostream>
#include "UartMessageCallbackManagement.h"

using namespace std;

namespace UartMessageInterface
{
    UartMessageCallbackManagement *UartMessageCallbackManagement::_Instance = NULL;

    UartMessageCallbackManagement &UartMessageCallbackManagement::getInstance()
    {
        if (_Instance == NULL)
        {
            _Instance = new UartMessageCallbackManagement;
        }
        return *_Instance;
    }

    void UartMessageCallbackManagement::registerRequestGetCallBack(eDataType dataType, const string &name, const CallBackRequestGet &func)
    {
        getInstance()._callBackListRequestGet.insert(make_pair(make_tuple(dataType, name), func));
    }

    void UartMessageCallbackManagement::invokeRequestGetCallBack(eDataType dataType, const string &name)
    {
        map<tuple<eDataType, string>, CallBackRequestGet>::iterator iter =
            getInstance()._callBackListRequestGet.find(make_tuple(dataType, name));

        if (iter == getInstance()._callBackListRequestGet.end())
            return;

        iter->second(dataType, name);
    }

    void UartMessageCallbackManagement::registerResponseGetCallBack(eDataType dataType, const string &name, const CallBackResponseGet &func)
    {
        getInstance()._callBackListResponseGet.insert(make_pair(make_tuple(dataType, name), func));
    }

    void UartMessageCallbackManagement::invokeResponseGetCallBack(eDataType dataType, const string &name, const Value &value)
    {
        map<tuple<eDataType, string>, CallBackResponseGet>::iterator iter =
            getInstance()._callBackListResponseGet.find(make_tuple(dataType, name));

        if (iter == getInstance()._callBackListResponseGet.end())
            return;

        iter->second(dataType, name, value);
    }

    void UartMessageCallbackManagement::registerSubscribeCallBack(CallBackSubscribe &func)
    {
        getInstance()._callBackSubscribe = func;
    }

    void UartMessageCallbackManagement::invokeSubscribeCallBack(eDataType dataType, const string &name, uint32_t period)
    {
        getInstance()._callBackSubscribe(dataType, name, period);
    }

    void UartMessageCallbackManagement::registerUnsubscribeCallBack(CallBackUnsubscribe &func)
    {
        getInstance()._callBackUnsubscribe = func;
    }

    void UartMessageCallbackManagement::invokeUnsubscribeCallBack(eDataType dataType, const string &name)
    {
        getInstance()._callBackUnsubscribe(dataType, name);
    }

}; // namespace UartMessageInterface