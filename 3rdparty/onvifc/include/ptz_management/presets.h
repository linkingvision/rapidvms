#ifndef PRESETS_H
#define PRESETS_H
#include <QObject>
#include <QList>
namespace ONVIF{
    class Presets : public QObject
    {
        Q_OBJECT
    public:
        explicit Presets(QObject *parent = NULL);
        virtual ~Presets();
        QList<QString> getToken() const
        {
            return m_token;
        }

        void setToken(QString token)
        {
            m_token.push_back(token);
        }

        QList<QString> getName() const
        {
            return m_name;
        }

        void setName(QString name)
        {
            m_name.push_back(name);
        }
    private:
        QList<QString> m_token;
        QList<QString> m_name;
    };
}
#endif // PRESETS_H
