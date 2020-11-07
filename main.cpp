#include <iostream>
#include "MessageInterface.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace UartMessageInterface;

int main(int, char **)
{
    // XMLDocument xmlDocWrite;
    // XMLNode *root = xmlDocWrite.NewElement("MESSAGE");
    // xmlDocWrite.InsertFirstChild(root);

    // XMLElement *timestamp = xmlDocWrite.NewElement("TIMESTAMP");
    // timestamp->SetText(UartMessageInterface::getCurrentTime().c_str());
    // root->InsertEndChild(timestamp);

    // XMLElement *status = xmlDocWrite.NewElement("STATUS");
    // root->InsertEndChild(status);

    // XMLElement *temp_room = status->InsertNewChildElement("TEMPERATURE");
    // temp_room->SetAttribute("NAME", "ROOM");
    // temp_room->SetAttribute("VALUE", 25.5);

    // XMLElement *temp_water = status->InsertNewChildElement("TEMPERATURE");
    // temp_water->SetAttribute("NAME", "WATER");
    // temp_water->SetAttribute("VALUE", 19.9);

    // XMLElement *humidity = status->InsertNewChildElement("HUMIDITY");
    // humidity->SetAttribute("NAME", "SENSOR1");
    // humidity->SetAttribute("VALUE", 35);

    // XMLElement *co2 = status->InsertNewChildElement("CO2");
    // co2->SetAttribute("NAME", "SENSOR2");
    // co2->SetAttribute("VALUE", 40);

    // XMLElement *conductivity = status->InsertNewChildElement("CONDUCTIVITY");
    // conductivity->SetAttribute("NAME", "SENSOR3");
    // conductivity->SetAttribute("VALUE", 45);

    // XMLElement *control = xmlDocWrite.NewElement("CONTROL");
    // root->InsertEndChild(control);

    // XMLElement *valve1 = control->InsertNewChildElement("VALVE");
    // valve1->SetAttribute("NAME", "POINT1");
    // valve1->SetAttribute("VALUE", true);

    // XMLElement *valve2 = control->InsertNewChildElement("VALVE");
    // valve2->SetAttribute("NAME", "POINT2");
    // valve2->SetAttribute("VALUE", false);

    // xmlDocWrite.SaveFile("test.xml");

    // XMLPrinter xmlPrint;
    // xmlDocWrite.Print(&xmlPrint);

    // string buf = xmlPrint.CStr();

    // for (int i = 0; i < 10; i++)
    // {
    //     buf.push_back('A' + i);
    //     appendCheckSum(buf);
    //     bool result = verityCheckSum(buf);
    // }

    UartSensorMessage reqTemp(123, Request, GetSensor);
    reqTemp.appendRequest(SensorTemperature, "ROOM");
    reqTemp.appendRequest(SensorTemperature, "WATER");
    string msgReq = reqTemp.sendMessage();

    processUartMessage(msgReq);


    UartSensorMessage rspTemp(123, Response, GetSensor);
    rspTemp.appendResponse(SensorTemperature, "ROOM", 25.5);
    rspTemp.appendResponse(SensorTemperature, "WATER", 19.0);
    string msgRsp = rspTemp.sendMessage();

    processUartMessage(msgRsp);

    return 0;
}
