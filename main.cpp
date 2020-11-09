#include <iostream>
#include "UartMessageSender.h"
#include "UartMessageReceiver.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace UartMessageInterface;

void onRequestTemp1(eDataType type, const string &name)
{
    cout << __FUNCTION__ << endl;
    cout << type << endl;
    cout << name << endl;
}

void onRequestTemp2(eDataType type, const string &name)
{
    cout << __FUNCTION__ << endl;
    cout << type << endl;
    cout << name << endl;
}

void onResponseTemp1(eDataType type, const string &name, const Value &value)
{
    cout << __FUNCTION__ << endl;
    cout << type << endl;
    cout << name << endl;
    cout << value.type << endl;
    cout << value.value.val_double << endl;
}

void onResponseTemp2(eDataType type, const string &name, const Value &value)
{
    cout << __FUNCTION__ << endl;
    cout << type << endl;
    cout << name << endl;
    cout << value.type << endl;
    cout << value.value.val_int << endl;
}

int main(int, char **)
{
    UartMessageSender reqTemp(Request, Get);
    reqTemp.appendRequest(SensorTemperature, "ROOM");
    reqTemp.appendRequest(SensorTemperature, "WATER");
    string msgReq = reqTemp.sendMessage();

    UartMessageCallbackManagement::registerRequestGetCallBack(SensorTemperature, "ROOM", onRequestTemp1);
    UartMessageCallbackManagement::registerRequestGetCallBack(SensorTemperature, "WATER", onRequestTemp1);

    UartMessageReceiver rcvReq(msgReq);
    rcvReq.processMessage();

    UartMessageSender rspTemp(Response, Get);
    rspTemp.appendResponse(SensorTemperature, "ROOM", 25.5, Double);
    rspTemp.appendResponse(SensorTemperature, "WATER", 19.0, Integer);
    string msgRsp = rspTemp.sendMessage();

    UartMessageCallbackManagement::registerResponseGetCallBack(SensorTemperature, "ROOM", onResponseTemp2);
    UartMessageCallbackManagement::registerResponseGetCallBack(SensorTemperature, "WATER", onResponseTemp2);

    UartMessageReceiver rcvRsp(msgReq);
    rcvRsp.processMessage();

    return 0;
}
