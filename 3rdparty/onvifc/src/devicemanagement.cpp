#include "devicemanagement.h"
#include <QDebug>

using namespace ONVIF;


DeviceManagement::DeviceManagement(const QString & wsdlUrl, const QString &username, const QString &password) 
    :Service(wsdlUrl, username, password) {
    
}

QHash<QString, QString> DeviceManagement::namespaces(const QString &key) {
    QHash<QString, QString> names;
    names.insert("SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope");
    names.insert("SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding");
    names.insert("xsi", "http://www.w3.org/2001/XMLSchema-instance");
    names.insert("xsd", "http://www.w3.org/2001/XMLSchema");
    names.insert("c14n", "http://www.w3.org/2001/10/xml-exc-c14n#");
    names.insert("wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd");
    names.insert("xenc", "http://www.w3.org/2001/04/xmlenc#");
    names.insert("ds", "http://www.w3.org/2000/09/xmldsig#");
    names.insert("wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd");
    names.insert("wsa5", "http://www.w3.org/2005/08/addressing");
    names.insert("xmime", "http://tempuri.org/xmime.xsd");
    names.insert("xop", "http://www.w3.org/2004/08/xop/include");
    names.insert("wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing");
    names.insert("tt", "http://www.onvif.org/ver10/schema");
    names.insert("wsbf", "http://docs.oasis-open.org/wsrf/bf-2");
    names.insert("wstop", "http://docs.oasis-open.org/wsn/t-1");
    names.insert("d", "http://schemas.xmlsoap.org/ws/2005/04/discovery");
    names.insert("wsr", "http://docs.oasis-open.org/wsrf/r-2");
    names.insert("dndl", "http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding");
    names.insert("dnrd", "http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding");
    names.insert("dn", "http://www.onvif.org/ver10/network/wsdl");
    names.insert("tad", "http://www.onvif.org/ver10/analyticsdevice/wsdl");
    names.insert("tanae", "http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding");
    names.insert("tanre", "http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding");
    names.insert("tan", "http://www.onvif.org/ver20/analytics/wsdl");
    names.insert("tds", "http://www.onvif.org/ver10/device/wsdl");
    names.insert("tetcp", "http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding");
    names.insert("tete", "http://www.onvif.org/ver10/events/wsdl/EventBinding");
    names.insert("tetnc", "http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding");
    names.insert("tetnp", "http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding");
    names.insert("tetpp", "http://www.onvif.org/ver10/events/wsdl/PullPointBinding");
    names.insert("tetpps", "http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding");
    names.insert("tev", "http://www.onvif.org/ver10/events/wsdl");
    names.insert("tetps", "http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding");
    names.insert("wsnt", "http://docs.oasis-open.org/wsn/b-2");
    names.insert("tetsm", "http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding");
    names.insert("timg", "http://www.onvif.org/ver20/imaging/wsdl");
    names.insert("timg10", "http://www.onvif.org/ver10/imaging/wsdl");
    names.insert("tls", "http://www.onvif.org/ver10/display/wsdl");
    names.insert("tmd", "http://www.onvif.org/ver10/deviceIO/wsdl");
    names.insert("tptz", "http://www.onvif.org/ver20/ptz/wsdl");
    names.insert("trc", "http://www.onvif.org/ver10/recording/wsdl");
    names.insert("trp", "http://www.onvif.org/ver10/replay/wsdl");
    names.insert("trt", "http://www.onvif.org/ver10/media/wsdl");
    names.insert("trv", "http://www.onvif.org/ver10/receiver/wsdl");
    names.insert("tse", "http://www.onvif.org/ver10/search/wsdl");
    names.insert("tns1", "http://www.onvif.org/ver10/schema");
    names.insert("tnsn", "http://www.eventextension.com/2011/event/topics");
    names.insert("tnsavg", "http://www.avigilon.com/onvif/ver10/topics");

    return names;
}


QHash<QString, QString> DeviceManagement::getDeviceInformation() {
    QHash<QString, QString> device_info;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetDeviceInformation"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL) {
        device_info.insert("mf", result->getValue("//tds:Manufacturer"));
        device_info.insert("model", result->getValue("//tds:Model"));
        device_info.insert("firmware_version", result->getValue("//tds:FirmwareVersion"));
        device_info.insert("serial_number", result->getValue("//tds:SerialNumber"));
        device_info.insert("hardware_id", result->getValue("//tds:HardwareId"));
    }
    delete result;
    delete msg;
    return device_info;
}

