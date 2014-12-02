#ifndef ONVIF_SERVICE_H
#define ONVIF_SERVICE_H

#include <QObject>
#include "message.h"
#include "client.h"
#include "messageparser.h"

namespace ONVIF {
    class Service : public QObject {
        Q_OBJECT
    public:
        explicit Service(const QString & wsdlUrl, const QString &username, const QString &password);
        
        ~Service();
        MessageParser *sendMessage(Message &message, const QString &namespaceKey = "");
        MessageParser *sendMessage(Message *message, const QString &namespaceKey = "");
        
    protected:
        virtual QHash<QString, QString> namespaces(const QString &key) = 0;
        Message *createMessage(QHash<QString, QString> &namespaces);
    private:
        QString mUsername, mPassword;
        Client *mClient;
    };
}


#endif // ONVIF_SERVICE_H
