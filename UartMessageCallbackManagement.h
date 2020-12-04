#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include <Arduino.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    class UartMessageCallbackManagement
    {
    public:
        typedef void(*CallBackRequestGet)(uint32_t /*seqId*/, const RequestGetData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackResponseGet)(uint32_t /*seqId*/, const ResponseGetData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackNotification)(uint32_t /*seqId*/, const NotificationData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackSubscribe)(uint32_t /*seqId*/, const SubscribeData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackUnsubscribe)(uint32_t /*seqId*/, const UnsubscribeData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackRequestSet)(uint32_t /*seqId*/, const RequestSetData* /*dataArr*/, size_t /*arrSize*/);
        typedef void(*CallBackAcknowledge)(uint32_t /*seqId*/, unsigned char /*msgId*/);
        
        static void registerRequestGetCallBack(const CallBackRequestGet func);
        static void invokeRequestGetCallBack(uint32_t seqId, const RequestGetData* dataArr, size_t arrSize);
        static void registerResponseGetCallBack(const CallBackResponseGet func);
        static void invokeResponseGetCallBack(uint32_t seqId, const ResponseGetData* dataArr, size_t arrSize);
        static void registerNotificationCallBack(const CallBackNotification func);
        static void invokeNotificationCallBack(uint32_t seqId, const NotificationData* dataArr, size_t arrSize);
        static void registerSubscribeCallBack(const CallBackSubscribe func);
        static void invokeSubscribeCallBack(uint32_t seqId, const SubscribeData* dataArr, size_t arrSize);
        static void registerUnsubscribeCallBack(const CallBackUnsubscribe func);
        static void invokeUnsubscribeCallBack(uint32_t seqId, const UnsubscribeData* dataArr, size_t arrSize);
        static void registerRequestSetCallBack(const CallBackRequestSet func);
        static void invokeRequestSetCallBack(uint32_t seqId, const RequestSetData* dataArr, size_t arrSize);
        static void registerAcknowledgeCallBack(const CallBackAcknowledge func);
        static void invokeAcknowledgeCallBack(uint32_t seqId, unsigned char msgId);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();
        UartMessageCallbackManagement();

        CallBackRequestGet _callBackRequestGet;
        CallBackResponseGet _callBackResponseGet;
        CallBackNotification _callBackNotification;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
        CallBackRequestSet _callBackRequestSet;
        CallBackAcknowledge _callBackAcknowledge;
    };


}; // namespace UartMessageInterface

#endif
