#include "continuousmove.h"
#include "message.h"
using namespace ONVIF;

ContinuousMove::ContinuousMove(QObject *parent):QObject(parent)
{

}

ContinuousMove::~ContinuousMove()
{

}

QDomElement ContinuousMove::toxml()
{
    QDomElement continuousMove = newElement("wsdl:ContinuousMove");
    QDomElement velocity = newElement("wsdl:Velocity");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement panTilt = newElement("sch:PanTilt");
    panTilt.setAttribute("x",this->panTiltX());
    panTilt.setAttribute("y",this->panTiltY());
    panTilt.setAttribute("space",this->panTiltSpace());
    QDomElement zoom = newElement("sch:Zoom");
    zoom.setAttribute("x",this->zoomX());
    zoom.setAttribute("space",this->zoomSpace());
    QDomElement timeout = newElement("wsdl:Timeout",this->timeout());
    continuousMove.appendChild(profileToken);
    continuousMove.appendChild(velocity);
    continuousMove.appendChild(timeout);
    velocity.appendChild(panTilt);
    velocity.appendChild(zoom);
    return continuousMove;
}


