#include "networkinterfaces.h"
#include "message.h"

using namespace ONVIF;

NetworkInterfaces::NetworkInterfaces(QObject *parent) : QObject(parent) {
}
NetworkInterfaces::~NetworkInterfaces(){
}

QDomElement NetworkInterfaces::toxml(){
    QDomElement setNetworkInterfaces,interfaceToken,networkInterface,networkInterfaceEnabled,link,autoNegotiation,speed,duplex,
            mtu,ipv4,ipv4Enabled,manual,address,prefixLength,dhcp;
    setNetworkInterfaces = newElement("wsdl:SetNetworkInterfaces");
    interfaceToken = newElement("wsdl:InterfaceToken");
    networkInterface = newElement("wsdl:NetworkInterface");
    networkInterfaceEnabled = newElement("sch:Enabled",this->networkInfacesEnabled() == true?"true":"false");
    link = newElement("sch:Link");
    autoNegotiation = newElement("sch:AutoNegotiation",this->autoNegotiation() == true?"true":"false");
    speed = newElement("sch:Speed",QString::number(this->speed()));
    duplex = newElement("sch:Duplex",this->duplex()==NetworkInterfaces::Full?"Full":"Half");

    mtu = newElement("sch:MTU",QString::number(this->mtu()));
    ipv4 = newElement("sch:IPv4");
    ipv4Enabled = newElement("sch:Enabled",this->ipv4Enabled() == true?"true":"false");
    manual = newElement("sch:Manual");
    address = newElement("sch:Address",this->ipv4ManualAddress());
    prefixLength = newElement("sch:PrefixLength",QString::number(this->ipv4ManualPrefixLength()));
    dhcp = newElement("sch:DHCP",this->ipv4Enabled() == true?"true":"false");
    setNetworkInterfaces.appendChild(interfaceToken);
    setNetworkInterfaces.appendChild(networkInterface);
    networkInterface.appendChild(networkInterfaceEnabled);
    networkInterface.appendChild(link);
    networkInterface.appendChild(mtu);
    networkInterface.appendChild(ipv4);
    link.appendChild(autoNegotiation);
    link.appendChild(speed);
    link.appendChild(duplex);
    ipv4.appendChild(ipv4Enabled);
    ipv4.appendChild(manual);
    ipv4.appendChild(dhcp);
    manual.appendChild(address);
    manual.appendChild(prefixLength);
    return setNetworkInterfaces;
}


