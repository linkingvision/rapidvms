#ifndef REMOVEPRESET_H
#define REMOVEPRESET_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class RemovePreset : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString profileToken READ profileToken WRITE setProfileToken)
        Q_PROPERTY(QString presetToken READ presetToken WRITE setPresetToken)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit RemovePreset(QObject *parent = NULL);
        virtual ~RemovePreset();
        QDomElement toxml();

        bool result() const
        {
            return m_result;
        }

        QString profileToken() const
        {
            return m_profileToken;
        }

        QString presetToken() const
        {
            return m_presetToken;
        }

    public slots:
        void setResult(bool arg)
        {
            m_result = arg;
        }

        void setProfileToken(QString arg)
        {
            m_profileToken = arg;
        }

        void setPresetToken(QString arg)
        {
            m_presetToken = arg;
        }

    private:

        bool m_result;
        QString m_profileToken;
        QString m_presetToken;
    };
}
#endif // REMOVEPRESET_H
