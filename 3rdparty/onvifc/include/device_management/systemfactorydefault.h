#ifndef SYSTEMFACTORYDEFAULT_H
#define SYSTEMFACTORYDEFAULT_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class SystemFactoryDefault : public QObject {
        Q_OBJECT
        Q_ENUMS(FactoryDefault)
        Q_PROPERTY(FactoryDefault factoryDefault READ factoryDefault WRITE setFactoryDefault)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit SystemFactoryDefault(QObject *parent = NULL);
        virtual ~SystemFactoryDefault();
        enum FactoryDefault {Hard,Soft};
        QDomElement toxml();
        FactoryDefault factoryDefault() const
        {
            return m_factoryDefault;
        }

        bool result() const
        {
            return m_result;
        }

    public slots:
        void setFactoryDefault(FactoryDefault arg)
        {
            m_factoryDefault = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

    private:
        FactoryDefault m_factoryDefault;
        bool m_result;
    };
}
#endif // SYSTEMFACTORYDEFAULT_H
