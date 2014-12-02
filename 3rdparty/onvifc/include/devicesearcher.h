#ifndef ONVIF_DEVICESEARCHER_H
#define ONVIF_DEVICESEARCHER_H

#include <QObject>
#include <QUdpSocket>

namespace ONVIF {
    class DeviceSearcher : public QObject {
        Q_OBJECT
    public:
        //static DeviceSearcher* searcher;
        static DeviceSearcher* instance(QHostAddress &addr);
		static QList<QHostAddress> getHostAddress();
        explicit DeviceSearcher(QHostAddress &addr, QObject *parent = 0);
        ~DeviceSearcher();
        
        void sendSearchMsg();
    signals:
        void receiveData(const QHash<QString, QString> &data);
    public slots:
    private slots:
        void readPendingDatagrams();
    private:
        QUdpSocket *mUdpSocket;
    };
}
#endif // ONVIF_DEVICESEARCHER_H

