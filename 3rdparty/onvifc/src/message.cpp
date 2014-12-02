#include "message.h"
#include <QUuid>
#include <QTime>
#include <QDateTime>
#include <QStringList>
#include <QCryptographicHash>
#include <time.h>
#include <QDebug>

using namespace ONVIF;


QDomElement ONVIF::hashToXml(const QString &name,const QHash<QString, QString> &hash) {
    QDomElement element = newElement(name);
    QHashIterator<QString, QString> i(hash);
    while(i.hasNext()) {
        i.next();
        element.appendChild(newElement(i.key(), i.value()));
    }
    return element;
}

QDomElement ONVIF::listToXml(const QString &name, const QString &itemName, const QStringList &list) {
    QDomElement element = newElement(name);
    for(int i = 0; i < list.length(); i++) {
        element.appendChild(newElement(itemName, list.at(i)));
    }
    return element;
}

QDomElement ONVIF::newElement(const QString &name, const QString &value) {
    QDomDocument doc;
    QDomElement element = doc.createElement(name);
    if(value != "") {
        QDomText textNode = doc.createTextNode(value);
        element.appendChild(textNode);
    }
    doc.appendChild(element);
    return doc.firstChildElement();
}


Message *Message::getOnvifSearchMessage() {
    QHash<QString, QString> namespaces;
    namespaces.insert("a", "http://schemas.xmlsoap.org/ws/2004/08/addressing");
    namespaces.insert("d", "http://schemas.xmlsoap.org/ws/2005/04/discovery");
    namespaces.insert("dn", "http://www.onvif.org/ver10/network/wsdl");
    namespaces.insert("dn", "http://www.onvif.org/ver10/network/wsdl");
    Message *msg = new Message(namespaces);
    QDomElement action = newElement("a:Action", "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe");
    QDomElement message_id = newElement("a:MessageID", "uuid:" + msg->uuid());
    QDomElement to = newElement("a:To", "urn:schemas-xmlsoap-org:ws:2005:04:discovery");
    msg->appendToHeader(action);
    msg->appendToHeader(message_id);
    msg->appendToHeader(to);
    
    QDomElement probe = newElement("d:Probe");
    probe.appendChild(newElement("d:Types", "dn:NetworkVideoTransmitter"));
    probe.appendChild(newElement("d:Scopes"));
    msg->appendToBody(probe);
    
    return msg;
}

#define SOAP_WSSE_NONCELEN	(20)
static bool CalcWssePassword(const QDateTime &current,  const QString &passwd, QString &passwdDigest, QString &nonceBase64)
{
    char buf[SOAP_WSSE_NONCELEN ];
    int nonceInt = qrand();
    QByteArray sha1Output;
    QCryptographicHash *hash = NULL;

    //sprintf(buf, "%8.8x%4.4hx%8.8x", (int)time(NULL), count++, nonceInt);
    memset(buf, 1, SOAP_WSSE_NONCELEN);
    memcpy(buf, &nonceInt, sizeof(nonceInt));
    QByteArray nonce (buf, SOAP_WSSE_NONCELEN);
    QString nonceStr(nonce.toBase64());
    //qDebug() << "nonceStr:";
    //qDebug() <<  nonceStr;
    QString timeStr(current.toTimeSpec(Qt::UTC).toString("yyyy-MM-ddThh:mm:ssZ"));
    //qDebug() << "timeStr:";
    //qDebug() << timeStr;
    
    //QString sha1Input = nonceStr + timeStr + passwd;
    //qDebug() << "sha1Input:";
    //qDebug() << sha1Input;
    hash=new QCryptographicHash(QCryptographicHash::Sha1); 
    //QByteArray sha1Output = QCryptographicHash::hash(sha1Input.toLatin1(), QCryptographicHash::Sha1);

    hash->addData(buf, SOAP_WSSE_NONCELEN);
    hash->addData(timeStr.toLatin1().data(), strlen(timeStr.toLatin1().data()));
    hash->addData(passwd.toLatin1().data(), strlen(passwd.toLatin1().data()));
    sha1Output = hash->result();
    QString passwdDigestStr(sha1Output.toBase64());
    QString sha1OutputStr(sha1Output.toHex());
    //qDebug() << "sha1Output" << sha1OutputStr;
    //qDebug() << "passwdDigestStr:";
    //qDebug() << passwdDigestStr;

    nonceBase64 = nonceStr;
    passwdDigest = passwdDigestStr;

    delete hash;
    
    return true;
}

