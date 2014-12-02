#ifndef PRESET_H
#define PRESET_H
#include <QObject>
#include <QDomElement>
namespace ONVIF {
    class Preset : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString profileToken READ profileToken WRITE setProfileToken)
        Q_PROPERTY(QString presetName READ presetName WRITE setPresetName)
        Q_PROPERTY(QString presetToken READ presetToken WRITE setPresetToken)
        Q_PROPERTY(bool result READ result WRITE setResult)
    public:
        explicit Preset(QObject *parent = NULL);
        virtual ~Preset();
        QDomElement toxml();
        QString profileToken() const
        {
            return m_profileToken;
        }

        QString presetName() const
        {
            return m_presetName;
        }

        QString presetToken() const
        {
            return m_presetToken;
        }

        bool result() const
        {
            return m_result;
        }

    public slots:
        void setProfileToken(QString arg)
        {
            m_profileToken = arg;
        }

        void setPresetName(QString arg)
        {
            m_presetName = arg;
        }

        void setPresetToken(QString arg)
        {
            m_presetToken = arg;
        }

        void setResult(bool arg)
        {
            m_result = arg;
        }

    private:
        QString m_profileToken;
        QString m_presetName;
        QString m_presetToken;
        bool m_result;
    };
}
#endif // PRESET_H
