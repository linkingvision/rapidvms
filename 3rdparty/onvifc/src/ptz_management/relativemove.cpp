#include "relativemove.h"
#include "message.h"
using namespace ONVIF;

RelativeMove::RelativeMove(QObject *parent):QObject(parent)
{

}

RelativeMove::~RelativeMove()
{

}

QDomElement RelativeMove::toxml()
{
    QDomElement RelativeMove = newElement("wsdl:RelativeMove");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement translation = newElement("wsdl:Translation");
    QDomElement translationPanTilt = newElement("sch:PanTilt");
    translationPanTilt.setAttribute("x",this->translationPanTiltX());
    translationPanTilt.setAttribute("y",this->translationPanTiltY());
    translationPanTilt.setAttribute("space",this->translationPanTiltSpace());
    QDomElement translationZoom = newElement("sch:Zoom");
    translationZoom.setAttribute("x",this->translationZoomX());
    translationZoom.setAttribute("space",this->translationZoomSpace());
    QDomElement speed = newElement("wsdl:Speed");
    QDomElement speedPanTilt = newElement("sch:PanTilt");
    speedPanTilt.setAttribute("x",this->speedPanTiltX());
    speedPanTilt.setAttribute("y",this->speedPanTiltY());
    speedPanTilt.setAttribute("space",this->speedPanTiltSpace());
    QDomElement speedZoom = newElement("sch:Zoom");
    speedZoom.setAttribute("x",this->speedZoomX());
    speedZoom.setAttribute("space",this->speedZoomSpace());
    RelativeMove.appendChild(profileToken);
    RelativeMove.appendChild(translation);
    RelativeMove.appendChild(speed);
    translation.appendChild(translationPanTilt);
    translation.appendChild(translationZoom);
    speed.appendChild(speedPanTilt);
    speed.appendChild(speedZoom);
    return RelativeMove;
}
