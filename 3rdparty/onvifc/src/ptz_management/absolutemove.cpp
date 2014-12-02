#include "absolutemove.h"
#include "message.h"
using namespace ONVIF;

AbsoluteMove::AbsoluteMove(QObject *parent):QObject(parent)
{

}

AbsoluteMove::~AbsoluteMove()
{

}

QDomElement AbsoluteMove::toxml()
{
    QDomElement absoluteMove = newElement("wsdl:AbsoluteMove");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement position = newElement("wsdl:Position");
    QDomElement positionPanTilt = newElement("sch:PanTilt");
    positionPanTilt.setAttribute("x",this->positionPanTiltX());
    positionPanTilt.setAttribute("y",this->positionPanTiltY());
    positionPanTilt.setAttribute("space",this->positionPanTiltSpace());
    QDomElement positionZoom = newElement("sch:Zoom");
    positionZoom.setAttribute("x",this->positionZoomX());
    positionZoom.setAttribute("space",this->positionZoomSpace());
    QDomElement speed = newElement("wsdl:Speed");
    QDomElement speedPanTilt = newElement("sch:PanTilt");
    speedPanTilt.setAttribute("x",this->speedPanTiltX());
    speedPanTilt.setAttribute("y",this->speedPanTiltY());
    speedPanTilt.setAttribute("space",this->speedPanTiltSpace());
    QDomElement speedZoom = newElement("sch:Zoom");
    speedZoom.setAttribute("x",this->speedZoomX());
    speedZoom.setAttribute("space",this->speedZoomSpace());
    absoluteMove.appendChild(profileToken);
    absoluteMove.appendChild(position);
    absoluteMove.appendChild(speed);
    position.appendChild(positionPanTilt);
    position.appendChild(positionZoom);
    speed.appendChild(speedPanTilt);
    speed.appendChild(speedZoom);
    return absoluteMove;
}
