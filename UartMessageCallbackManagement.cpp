#include "UartMessageCallbackManagement.h"

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

    UartMessageCallbackManagement::UartMessageCallbackManagement()
        : _callBackRequestGet(NULL),
          _callBackResponseGet(NULL),
          _callBackRequestSet(NULL),
          _callBackSubscribe(NULL),
          _callBackUnsubscribe(NULL),
          _callBackNotification(NULL),
          _callBackAcknowledge(NULL)
    {
    }

    void UartMessageCallbackManagement::registerRequestGetCallBack(const CallBackRequestGet func)
    {
        getInstance()._callBackRequestGet = func;
    }

    void UartMessageCallbackManagement::invokeRequestGetCallBack(uint32_t seqId, const RequestGetData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackRequestGet == NULL) return;
        getInstance()._callBackRequestGet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerResponseGetCallBack(const CallBackResponseGet func)
    {
        getInstance()._callBackResponseGet = func;
    }

    void UartMessageCallbackManagement::invokeResponseGetCallBack(uint32_t seqId, const ResponseGetData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackResponseGet == NULL) return;

        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        getInstance()._callBackResponseGet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerNotificationCallBack(const CallBackNotification func)
    {
        getInstance()._callBackNotification = func;
    }

    void UartMessageCallbackManagement::invokeNotificationCallBack(uint32_t seqId, const NotificationData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackNotification == NULL) return;
        
        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        getInstance()._callBackNotification(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerSubscribeCallBack(const CallBackSubscribe func)
    {
        getInstance()._callBackSubscribe = func;
    }

    void UartMessageCallbackManagement::invokeSubscribeCallBack(uint32_t seqId, const SubscribeData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackSubscribe == NULL) return;
        getInstance()._callBackSubscribe(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerUnsubscribeCallBack(const CallBackUnsubscribe func)
    {
        getInstance()._callBackUnsubscribe = func;
    }

    void UartMessageCallbackManagement::invokeUnsubscribeCallBack(uint32_t seqId, const UnsubscribeData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackUnsubscribe == NULL) return;
        getInstance()._callBackUnsubscribe(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerRequestSetCallBack(const CallBackRequestSet func)
    {
        getInstance()._callBackRequestSet = func;
    }

    void UartMessageCallbackManagement::invokeRequestSetCallBack(uint32_t seqId, const RequestSetData *dataArr, size_t arrSize)
    {
        if(getInstance()._callBackRequestSet == NULL) return;
        
        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        getInstance()._callBackRequestSet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerAcknowledgeCallBack(const CallBackAcknowledge func)
    {
        getInstance()._callBackAcknowledge = func;
    }

    void UartMessageCallbackManagement::invokeAcknowledgeCallBack(uint32_t seqId, unsigned char msgId)
    {
        if(getInstance()._callBackAcknowledge == NULL) return;
        getInstance()._callBackAcknowledge(seqId, msgId);
    }

}; // namespace UartMessageInterface