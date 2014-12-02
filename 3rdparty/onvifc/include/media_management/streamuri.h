#ifndef STREAMURI_H
#define STREAMURI_H
#include <QObject>
namespace ONVIF {
    class StreamUri : public QObject
    {
       Q_OBJECT
        Q_PROPERTY(QString uri READ uri WRITE setUri)
        Q_PROPERTY(bool invalidAfterConnect READ invalidAfterConnect WRITE setInvalidAfterConnect)
        Q_PROPERTY(bool invalidAfterReboot READ invalidAfterReboot WRITE setInvalidAfterReboot)
        Q_PROPERTY(QString timeout READ timeout WRITE setTimeout)
    public:
        explicit StreamUri(QObject *parent = NULL);
        virtual ~StreamUri();
        QString uri() const
        {
            return m_uri;
        }

        bool invalidAfterConnect() const
        {
            return m_invalidAfterConnect;
        }

        bool invalidAfterReboot() const
        {
            return m_invalidAfterReboot;
        }

        QString timeout() const
        {
            return m_timeout;
        }

    public slots:
        void setUri(QString arg)
        {
            m_uri = arg;
        }

        void setInvalidAfterConnect(bool arg)
        {
            m_invalidAfterConnect = arg;
        }

        void setInvalidAfterReboot(bool arg)
        {
            m_invalidAfterReboot = arg;
        }

        void setTimeout(QString arg)
        {
            m_timeout = arg;
        }

    private:
        QString m_uri;
        bool m_invalidAfterConnect;
        bool m_invalidAfterReboot;
        QString m_timeout;
    };
}
#endif // STREAMURI_H
