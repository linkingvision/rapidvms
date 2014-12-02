#include "systemfactorydefault.h"
#include "message.h"
using namespace ONVIF;

SystemFactoryDefault::SystemFactoryDefault(QObject *parent) : QObject(parent) {
}
SystemFactoryDefault::~SystemFactoryDefault(){
}

QDomElement SystemFactoryDefault::toxml(){
    QDomElement setSystemFactoryDefault,factoryDefault;
    setSystemFactoryDefault = newElement("wsdl:SetSystemFactoryDefault");
    factoryDefault = newElement("wsdl:FactoryDefault",this->factoryDefault() == SystemFactoryDefault::Hard?"Hard":"Soft");
    setSystemFactoryDefault.appendChild(factoryDefault);
    return setSystemFactoryDefault;
}