Message *DeviceManagement::newMessage() {
    QHash<QString, QString> names;
    names.insert("wsdl", "http://www.onvif.org/ver10/device/wsdl");
    names.insert("sch", "http://www.onvif.org/ver10/schema");
    return createMessage(names);
}

SystemDateAndTime *DeviceManagement::getSystemDateAndTime() {
    SystemDateAndTime *systemDateAndTime = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetSystemDateAndTime"));
    MessageParser *result = sendMessage(msg);
    
    if(result != NULL) {
        systemDateAndTime = new SystemDateAndTime();
        systemDateAndTime->setProperty("dateTimeType", result->getValue("//tt:DateTimeType"));
        systemDateAndTime->setDaylightSavings(result->getValue("//tt:DaylightSavings") == "true");
        systemDateAndTime->setUtcTime(
            result->getValue("//tt:UTCDateTime/tt:Date/tt:Year").toInt(),
            result->getValue("//tt:UTCDateTime/tt:Date/tt:Month").toInt(),
            result->getValue("//tt:UTCDateTime/tt:Date/tt:Day").toInt(),
            result->getValue("//tt:UTCDateTime/tt:Time/tt:Hour").toInt(),
            result->getValue("//tt:UTCDateTime/tt:Time/tt:Minute").toInt(),
            result->getValue("//tt:UTCDateTime/tt:Time/tt:Second").toInt()
        );
        systemDateAndTime->setLocalTime(
            result->getValue("//tt:LocalDateTime/tt:Date/tt:Year").toInt(),
            result->getValue("//tt:LocalDateTime/tt:Date/tt:Month").toInt(),
            result->getValue("//tt:LocalDateTime/tt:Date/tt:Day").toInt(),
            result->getValue("//tt:LocalDateTime/tt:Time/tt:Hour").toInt(),
            result->getValue("//tt:LocalDateTime/tt:Time/tt:Minute").toInt(),
            result->getValue("//tt:LocalDateTime/tt:Time/tt:Second").toInt()
        );
    }
    
    delete result;
    delete msg;
    return systemDateAndTime;
}

void DeviceManagement::setSystemDateAndTime(SystemDateAndTime *systemDateAndTime)
{
    Message *msg = newMessage();
    msg->appendToBody(systemDateAndTime->toxml());
    qDebug() << msg->toXmlStr();
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        if(result->find("//tds:SetSystemDateAndTimeResponse"))
            systemDateAndTime->setResult(true);
        else
            systemDateAndTime->setResult(false);
        delete result;
        delete msg;
   }
}

void DeviceManagement::setSystemFactoryDefault(SystemFactoryDefault *systemFactoryDefault)
{
    Message *msg = newMessage();
    msg->appendToBody(systemFactoryDefault->toxml());
    qDebug() << msg->toXmlStr();
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        if(result->find("//tds:SetSystemFactoryDefaultResponse"))
            systemFactoryDefault->setResult(true);
        else
            systemFactoryDefault->setResult(false);
        delete result;
        delete msg;
    }
}

void DeviceManagement::systemReboot(SystemReboot *systemReboot)
{
    Message *msg = newMessage();
    msg->appendToBody(systemReboot->toxml());
    qDebug() << msg->toXmlStr();
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        if(result->find("//tds:SystemRebootResponse"))
            systemReboot->setResult(true);
        else
            systemReboot->setResult(false);
        delete result;
        delete msg;
    }
}

Users *DeviceManagement::getUsers()
{
    Users *user = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetUsers"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL) {
        user = new Users();
        user->setProperty("userName", result->getValue("//tt:Username"));
        user->setProperty("passWord",result->getValue("//tt:Password"));
        user->setProperty("userLevel",result->getValue("//tt:UserLevel"));
    }
    delete result;
    delete msg;
    return user;
}