Message* Message::getMessageWithUserInfo(QHash<QString, QString> &namespaces, const QString &name, const QString &passwd) {
    namespaces.insert("wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd");
    namespaces.insert("wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd");
    Message *msg = new Message(namespaces);
    QDomElement security = newElement("wsse:Security");
    
    QDomElement usernameToken = newElement("wsse:UsernameToken");
    //usernameToken.setAttribute("wsu:Id", "UsernameToken-1");
    //QDateTime current = QDateTime::currentDateTime();
    QDateTime current = QDateTime::currentDateTime();
    //current.setTime_t(0);
#if 0 /* PasswordText */
    QDomElement username = newElement("wsse:Username", name);
    QDomElement password = newElement("wsse:Password", passwd);
    password.setAttribute("Type", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText");
    usernameToken.appendChild(username);
    usernameToken.appendChild(password);
    usernameToken.appendChild(newElement("wsu:Created", current.toTimeSpec(Qt::UTC).toString("yyyy-MM-ddThh:mm:ss")));
#else /* PasswordDigest  */
    QDomElement username = newElement("wsse:Username", name);
    QString passwdDigest;
    QString nonceBase64;
    /* calc passwd Digest and nonce */
    CalcWssePassword(current, passwd, passwdDigest, nonceBase64);

    QDomElement password = newElement("wsse:Password", passwdDigest);
    QDomElement nonce = newElement("wsse:Nonce", nonceBase64);
    password.setAttribute("Type", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest");
    usernameToken.appendChild(username);
    usernameToken.appendChild(password);
    usernameToken.appendChild(nonce);
    usernameToken.appendChild(newElement("wsu:Created", current.toTimeSpec(Qt::UTC).toString("yyyy-MM-ddThh:mm:ssZ")));
#endif
#if 0    
    QDomElement timestamp = newElement("wsu:Timestamp");
    timestamp.setAttribute("wsu:Id", "Timestamp-2");
    timestamp.appendChild(newElement("wsu:Created", current.toTimeSpec(Qt::UTC).toString("yyyy-MM-ddThh:mm:ss")));
    timestamp.appendChild(newElement("wsu:Expires", current.toTimeSpec(Qt::UTC).addSecs(10).toString("yyyy-MM-ddThh:mm:ss")));
#endif    
    security.appendChild(usernameToken);
    //security.appendChild(timestamp);
    msg->appendToHeader(security);
    return msg;
}


Message::Message(const QHash<QString, QString> &namespaces, QObject *parent) : QObject(parent) {
    this->mNamespaces = namespaces;
    mDoc.appendChild(mDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
    mEnv = mDoc.createElementNS("http://www.w3.org/2003/05/soap-envelope", "soap:Envelope");
    mHeader = mDoc.createElement("soap:Header");
    mBody = mDoc.createElement("soap:Body");
}

QString Message::toXmlStr() {
    QHashIterator<QString, QString> i(mNamespaces);
    while (i.hasNext()) {
        i.next();
        mEnv.setAttribute("xmlns:" + i.key(), i.value());
    }
    
    mEnv.appendChild(mHeader);
    mEnv.appendChild(mBody);
    mDoc.appendChild(mEnv);
    return mDoc.toString();
}

QString Message::uuid() {
    QUuid id = QUuid::createUuid();
    return id.toString();
}

void Message::appendToBody(const QDomElement &body) {
    mBody.appendChild(body);
}

void Message::appendToHeader(const QDomElement &header) {
    mHeader.appendChild(header);
}
