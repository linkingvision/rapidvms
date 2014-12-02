#include "service.h"
#include <QDebug>

using namespace ONVIF;

Service::Service(const QString & wsdlUrl, const QString &username, const QString &password) {
    mUsername = username;
    mPassword = password;
    
    mClient = new Client(wsdlUrl);
}

Service::~Service() {
    if(mClient != NULL) {
        delete mClient;
        mClient = NULL;
    }
}

MessageParser *Service::sendMessage(Message &message, const QString &namespaceKey) {

    return sendMessage(&message, namespaceKey);
}

MessageParser *Service::sendMessage(Message *message, const QString &namespaceKey) {
    if(message == NULL) {
        return NULL;
    }
    QString result = mClient->sendData(message->toXmlStr());
    //qDebug() << "receive data ==> " << result;
    if(result == "") {
        return NULL;
    }
    QHash<QString, QString> names = namespaces(namespaceKey);
    return new MessageParser(result, names);
}

Message *Service::createMessage(QHash<QString, QString> &namespaces) {
    return Message::getMessageWithUserInfo(namespaces, mUsername, mPassword);
}

