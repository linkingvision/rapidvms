#include "stop.h"
#include "message.h"
using namespace ONVIF;
Stop::Stop(QObject *parent):QObject(parent)
{

}

Stop::~Stop()
{

}

QDomElement Stop::toxml()
{
    QDomElement stop = newElement("wsdl:Stop");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement panTilt = newElement("wsdl:PanTilt",this->panTilt() == true?"true":"false");
    QDomElement zoom = newElement("wsdl:Zoom",this->zoom() == true?"true":"false");
    stop.appendChild(profileToken);
    stop.appendChild(panTilt);
    stop.appendChild(zoom);
    return stop;
}

