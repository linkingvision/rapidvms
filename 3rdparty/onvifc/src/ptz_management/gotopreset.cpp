#include "gotopreset.h"
#include "message.h"

using namespace ONVIF;

GotoPreset::GotoPreset(QObject *parent):QObject(parent)
{

}

GotoPreset::~GotoPreset()
{

}

QDomElement GotoPreset::toxml()
{
    QDomElement gotoPreset = newElement("wsdl:GotoPreset");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement presetToken = newElement("wsdl:PresetToken",this->presetToken());
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
    gotoPreset.appendChild(profileToken);
    gotoPreset.appendChild(presetToken);
    gotoPreset.appendChild(speed);
    return gotoPreset;
}