Capabilities *DeviceManagement::getCapabilitiesPtz()
{
    Capabilities *capabilities = NULL;
    Message *msg = newMessage();
    QDomElement cap = newElement("wsdl:GetCapabilities");
    cap.appendChild(newElement("wsdl:Category","PTZ"));
    msg->appendToBody(cap);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        capabilities = new Capabilities();
        capabilities->setProperty("ptzXAddr",result->getValue("//tt:PTZ/tt:XAddr"));
    }
    delete result;
    delete msg;
    return capabilities;
}

Capabilities *DeviceManagement::getCapabilitiesImaging()
{
    Capabilities *capabilities = NULL;
    Message *msg = newMessage();
    QDomElement cap = newElement("wsdl:GetCapabilities");
    cap.appendChild(newElement("wsdl:Category","Imaging"));
    msg->appendToBody(cap);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        capabilities = new Capabilities();
        capabilities->setProperty("imagingXAddr",result->getValue("//tt:Imaging/tt:XAddr"));
    }
    delete result;
    delete msg;
    return capabilities;
}

Capabilities *DeviceManagement::getCapabilitiesMedia()
{
    Capabilities *capabilities = NULL;
    Message *msg = newMessage();
    QDomElement cap = newElement("wsdl:GetCapabilities");
    cap.appendChild(newElement("wsdl:Category","Media"));
    msg->appendToBody(cap);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        capabilities = new Capabilities();
        capabilities->setProperty("mediaXAddr",result->getValue("//tt:Media/tt:XAddr"));
        capabilities->setProperty("rtpMulticast",result->getValue("//tt:RTPMulticast") == "true"?true:false);
        capabilities->setProperty("rtpTcp",result->getValue("//tt:RTP_TCP") == "true"?true:false);
        capabilities->setProperty("rtpRtspTcp",result->getValue("//tt:RTP_RTSP_TCP") == "true"?true:false);
    }
    delete result;
    delete msg;
    return capabilities;
}

Capabilities *DeviceManagement::getCapabilitiesDevice()
{
    Capabilities *capabilities = NULL;
    Message *msg = newMessage();
    QDomElement cap = newElement("wsdl:GetCapabilities");
    cap.appendChild(newElement("wsdl:Category","Device"));
    msg->appendToBody(cap);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        capabilities = new Capabilities();
        capabilities->setProperty("deviceXAddr",result->getValue("//tt:Device/tt:XAddr"));
        capabilities->setProperty("iPFilter",result->getValue("//tt:IPFilter") == "true"?true:false);
        capabilities->setProperty("zeroConfiguration",result->getValue("//tt:ZeroConfiguration") == "true"?true:false);
        capabilities->setProperty("iPVersion6",result->getValue("//tt:IPVersion6") == "true"?true:false);
        capabilities->setProperty("dynDNS",result->getValue("//tt:DynDNS") == "true"?true:false);
        capabilities->setProperty("discoveryResolve",result->getValue("//tt:DiscoveryResolve") == "true"?true:false);
        capabilities->setProperty("discoveryBye",result->getValue("//tt:DiscoveryBye") == "true"?true:false);
        capabilities->setProperty("remoteDiscovery",result->getValue("//tt:RemoteDiscovery") == "true"?true:false);
        capabilities->setProperty("systemBackup",result->getValue("//tt:SystemBackup") == "true"?true:false);
        capabilities->setProperty("systemLogging",result->getValue("//tt:SystemLogging") == "true"?true:false);
        capabilities->setProperty("firmwareUpgrade",result->getValue("//tt:FirmwareUpgrade") == "true"?true:false);
        capabilities->setProperty("major",result->getValue("//tt:Major").toInt());
        capabilities->setProperty("minor",result->getValue("//tt:Minor").toInt());
        capabilities->setProperty("httpFirmwareUpgrade",result->getValue("//tt:HttpFirmwareUpgrade") == "true"?true:false);
        capabilities->setProperty("httpSystemBackup",result->getValue("//tt:HttpSystemBackup") == "true"?true:false);
        capabilities->setProperty("httpSystemLogging",result->getValue("//tt:HttpSystemLogging") == "true"?true:false);
        capabilities->setProperty("httpSupportInformation",result->getValue("//tt:HttpSupportInformation") == "true"?true:false);
        capabilities->setProperty("inputConnectors",result->getValue("//tt:InputConnectors").toInt());
        capabilities->setProperty("relayOutputs",result->getValue("//tt:RelayOutputs").toInt());
        capabilities->setProperty("tls11",result->getValue("//tt:TLS1.1") == "true"?true:false);
        capabilities->setProperty("tls12",result->getValue("//tt:TLS1.2") == "true"?true:false);
        capabilities->setProperty("onboardKeyGeneration",result->getValue("//tt:OnboardKeyGeneration") == "true"?true:false);
        capabilities->setProperty("accessPolicyConfig",result->getValue("//tt:AccessPolicyConfig") == "true"?true:false);
        capabilities->setProperty("x509Token",result->getValue("//tt:X.509Token") == "true"?true:false);
        capabilities->setProperty("samlToken",result->getValue("//tt:SAMLToken") == "true"?true:false);
        capabilities->setProperty("kerberosToken",result->getValue("//tt:KerberosToken") == "true"?true:false);
        capabilities->setProperty("relToken",result->getValue("//tt:RELToken") == "true"?true:false);
        capabilities->setProperty("tls10",result->getValue("//tt:TLS1.0") == "true"?true:false);
        capabilities->setProperty("dot1x",result->getValue("//tt:Dot1x") == "true"?true:false);
        capabilities->setProperty("remoteUserHanding",result->getValue("//tt:RemoteUserHanding") == "true"?true:false);

 }
    delete result;
    delete msg;
    return capabilities;
}



