#include "capabilities.h"

using namespace ONVIF;

Capabilities::Capabilities(QObject *parent) : QObject(parent) {
}
Capabilities::~Capabilities(){
}
QDomElement Capabilities::toxml()
{
	QDomElement fake;
	return fake;
}
QString Capabilities::enumToString(Category category)
{
    switch (category) {
    case Capabilities::All:
        return "All";
        break;
    case Capabilities::Analytics:
        return "Analytics";
        break;
    case Capabilities::Device:
        return "Device";
        break;
    case Capabilities::Events:
        return "Events";
        break;
    case Capabilities::Imaging:
        return "Imaging";
        break;
    case Capabilities::Media:
        return "Media";
        break;
    case Capabilities::PTZ:
        return "PTZ";
        break;
    default:
        break;
    }
}

