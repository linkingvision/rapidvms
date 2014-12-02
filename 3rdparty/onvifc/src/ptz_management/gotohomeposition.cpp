#include "gotohomeposition.h"
#include "message.h"

using namespace ONVIF;

GotoHomePosition::GotoHomePosition(QObject *parent):QObject(parent)
{

}

GotoHomePosition::~GotoHomePosition()
{

}

QDomElement GotoHomePosition::toxml()
{
    QDomElement gotoHomePosition = newElement("wsdl:GotoHomePosition");
    QDomElement profileToken  = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement speed = newElement("wsdl:Speed");
    QDomElement panTilt = newElement("sch:PanTilt");
    QDomElement zoom = newElement("sch:Zoom");
    panTilt.setAttribute("x",this->panTiltX());
    panTilt.setAttribute("y",this->panTiltY());
    panTilt.setAttribute("space",this->panTiltSpace());
    zoom.setAttribute("x",this->zoomX());
    zoom.setAttribute("space",this->zoomSpace());
    speed.appendChild(panTilt);
    speed.appendChild(zoom);
    gotoHomePosition.appendChild(profileToken);
    gotoHomePosition.appendChild(speed);
    return gotoHomePosition;
}

