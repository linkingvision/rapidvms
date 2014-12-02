#include "preset.h"
#include "message.h"
using namespace ONVIF;

Preset::Preset(QObject *parent):QObject(parent)
{

}

Preset::~Preset()
{

}

QDomElement Preset::toxml()
{
    QDomElement setPreset = newElement("wsdl:SetPreset");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement presetName = newElement("wsdl:PresetName",this->presetName());
    QDomElement presetToken = newElement("wsdl:PresetToken",this->presetToken());
    setPreset.appendChild(profileToken);
    setPreset.appendChild(presetName);
    setPreset.appendChild(presetToken);
    return setPreset;
}
