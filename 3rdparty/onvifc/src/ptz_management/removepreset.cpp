#include "removepreset.h"
#include "message.h"

using namespace ONVIF;

RemovePreset::RemovePreset(QObject *parent):QObject(parent)
{

}

RemovePreset::~RemovePreset()
{

}

QDomElement RemovePreset::toxml()
{
    QDomElement removePreset = newElement("wsdl:RemovePreset");
    QDomElement profileToken = newElement("wsdl:ProfileToken",this->profileToken());
    QDomElement presetToken = newElement("wsdl:PresetToken",this->presetToken());
    removePreset.appendChild(profileToken);
    removePreset.appendChild(presetToken);
    return removePreset;
}
