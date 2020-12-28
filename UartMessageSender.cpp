#include "Arduino.h"
#include "UartMessageSender.h"
#include "UartEndian.h"

namespace UartMessageInterface
{

    UartMessageSender::UartMessageSender(const unsigned char msgId)
        : _header((MsgCommonHeader *)_messageBuffer)
    {
        memset(_messageBuffer, 0x00, sizeof(_messageBuffer));
        _header->msgId = msgId;
        _header->msgSize = sizeof(MsgCommonHeader);
    }

    UartMessageSender::~UartMessageSender()
    {
    }

    void UartMessageSender::setSeqId(uint32_t seqId)
    {
        _header->seqId = htonl(seqId);
    }

    void UartMessageSender::sendMessage()
    {
        uint8_t checkSum = getCheckSum(_messageBuffer, _header->msgSize);
        _messageBuffer[_header->msgSize] = checkSum;
        _header->msgSize += 1;

        Serial.write("<BEGIN>");
        Serial.write(_messageBuffer, _header->msgSize);
        Serial.write("<END>");
    }

    void UartMessageSender::appendRequestGetDataCommon(unsigned char dataType)
    {
        RequestGetData data;
        memset(&data, 0x00, sizeof(RequestGetData));

        data.type = dataType;

        appendData(data);
    }

    void UartMessageSender::appendRequestGetData(unsigned char dataType)
    {
        if (_header->msgId != UartMessageInterface::MsgId::RequestGet)
            return;

        appendRequestGetDataCommon(dataType);
    }

    void UartMessageSender::appendSubscribeData(unsigned char dataType)
    {
        if (_header->msgId != UartMessageInterface::MsgId::Subscribe)
            return;

        appendRequestGetDataCommon(dataType);
    }

    void UartMessageSender::appendUnsubscribeData(unsigned char dataType)
    {
        if (_header->msgId != UartMessageInterface::MsgId::Unsubscribe)
            return;

        appendRequestGetDataCommon(dataType);
    }

    void UartMessageSender::appendResponseGetDataCommon(unsigned char dataType, uint32_t value)
    {
        ResponseGetData data;
        memset(&data, 0x00, sizeof(ResponseGetData));

        data.type = dataType;
        data.value = htonl(value);

        appendData(data);
    }

    void UartMessageSender::appendResponseGetData(unsigned char dataType, uint32_t value)
    {
        if (_header->msgId != UartMessageInterface::MsgId::ResponseGet)
            return;

        appendResponseGetDataCommon(dataType, value);
    }

    void UartMessageSender::appendNotificationData(unsigned char dataType, uint32_t value)
    {
        if (_header->msgId != UartMessageInterface::MsgId::Notification)
            return;

        appendResponseGetDataCommon(dataType, value);
    }

    void UartMessageSender::appendRequestSetData(unsigned char dataType, uint32_t value)
    {
        if (_header->msgId != UartMessageInterface::MsgId::RequestSet)
            return;

        appendResponseGetDataCommon(dataType, value);
    }

}; // namespace UartMessageInterface