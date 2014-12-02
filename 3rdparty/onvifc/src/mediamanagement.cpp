#include "mediamanagement.h"
#include "messageparser.h"
#include <QXmlResultItems>
#include <QDebug>
using namespace ONVIF;
MediaManagement::MediaManagement(const QString & wsdlUrl, const QString &username, const QString &password)
    :Service(wsdlUrl, username, password) {

}

QHash<QString, QString> MediaManagement::namespaces(const QString &key) {
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
Message *MediaManagement::newMessage() {
    QHash<QString, QString> names;
    names.insert("wsdl", "http://www.onvif.org/ver10/media/wsdl");
    names.insert("tt","http://www.onvif.org/ver10/schema");
    names.insert("trt", "http://www.onvif.org/ver10/media/wsdl");
    names.insert("sch", "http://www.onvif.org/ver10/schema");
    return createMessage(names);
}
VideoSourceConfigurations *MediaManagement::getVideoSourceConfigurations()
{
    VideoSourceConfigurations *videoSourceConfigurations = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetVideoSourceConfigurations"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        videoSourceConfigurations = new VideoSourceConfigurations();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configurations");

        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QString name,useCount,sourceToken,bounds,value;
        QDomDocument doc;
        QRect rect;
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&name);
            videoSourceConfigurations->setName(name.trimmed());

            query->setQuery(result->nameSpace()+"./tt:UseCount/string()");
            query->evaluateTo(&useCount);
            videoSourceConfigurations->setUseCount(useCount.trimmed().toInt());

            query->setQuery(result->nameSpace()+ "./tt:SourceToken/string()");
            query->evaluateTo(&sourceToken);
            videoSourceConfigurations->setSourceToken(sourceToken.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Bounds[@width]");
            query->evaluateTo(&bounds);
            doc.setContent(bounds);
            QDomNodeList itemNodeList = doc.elementsByTagName("tt:Bounds");
            for(int i=0; i< itemNodeList.size(); i++){
                QDomNode node = itemNodeList.at(i);
                value = node.toElement().attribute("width");
                rect.setWidth(value.toInt());
                value = node.toElement().attribute("height");
                rect.setHeight(value.toInt());
                value = node.toElement().attribute("x");
                rect.setLeft(value.toInt());
                value = node.toElement().attribute("y");
                rect.setTop(value.toInt());
            }
            videoSourceConfigurations->setBounds(rect);
            item = items.next();
        }
    }
    delete result;
    delete msg;
    return videoSourceConfigurations;
}