NetworkInterfaces *DeviceManagement::getNetworkInterfaces()
{
    NetworkInterfaces *networkInterfaces = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetNetworkInterfaces"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        networkInterfaces = new NetworkInterfaces();
        networkInterfaces->setProperty("networkInfacesEnabled",result->getValue("//tds:NetworkInterfaces/tt:Enabled"));
        networkInterfaces->setProperty("networkInfacesName",result->getValue("//tt:Name"));
        networkInterfaces->setProperty("hwAaddress",result->getValue("//tt:HwAddress"));
        networkInterfaces->setProperty("mtu",result->getValue("//tt:MTU").toInt());
        networkInterfaces->setProperty("ipv4Enabled",result->getValue("//tt:IPv4/tt:Enabled"));
        networkInterfaces->setProperty("ipv4ManualAddress",result->getValue("//tt:Manual/tt:Address"));
        networkInterfaces->setProperty("ipv4ManualPrefixLength",result->getValue("//tt:Manual/tt:PrefixLength").toInt());
        networkInterfaces->setProperty("ipv4LinkLocalAddress",result->getValue("//tt:LinkLocal/tt:Address"));
        networkInterfaces->setProperty("ipvLinkLocalPrefixLength",result->getValue("//tt:LinkLocal/tt:PrefixLength").toInt());
        networkInterfaces->setProperty("ipv4FromDHCPAddress",result->getValue("//tt:FromDHCP/tt:Address"));
        networkInterfaces->setProperty("ipv4FromDHCPPrefixLength",result->getValue("//tt:FromDHCP/tt:PrefixLength").toInt());
        networkInterfaces->setProperty("ivp4DHCP",result->getValue("//tt:DHCP"));
    }
    delete result;
    delete msg;
    return networkInterfaces;
}

void DeviceManagement::setNetworkInterfaces(NetworkInterfaces *networkInterfaces)
{
    Message *msg = newMessage();
    msg->appendToBody(networkInterfaces->toxml());
    qDebug() << msg->toXmlStr();
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        if(result->find("//tds:SetNetworkInterfacesResponse"))
            networkInterfaces->setResult(true);
        else
            networkInterfaces->setResult(false);
        delete result;
        delete msg;
    }
}


NetworkProtocols *DeviceManagement::getNetworkProtocols()
{
    NetworkProtocols *networkProtocols = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetNetworkProtocols"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        networkProtocols = new NetworkProtocols();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//tds:NetworkProtocols");
        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QString protocolsName,protocolsEnabled,protocolsPort;
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&protocolsName);
            networkProtocols->setNetworkProtocolsName(protocolsName.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Enabled/string()");
            query->evaluateTo(&protocolsEnabled);
            networkProtocols->setNetworkProtocolsEnabled(protocolsEnabled.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:Port/string()");
            query->evaluateTo(&protocolsPort);
            networkProtocols->setNetworkProtocolsPort(protocolsPort.trimmed().toInt());
            item = items.next();
        }

    }
    delete result;
    delete msg;
    return networkProtocols;
}


