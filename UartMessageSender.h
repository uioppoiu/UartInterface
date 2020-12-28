#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include <Arduino.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageSender
    {
    public:
        UartMessageSender(const unsigned char msgId);
        ~UartMessageSender();

        void setSeqId(uint32_t seqId);
        
        template<typename T>
        void appendData(const T& data)
        {
            if (_header->sizeOfData == 0)
            {
                _header->sizeOfData = sizeof(T);
            }

            if (_header->sizeOfData != sizeof(T))
            {
                // Data type을 섞어서 append할 수 없다.
                return;
            }

            T* dataPtr = (T*)(_messageBuffer + sizeof(MsgCommonHeader));
            memcpy(dataPtr + _header->numOfData, &data, sizeof(T));

            _header->msgSize += sizeof(T);
            _header->numOfData += 1;
        }

        void appendRequestGetDataCommon(unsigned char dataType);
        void appendRequestGetData(unsigned char dataType);
        void appendSubscribeData(unsigned char dataType);
        void appendUnsubscribeData(unsigned char dataType);

        void appendResponseGetDataCommon(unsigned char dataType, uint32_t value);
        void appendResponseGetData(unsigned char dataType, uint32_t value);
        void appendNotificationData(unsigned char dataType, uint32_t value);
        void appendRequestSetData(unsigned char dataType, uint32_t value);

        void sendMessage();

    private:
        uint8_t _messageBuffer[128];
        MsgCommonHeader* _header;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