VideoEncoderConfigurations *MediaManagement::getVideoEncoderConfigurations()
{
    VideoEncoderConfigurations *videoEncoderConfigurations = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetVideoEncoderConfigurations"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        videoEncoderConfigurations = new VideoEncoderConfigurations();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configurations");
        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QString name,useCount,encoding,width,height,quality,frameRateLimit,encodingInterval,
                bitrateLimit,govLength,h264Profile,type,ipv4Address,ipv6Address,port,ttl,
                autoStart,sessionTimeout;
        QString value,xml;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        while(!item.isNull()){
            query->setFocus(item);

            query->setQuery(result->nameSpace()+".");
            query->evaluateTo(&xml);
            doc.setContent(xml);
            itemNodeList = doc.elementsByTagName("trt:Configurations");
            for(int i=0; i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                value = node.toElement().attribute("token");
                videoEncoderConfigurations->setToken(value.trimmed());
            }

            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&name);
            videoEncoderConfigurations->setName(name.trimmed());

            query->setQuery(result->nameSpace()+"./tt:UseCount/string()");
            query->evaluateTo(&useCount);
            videoEncoderConfigurations->setUseCount(useCount.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Encoding/string()");
            query->evaluateTo(&encoding);
            videoEncoderConfigurations->setEncoding(encoding.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Resolution/tt:Width/string()");
            query->evaluateTo(&width);
            videoEncoderConfigurations->setWidth(width.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Resolution/tt:Height/string()");
            query->evaluateTo(&height);
            videoEncoderConfigurations->setHeight(height.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Quality/string()");
            query->evaluateTo(&quality);
            videoEncoderConfigurations->setQuality(quality.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:RateControl/tt:FrameRateLimit/string()");
            query->evaluateTo(&frameRateLimit);
            videoEncoderConfigurations->setFrameRateLimit(frameRateLimit.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:RateControl/tt:EncodingInterval/string()");
            query->evaluateTo(&encodingInterval);
            videoEncoderConfigurations->setEncodingInterval(encodingInterval.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:RateControl/tt:BitrateLimit/string()");
            query->evaluateTo(&bitrateLimit);
            videoEncoderConfigurations->setBitrateLimit(bitrateLimit.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:H264/tt:GovLength/string()");
            query->evaluateTo(&govLength);
            videoEncoderConfigurations->setGovLength(govLength.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:H264/tt:H264Profile/string()");
            query->evaluateTo(&h264Profile);
            videoEncoderConfigurations->setH264Profile(h264Profile.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&type);
            videoEncoderConfigurations->setType(type.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&ipv4Address);
            videoEncoderConfigurations->setIpv4Address(ipv4Address.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&ipv6Address);
            videoEncoderConfigurations->setIpv6Address(ipv6Address.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Port/string()");
            query->evaluateTo(&port);
            videoEncoderConfigurations->setPort(port.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&ttl);
            videoEncoderConfigurations->setTtl(ttl.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&autoStart);
            videoEncoderConfigurations->setAutoStart(autoStart.trimmed() == "true"?true:false);
            item = items.next();
        }
    }
    delete result;
    delete msg;
    return videoEncoderConfigurations;
}

Profiles *MediaManagement::getProfiles()
{
    Profiles *profiles = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetProfiles"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        profiles = new Profiles();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Profiles");
        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QDomDocument doc;
        QString value,bounds,panTilt,zoom,profileNode;
        QRect rect;
        while(!item.isNull()){
            query->setFocus(item);

            query->setQuery(result->nameSpace()+".");
            query->evaluateTo(&profileNode);
            doc.setContent(profileNode);
            QDomNodeList itemNodeList = doc.elementsByTagName("trt:Profiles");
            QDomNode node;
            for(int i=0; i<itemNodeList.size();i++)
            {
                value = itemNodeList.at(i).toElement().attribute("token");
                profiles->m_toKenPro.push_back(value.trimmed());

                value = itemNodeList.at(i).toElement().attribute("fixed");
                profiles->m_fixed.push_back(value.trimmed() == "true"?true:false);
            }

            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&value);
            profiles->m_namePro.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profiles->m_nameVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profiles->m_useCountVsc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:SourceToken/string()");
            query->evaluateTo(&value);
            profiles->m_sourceTokenVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Bounds");
            query->evaluateTo(&bounds);
            doc.setContent(bounds);
            itemNodeList = doc.elementsByTagName("tt:Bounds");
            for(int i = 0; i<itemNodeList.size();i++){
                node = itemNodeList.at(i);
                value = node.toElement().attribute("width");
                rect.setWidth(value.toInt());
                value = node.toElement().attribute("height");
                rect.setHeight(value.toInt());
                value = node.toElement().attribute("x");
                rect.setLeft(value.toInt());
                value = node.toElement().attribute("y");
                rect.setTop(value.toInt());
            }
            profiles->m_boundsVsc.push_back(rect);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profiles->m_nameVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profiles->m_useCountVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Encoding/string()");
            query->evaluateTo(&value);
            profiles->m_encodingVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Width/string()");
            query->evaluateTo(&value);
            profiles->m_widthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Height/string()");
            query->evaluateTo(&value);
            profiles->m_heightVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Quality/string()");
            query->evaluateTo(&value);
            profiles->m_qualityVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:FrameRateLimit/string()");
            query->evaluateTo(&value);
            profiles->m_frameRateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:EncodingInterval/string()");
            query->evaluateTo(&value);
            profiles->m_encodingIntervalVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:BitrateLimit/string()");
            query->evaluateTo(&value);
            profiles->m_bitrateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:GovLength/string()");
            query->evaluateTo(&value);
            profiles->m_govLengthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:H264Profile/string()");
            query->evaluateTo(&value);
            profiles->m_h264ProfileVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profiles->m_typeVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profiles->m_ipv4AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profiles->m_ipv6AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profiles->m_portVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profiles->m_ttlVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profiles->m_autoStartVec.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profiles->m_sessionTimeoutVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profiles->m_namePtz.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profiles->m_useCountPtz.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:NodeToken/string()");
            query->evaluateTo(&value);
            profiles->m_nodeToken.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsolutePantTiltPositionSpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultAbsolutePantTiltPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsoluteZoomPositionSpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultAbsoluteZoomPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativePanTiltTranslationSpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultRelativePantTiltTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativeZoomTranslationSpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultRelativeZoomTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousPanTiltVelocitySpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultContinuousPantTiltVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousZoomVelocitySpace/string()");
            query->evaluateTo(&value);
            profiles->m_defaultContinuousZoomVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:PanTilt");
            query->evaluateTo(&panTilt);
            doc.setContent(panTilt);
            itemNodeList = doc.elementsByTagName("tt:PanTilt");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profiles->m_panTiltSpace.push_back(node.toElement().attribute("space").trimmed());
                profiles->m_panTiltX.push_back(node.toElement().attribute("x").trimmed().toInt());
                profiles->m_panTiltY.push_back(node.toElement().attribute("y").trimmed().toInt());

            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:Zoom");
            query->evaluateTo(&zoom);
            doc.setContent(zoom);
            itemNodeList = doc.elementsByTagName("tt:Zoom");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profiles->m_zoomSpace.push_back(node.toElement().attribute("space").trimmed());
                profiles->m_zoomX.push_back(node.toElement().attribute("x").trimmed().toInt());
            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZTimeout/string()");
            query->evaluateTo(&value);
            profiles->m_defaultPTZTimeout.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profiles->m_panTiltUri.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profiles->m_xRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profiles->m_xRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Min/string()");
            query->evaluateTo(&value);
            profiles->m_yRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Max/string()");
            query->evaluateTo(&value);
            profiles->m_yRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profiles->m_zoomUri.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profiles->m_xRangeMinZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profiles->m_xRangeMaxZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profiles->m_nameMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profiles->m_useCountMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Status/string()");
            query->evaluateTo(&value);
            profiles->m_status.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Position/string()");
            query->evaluateTo(&value);
            profiles->m_position.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:Filter/string()");
            query->evaluateTo(&value);
            profiles->m_filter.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:SubscriptionPolicy/string()");
            query->evaluateTo(&value);
            profiles->m_subscriptionPolicy.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Analytics/string()");
            query->evaluateTo(&value);
            profiles->m_analytics.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profiles->m_typeMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profiles->m_ipv4AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profiles->m_ipv6AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profiles->m_portMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profiles->m_ttlMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profiles->m_autoStartMc.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profiles->m_sessionTimeoutMc.push_back(value.trimmed());


            item = items.next();
        }
    }
    delete msg;
    delete result;
    return profiles;
}

Profile *MediaManagement::getProfile720P()
{
    Profile *profile = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("<wsdl:ProfileToken>profile_720P</wsdl:ProfileToken>"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        profile = new Profile();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Profile");
        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QDomDocument doc;
        QString value,bounds,panTilt,zoom,profileNode;
        QRect rect;
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+".");
            query->evaluateTo(&profileNode);
            doc.setContent(profileNode);
            QDomNodeList itemNodeList = doc.elementsByTagName("trt:Profiles");
            QDomNode node;
            for(int i=0; i<itemNodeList.size();i++)
            {
                value = itemNodeList.at(i).toElement().attribute("token");
                profile->m_toKenPro.push_back(value.trimmed());
                qDebug() << "===========" << value;
                value = itemNodeList.at(i).toElement().attribute("fixed");
                profile->m_fixed.push_back(value.trimmed() == "true"?true:false);
            }

            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_namePro.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountVsc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:SourceToken/string()");
            query->evaluateTo(&value);
            profile->m_sourceTokenVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Bounds");
            query->evaluateTo(&bounds);
            doc.setContent(bounds);
            itemNodeList = doc.elementsByTagName("tt:Bounds");
            for(int i = 0; i<itemNodeList.size();i++){
                node = itemNodeList.at(i);
                value = node.toElement().attribute("width");
                rect.setWidth(value.toInt());
                value = node.toElement().attribute("height");
                rect.setHeight(value.toInt());
                value = node.toElement().attribute("x");
                rect.setLeft(value.toInt());
                value = node.toElement().attribute("y");
                rect.setTop(value.toInt());
            }
            profile->m_boundsVsc.push_back(rect);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Encoding/string()");
            query->evaluateTo(&value);
            profile->m_encodingVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Width/string()");
            query->evaluateTo(&value);
            profile->m_widthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Height/string()");
            query->evaluateTo(&value);
            profile->m_heightVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Quality/string()");
            query->evaluateTo(&value);
            profile->m_qualityVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:FrameRateLimit/string()");
            query->evaluateTo(&value);
            profile->m_frameRateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:EncodingInterval/string()");
            query->evaluateTo(&value);
            profile->m_encodingIntervalVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:BitrateLimit/string()");
            query->evaluateTo(&value);
            profile->m_bitrateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:GovLength/string()");
            query->evaluateTo(&value);
            profile->m_govLengthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:H264Profile/string()");
            query->evaluateTo(&value);
            profile->m_h264ProfileVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profile->m_typeVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv4AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv6AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profile->m_portVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profile->m_ttlVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profile->m_autoStartVec.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profile->m_sessionTimeoutVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_namePtz.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountPtz.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:NodeToken/string()");
            query->evaluateTo(&value);
            profile->m_nodeToken.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsolutePantTiltPositionSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultAbsolutePantTiltPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsoluteZoomPositionSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultAbsoluteZoomPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativePanTiltTranslationSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultRelativePantTiltTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativeZoomTranslationSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultRelativeZoomTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousPanTiltVelocitySpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultContinuousPantTiltVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousZoomVelocitySpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultContinuousZoomVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:PanTilt");
            query->evaluateTo(&panTilt);
            doc.setContent(panTilt);
            itemNodeList = doc.elementsByTagName("tt:PanTilt");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profile->m_panTiltSpace.push_back(node.toElement().attribute("space").trimmed());
                profile->m_panTiltX.push_back(node.toElement().attribute("x").trimmed().toInt());
                profile->m_panTiltY.push_back(node.toElement().attribute("y").trimmed().toInt());

            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:Zoom");
            query->evaluateTo(&zoom);
            doc.setContent(zoom);
            itemNodeList = doc.elementsByTagName("tt:Zoom");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profile->m_zoomSpace.push_back(node.toElement().attribute("space").trimmed());
                profile->m_zoomX.push_back(node.toElement().attribute("x").trimmed().toInt());
            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZTimeout/string()");
            query->evaluateTo(&value);
            profile->m_defaultPTZTimeout.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profile->m_panTiltUri.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_yRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_yRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profile->m_zoomUri.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMinZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMaxZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Status/string()");
            query->evaluateTo(&value);
            profile->m_status.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Position/string()");
            query->evaluateTo(&value);
            profile->m_position.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:Filter/string()");
            query->evaluateTo(&value);
            profile->m_filter.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:SubscriptionPolicy/string()");
            query->evaluateTo(&value);
            profile->m_subscriptionPolicy.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Analytics/string()");
            query->evaluateTo(&value);
            profile->m_analytics.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profile->m_typeMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv4AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv6AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profile->m_portMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profile->m_ttlMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profile->m_autoStartMc.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profile->m_sessionTimeoutMc.push_back(value.trimmed());


            item = items.next();
        }
    }
    delete msg;
    delete result;
    return profile;
}

Profile *MediaManagement::getProfileD1()
{
    Profile *profile = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("<wsdl:ProfileToken>profile_D1</wsdl:ProfileToken>"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        profile = new Profile();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Profiles");
        QXmlResultItems items;
        query->evaluateTo(&items);
        QXmlItem item = items.next();
        QDomDocument doc;
        QString value,bounds,panTilt,zoom,profileNode;
        QRect rect;
        while(!item.isNull()){
            query->setFocus(item);

            query->setQuery(result->nameSpace()+".");
            query->evaluateTo(&profileNode);
            doc.setContent(profileNode);
            QDomNodeList itemNodeList = doc.elementsByTagName("trt:Profiles");
            QDomNode node;
            for(int i=0; i<itemNodeList.size();i++)
            {
                value = itemNodeList.at(i).toElement().attribute("token");
                profile->m_toKenPro.push_back(value.trimmed());

                value = itemNodeList.at(i).toElement().attribute("fixed");
                profile->m_fixed.push_back(value.trimmed() == "true"?true:false);
            }

            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_namePro.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountVsc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:SourceToken/string()");
            query->evaluateTo(&value);
            profile->m_sourceTokenVsc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoSourceConfiguration/tt:Bounds");
            query->evaluateTo(&bounds);
            doc.setContent(bounds);
            itemNodeList = doc.elementsByTagName("tt:Bounds");
            for(int i = 0; i<itemNodeList.size();i++){
                node = itemNodeList.at(i);
                value = node.toElement().attribute("width");
                rect.setWidth(value.toInt());
                value = node.toElement().attribute("height");
                rect.setHeight(value.toInt());
                value = node.toElement().attribute("x");
                rect.setLeft(value.toInt());
                value = node.toElement().attribute("y");
                rect.setTop(value.toInt());
            }
            profile->m_boundsVsc.push_back(rect);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Encoding/string()");
            query->evaluateTo(&value);
            profile->m_encodingVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Width/string()");
            query->evaluateTo(&value);
            profile->m_widthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Resolution/tt:Height/string()");
            query->evaluateTo(&value);
            profile->m_heightVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Quality/string()");
            query->evaluateTo(&value);
            profile->m_qualityVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:FrameRateLimit/string()");
            query->evaluateTo(&value);
            profile->m_frameRateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:EncodingInterval/string()");
            query->evaluateTo(&value);
            profile->m_encodingIntervalVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:RateControl/tt:BitrateLimit/string()");
            query->evaluateTo(&value);
            profile->m_bitrateLimitVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:GovLength/string()");
            query->evaluateTo(&value);
            profile->m_govLengthVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:H264/tt:H264Profile/string()");
            query->evaluateTo(&value);
            profile->m_h264ProfileVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profile->m_typeVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv4AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv6AddressVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profile->m_portVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profile->m_ttlVec.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profile->m_autoStartVec.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:VideoEncoderConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profile->m_sessionTimeoutVec.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_namePtz.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountPtz.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:NodeToken/string()");
            query->evaluateTo(&value);
            profile->m_nodeToken.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsolutePantTiltPositionSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultAbsolutePantTiltPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultAbsoluteZoomPositionSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultAbsoluteZoomPositionSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativePanTiltTranslationSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultRelativePantTiltTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultRelativeZoomTranslationSpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultRelativeZoomTranslationSpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousPanTiltVelocitySpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultContinuousPantTiltVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultContinuousZoomVelocitySpace/string()");
            query->evaluateTo(&value);
            profile->m_defaultContinuousZoomVelocitySpace.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:PanTilt");
            query->evaluateTo(&panTilt);
            doc.setContent(panTilt);
            itemNodeList = doc.elementsByTagName("tt:PanTilt");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profile->m_panTiltSpace.push_back(node.toElement().attribute("space").trimmed());
                profile->m_panTiltX.push_back(node.toElement().attribute("x").trimmed().toInt());
                profile->m_panTiltY.push_back(node.toElement().attribute("y").trimmed().toInt());

            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZSpeed/tt:Zoom");
            query->evaluateTo(&zoom);
            doc.setContent(zoom);
            itemNodeList = doc.elementsByTagName("tt:Zoom");
            for(int i=0;i<itemNodeList.size();i++)
            {
                node = itemNodeList.at(i);
                profile->m_zoomSpace.push_back(node.toElement().attribute("space").trimmed());
                profile->m_zoomX.push_back(node.toElement().attribute("x").trimmed().toInt());
            }

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:DefaultPTZTimeout/string()");
            query->evaluateTo(&value);
            profile->m_defaultPTZTimeout.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profile->m_panTiltUri.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_yRangeMinPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:PanTiltLimits/tt:Range/tt:YRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_yRangeMaxPt.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:URI/string()");
            query->evaluateTo(&value);
            profile->m_zoomUri.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Min/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMinZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:PTZConfiguration/tt:ZoomLimits/tt:Range/tt:XRange/tt:Max/string()");
            query->evaluateTo(&value);
            profile->m_xRangeMaxZm.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Name/string()");
            query->evaluateTo(&value);
            profile->m_nameMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:UseCount/string()");
            query->evaluateTo(&value);
            profile->m_useCountMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Status/string()");
            query->evaluateTo(&value);
            profile->m_status.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:PTZStatus/tt:Position/string()");
            query->evaluateTo(&value);
            profile->m_position.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:Filter/string()");
            query->evaluateTo(&value);
            profile->m_filter.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Events/tt:SubscriptionPolicy/string()");
            query->evaluateTo(&value);
            profile->m_subscriptionPolicy.push_back(value.trimmed());


            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Analytics/string()");
            query->evaluateTo(&value);
            profile->m_analytics.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            profile->m_typeMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv4AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            profile->m_ipv6AddressMc.push_back(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            profile->m_portMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            profile->m_ttlMc.push_back(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            profile->m_autoStartMc.push_back(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:MetadataConfiguration/tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            profile->m_sessionTimeoutMc.push_back(value.trimmed());


            item = items.next();
        }
    }
    delete msg;
    delete result;
    return profile;
}

AudioSourceConfigurations* MediaManagement::getAudioSourceConfigurations()
{
    AudioSourceConfigurations *audioSourceConfigurations = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetAudioSourceConfigurations"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        audioSourceConfigurations = new AudioSourceConfigurations();
        QString xml,value;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        QXmlQuery *query = result->query();
        QXmlResultItems items;
        QXmlItem item;
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configurations");
        query->evaluateTo(&xml);
        doc.setContent(xml);
        itemNodeList = doc.elementsByTagName("trt:Configurations");
        for(int i=0; i<itemNodeList.size();i++)
        {
            node = itemNodeList.at(i);
            audioSourceConfigurations->setToken(node.toElement().attribute("token").trimmed());
        }
        query->evaluateTo(&items);
        item = items.next();
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&value);
            audioSourceConfigurations->setName(value.trimmed());
            query->setQuery(result->nameSpace()+"./tt:UseCount/string()");
            query->evaluateTo(&value);
            audioSourceConfigurations->setUseCount(value.trimmed().toInt());
            query->setQuery(result->nameSpace()+"./tt:SourceToken/string()");
            query->evaluateTo(&value);
            audioSourceConfigurations->setSourceToken(value.trimmed());
            item = items.next();
        }
    }

    delete msg;
    delete result;
    return audioSourceConfigurations;
}

AudioEncoderConfigurations* MediaManagement::getAudioEncoderConfigurations()
{
    AudioEncoderConfigurations *audioEncoderConfigurations = NULL;
    Message *msg = newMessage();
    msg->appendToBody(newElement("wsdl:GetAudioEncoderConfigurations"));
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        audioEncoderConfigurations = new AudioEncoderConfigurations();
        QXmlQuery *query = result->query();
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configurations");
        QXmlResultItems  items;
        QXmlItem item;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        QString value,xml;
        query->evaluateTo(&xml);
        doc.setContent(xml);
        itemNodeList = doc.elementsByTagName("trt:Configurations");
        for(int i=0;i<itemNodeList.size();i++)
        {
            node = itemNodeList.at(i);
            value = node.toElement().attribute("token");
            audioEncoderConfigurations->setToken(value.trimmed());
        }

        query->evaluateTo(&items);
        item = items.next();
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Name/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setName(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:UseCount/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setUseCount(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Encoding/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setEncoding(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Bitrate/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setBitrate(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:SampleRate/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setSampleRate(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:Type/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setType(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:IPv4Address/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setIpv4Address(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Address/tt:IPv6Address/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setIpv6Address(value.trimmed());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:Port/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setPort(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:TTL/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setTtl(value.trimmed().toInt());

            query->setQuery(result->nameSpace()+"./tt:Multicast/tt:AutoStart/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setAutoStart(value.trimmed() == "true"?true:false);

            query->setQuery(result->nameSpace()+"./tt:SessionTimeout/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurations->setSessionTimeout(value.trimmed());
            item = items.next();
        }
    }
    delete msg;
    delete result;
    return audioEncoderConfigurations;
}

VideoSourceConfiguration *MediaManagement::getVideoSourceConfiguration()
{
    VideoSourceConfiguration *videoSourceConfiguration = NULL;
    Message *msg = newMessage();
    QDomElement token = newElement("wsdl:ConfigurationToken","profile_VideoSource_1");
    QDomElement body = newElement("wsdl:GetVideoSourceConfiguration");
    body.appendChild(token);
    msg->appendToBody(body);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        videoSourceConfiguration = new VideoSourceConfiguration();
        QXmlQuery *query = result->query();
        QString value,xml;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        QRect rect;
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configuration");
        query->evaluateTo(&xml);
        doc.setContent(xml);
        itemNodeList = doc.elementsByTagName("trt:Configuration");
        for(int i=0; i<itemNodeList.size(); i++)
        {
            node = itemNodeList.at(i);
            value = node.toElement().attribute("token");
            videoSourceConfiguration->setToken(value.trimmed());
        }

        videoSourceConfiguration->setName(result->getValue("//tt:Name").trimmed());
        videoSourceConfiguration->setUseCount(result->getValue("//tt:UseCount").trimmed().toInt());
        videoSourceConfiguration->setSourceToken(result->getValue("//tt:SourceToken").trimmed());

        itemNodeList = doc.elementsByTagName("tt:Bounds");
        for(int i=0; i<itemNodeList.size(); i++)
        {
            node = itemNodeList.at(i);
            value = node.toElement().attribute("height");
            rect.setHeight(value.trimmed().toInt());
            value = node.toElement().attribute("width");
            rect.setWidth(value.trimmed().toInt());
            value = node.toElement().attribute("y");
            rect.setLeft(value.trimmed().toInt());
            value = node.toElement().attribute("x");
            rect.setTop(value.trimmed().toInt());
        }
        videoSourceConfiguration->setBounds(rect);
    }
    delete msg;
    delete result;
    return videoSourceConfiguration;
}

VideoEncoderConfiguration *MediaManagement::getVideoEncoderConfiguration()
{
    VideoEncoderConfiguration *videoEncoderConfiguration = NULL;
    Message *msg = newMessage();
    QDomElement token = newElement("wsdl:ConfigurationToken","profile_video_stream_D1");
    QDomElement body = newElement("wsdl:GetVideoEncoderConfiguration");
    body.appendChild(token);
    msg->appendToBody(body);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        videoEncoderConfiguration = new VideoEncoderConfiguration();
        QXmlQuery *query = result->query();
        QString value,xml;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        QRect rect;
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configuration");
        query->evaluateTo(&xml);
        doc.setContent(xml);
        itemNodeList = doc.elementsByTagName("trt:Configuration");
        for(int i=0; i<itemNodeList.size(); i++)
        {
            node = itemNodeList.at(i);
            value = node.toElement().attribute("token");
            videoEncoderConfiguration->setToken(value.trimmed());
        }
        videoEncoderConfiguration->setName(result->getValue("//tt:Name").trimmed());
        videoEncoderConfiguration->setUseCount(result->getValue("//tt:UseCount").trimmed().toInt());
        videoEncoderConfiguration->setEncoding(result->getValue("//tt:Encoding").trimmed());
        videoEncoderConfiguration->setWidth(result->getValue("//tt:Resolution/tt:Width").trimmed().toInt());
        videoEncoderConfiguration->setHeight(result->getValue("//tt:Resolution/tt:Height").trimmed().toInt());
        videoEncoderConfiguration->setQuality(result->getValue("//tt:Quality").trimmed().toInt());
        videoEncoderConfiguration->setFrameRateLimit(result->getValue("//tt:RateControl/tt:FrameRateLimit").trimmed().toInt());
        videoEncoderConfiguration->setEncodingInterval(result->getValue("//tt:RateControl/tt:EncodingInterval").trimmed().toInt());
        videoEncoderConfiguration->setBitrateLimit(result->getValue("//tt:RateControl/tt:BitrateLimit").trimmed().toInt());
        videoEncoderConfiguration->setGovLength(result->getValue("//tt:H264/tt:GovLength").trimmed().toInt());
        videoEncoderConfiguration->setH264Profile(result->getValue("//tt:H264/tt:H264Profile").trimmed());
        videoEncoderConfiguration->setType(result->getValue("//tt:Multicast/tt:Address/tt:Type").trimmed());
        videoEncoderConfiguration->setIpv4Address(result->getValue("//tt:Multicast/tt:Address/tt:IPv4Address").trimmed());
        videoEncoderConfiguration->setIpv6Address(result->getValue("//tt:Multicast/tt:Address/tt:IPv6Address").trimmed());
        videoEncoderConfiguration->setPort(result->getValue("//tt:Multicast/tt:Port").trimmed().toInt());
        videoEncoderConfiguration->setTtl(result->getValue("//tt:Multicast/tt:TTL").trimmed().toInt());
        videoEncoderConfiguration->setAutoStart(result->getValue("//tt:Multicast/tt:AutoStart") == "true"?true:false);
        videoEncoderConfiguration->setSessionTimeout(result->getValue("//tt:SessionTimeout").trimmed());
    }

    delete msg;
    delete result;
    return videoEncoderConfiguration;
}

AudioEncoderConfiguration *MediaManagement::getAudioEncoderConfiguration()
{
    AudioEncoderConfiguration *audioEncoderConfiguration = NULL;
    Message *msg = newMessage();
    QDomElement token = newElement("wsdl:ConfigurationToken","profile_audio_stream_1");
    QDomElement body = newElement("wsdl:GetAudioEncoderConfiguration");
    body.appendChild(token);
    msg->appendToBody(body);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        audioEncoderConfiguration = new AudioEncoderConfiguration();
        QXmlQuery *query = result->query();
        QString value,xml;
        QDomDocument doc;
        QDomNodeList itemNodeList;
        QDomNode node;
        query->setQuery(result->nameSpace()+"doc($inputDocument)//trt:Configuration");
        query->evaluateTo(&xml);
        doc.setContent(xml);
        itemNodeList = doc.elementsByTagName("trt:Configuration");
        for(int i=0; i<itemNodeList.size(); i++)
        {
            node = itemNodeList.at(i);
            value = node.toElement().attribute("token");
            audioEncoderConfiguration->setToken(value.trimmed());
        }
        audioEncoderConfiguration->setName(result->getValue("//tt:Name").trimmed());
        audioEncoderConfiguration->setUseCount(result->getValue("//tt:UseCount").trimmed().toInt());
        audioEncoderConfiguration->setEncoding(result->getValue("//tt:Encoding").trimmed());
        audioEncoderConfiguration->setBitrate(result->getValue("//tt:Bitrate").trimmed().toInt());
        audioEncoderConfiguration->setSampleRate(result->getValue("//tt:SampleRate").trimmed().toInt());
        audioEncoderConfiguration->setType(result->getValue("//tt:Multicast/tt:Address/tt:Type").trimmed());
        audioEncoderConfiguration->setIpv4Address(result->getValue("//tt:Multicast/tt:Address/tt:IPv4Address").trimmed());
        audioEncoderConfiguration->setIpv6Address(result->getValue("//tt:Multicast/tt:Address/tt:IPv6Address").trimmed());
        audioEncoderConfiguration->setPort(result->getValue("//tt:Multicast/tt:Port").trimmed().toInt());
        audioEncoderConfiguration->setTtl(result->getValue("//tt:Multicast/tt:TTL").trimmed().toInt());
        audioEncoderConfiguration->setAutoStart(result->getValue("//tt:Multicast/tt:AutoStart").trimmed() == "true"?true:false);
        audioEncoderConfiguration->setSessionTimeout(result->getValue("//tt:SessionTimeout").trimmed());
    }
    delete msg;
    delete result;
    return audioEncoderConfiguration;
}

AudioEncoderConfigurationOptions *MediaManagement::getAudioEncoderConfigurationOptions()
{
    AudioEncoderConfigurationOptions *audioEncoderConfigurationOptions = NULL;
    Message *msg = newMessage();
    QDomElement configurationToken = newElement("wsdl:ConfigurationToken","profile_audio_stream_1");
    QDomElement profileToken = newElement("wsdl:ProfileToken","profile_CIF");
    QDomElement body = newElement("wsdl:GetAudioEncoderConfigurationOptions");
    body.appendChild(configurationToken);
    body.appendChild(profileToken);
    msg->appendToBody(body);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        audioEncoderConfigurationOptions = new AudioEncoderConfigurationOptions();
        QXmlQuery *query = result->query();
        QXmlResultItems items,items1;
        QXmlItem item,item1;
        QList<int> bitrateList,sampleRateList;
        QString value;
        query->setQuery(result->nameSpace()+"doc($inputDocument)//tt:Options");
        query->evaluateTo(&items);
        item = items.next();
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Encoding/string()");
            query->evaluateTo(&value);
            audioEncoderConfigurationOptions->setEncoding(audioEncoderConfigurationOptions->stringToEnum(value.trimmed()));

            query->setQuery(result->nameSpace()+"./tt:BitrateList/tt:Items");
            query->evaluateTo(&items1);
            item1 = items1.next();
            while(!item1.isNull()){
                query->setFocus(item1);
                query->setQuery(result->nameSpace()+"./string()");
                query->evaluateTo(&value);
                bitrateList.push_back(value.trimmed().toInt());
                item1 = items1.next();
            }
            audioEncoderConfigurationOptions->setBitrateList(bitrateList);
            query->setQuery(result->nameSpace()+"../../tt:SampleRateList/tt:Items");
            query->evaluateTo(&items1);
            item1 = items1.next();
            while(!item1.isNull()){
                query->setFocus(item1);
                query->setQuery(result->nameSpace()+"./string()");
                query->evaluateTo(&value);
                sampleRateList.push_back(value.trimmed().toInt());
                item1 = items1.next();
            }
            audioEncoderConfigurationOptions->setSampleRateList(sampleRateList);
            item = items.next();
        }
    }

    delete msg;
    delete result;
    return audioEncoderConfigurationOptions;
}

VideoEncoderConfigurationOptions *MediaManagement::getVideoEncoderConfigurationOptions()
{
    VideoEncoderConfigurationOptions *videoEncoderConfigurationOptions = NULL;
    Message *msg = newMessage();
    QDomElement configurationToken = newElement("wsdl:ConfigurationToken","profile_VideoSource_1");
    QDomElement profileTokekn = newElement("wsdl:ProfileToken","profile_CIF");
    QDomElement body = newElement("wsdl:GetVideoEncoderConfigurationOptions");
    body.appendChild(configurationToken);
    body.appendChild(profileTokekn);
    msg->appendToBody(body);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        videoEncoderConfigurationOptions = new VideoEncoderConfigurationOptions();
        QXmlQuery *query = result->query();
        QXmlResultItems items;
        QXmlItem item;
        QString value;
        videoEncoderConfigurationOptions->setQualityRangeMin(result->getValue("//tt:QualityRange/tt:Min").trimmed().toInt());
        videoEncoderConfigurationOptions->setQulityRangeMax(result->getValue("//tt:QualityRange/tt:Max").trimmed().toInt());
        query->setQuery(result->nameSpace()+"doc($inputDocument)//tt:H264/tt:ResolutionsAvailable");
        query->evaluateTo(&items);
        item = items.next();
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./tt:Width/string()");
            query->evaluateTo(&value);
            videoEncoderConfigurationOptions->setResAvailableWidth(value.trimmed().toInt());
            query->setQuery(result->nameSpace()+"./tt:Height/string()");
            query->evaluateTo(&value);
            videoEncoderConfigurationOptions->setResAvailableHeight(value.trimmed().toInt());
            item = items.next();
        }
        videoEncoderConfigurationOptions->setGovLengthRangeMin(result->getValue("//tt:H264/tt:GovLengthRange/tt:Min").trimmed().toInt());
        videoEncoderConfigurationOptions->setGovLengthRangeMax(result->getValue("//tt:H264/tt:GovLengthRange/tt:Max").trimmed().toInt());
        videoEncoderConfigurationOptions->setFrameRateRangeMin(result->getValue("//tt:H264/tt:FrameRateRange/tt:Min").trimmed().toInt());
        videoEncoderConfigurationOptions->setFrameRateRangeMax(result->getValue("//tt:H264/tt:FrameRateRange/tt:Max").trimmed().toInt());
        videoEncoderConfigurationOptions->setEncodingIntervalRangeMin(result->getValue("//tt:H264/tt:EncodingIntervalRange/tt:Min").trimmed().toInt());
        videoEncoderConfigurationOptions->setEncodingIntervalRangeMax(result->getValue("//tt:H264/tt:EncodingIntervalRange/tt:Max").trimmed().toInt());

        query->setQuery(result->nameSpace()+"../tt:H264ProfilesSupported");
        query->evaluateTo(&items);
        item = items.next();
        while(!item.isNull()){
            query->setFocus(item);
            query->setQuery(result->nameSpace()+"./string()");
            query->evaluateTo(&value);
            videoEncoderConfigurationOptions->setH264ProfilesSupported(videoEncoderConfigurationOptions->stringToEnum(value.trimmed()));
            item = items.next();
        }
    }

    delete msg;
    delete result;
    return videoEncoderConfigurationOptions;
}

StreamUri *MediaManagement::getStreamUri(const QString &token)
{
    StreamUri *streamUri = NULL;
    Message *msg = newMessage();
    QDomElement stream =newElement("sch:Stream","RTP-Unicast");
    QDomElement transport = newElement("sch:Transport");
    QDomElement protocol = newElement("sch:Protocol","RTSP");
    QDomElement tunnel = newElement("sch:Tunnel");
    QDomElement streamSetup = newElement("wsdl:StreamSetup");
    QDomElement getStreamUri = newElement("wsdl:GetStreamUri");
    QDomElement profileToken = newElement("wsdl:ProfileToken",token);
    getStreamUri.appendChild(streamSetup);
    getStreamUri.appendChild(profileToken);
    streamSetup.appendChild(stream);
    streamSetup.appendChild(transport);
    transport.appendChild(protocol);
    //transport.appendChild(tunnel);
    msg->appendToBody(getStreamUri);
    MessageParser *result = sendMessage(msg);
    if(result != NULL){
        streamUri = new StreamUri();
        streamUri->setUri(result->getValue("//tt:Uri").trimmed());
        streamUri->setInvalidAfterConnect(result->getValue("//tt:InvalidAfterConnect").trimmed() == "true"?true:false);
        streamUri->setInvalidAfterReboot(result->getValue("//tt:InvalidAfterReboot").trimmed() == "true"?true:false);
        streamUri->setTimeout(result->getValue("//tt:Timeout").trimmed());
    }
    delete msg;
    delete result;
    return streamUri;
}
