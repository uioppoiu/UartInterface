#include "UartMessageReceiver.h"
#include "UartEndian.h"

namespace UartMessageInterface
{
    UartMessageReceiver::UartMessageReceiver(const uint8_t* msg, size_t msgSize)
    {
        memcpy(_message, msg, msgSize);
        // if (verityCheckSum(message) == false) throw;
    }

    UartMessageReceiver::~UartMessageReceiver()
    {
    }

    void UartMessageReceiver::processMessage()
    {
        const MsgCommonHeader *msgHeader = (const MsgCommonHeader *)_message;
        const uint8_t msgId = msgHeader->msgId; // 메시지 ID
        const uint32_t seqId = ntohl(msgHeader->seqId);
        const uint8_t numOfData = msgHeader->numOfData; // 데이터의 수

        Serial.print("MsgId:0x");
        Serial.print(msgId,16);
        Serial.print(" SeqId:");
        Serial.println(seqId);

        switch (msgId)
        {
        case MsgId::RequestGet:
        {
            const RequestGetData *dataArr = (const RequestGetData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeRequestGetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::RequestSet:
        {
            const RequestSetData *dataArr = (const RequestSetData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeRequestSetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Subscribe:
        {
            const SubscribeData *dataArr = (const SubscribeData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeSubscribeCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Unsubscribe:
        {
            const UnsubscribeData *dataArr = (const UnsubscribeData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeUnsubscribeCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Notification:
        {
            const NotificationData *dataArr = (const NotificationData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeNotificationCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::ResponseGet:
        {
            const ResponseGetData *dataArr = (const ResponseGetData *)(_message + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeResponseGetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Acknowledge:
        {
            UartMessageCallbackManagement::invokeAcknowledgeCallBack(seqId, msgId);
        }
        break;
        default:
            return;
        }
    }
}; // namespace UartMessageInterface