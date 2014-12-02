#ifndef NODES_H
#define NODES_H
#include <QObject>
#include <QList>
#include <QDomElement>
namespace ONVIF {
    class Nodes : public QObject
    {
        Q_OBJECT
    public:
        explicit Nodes(QObject *parent = NULL);
        virtual ~Nodes();
        QList<QString> getPtzNodeToken() const
        {
            return m_ptzNodeToken;
        }
        void setPtzNodeToken(QString ptzNodeToken)
        {
            m_ptzNodeToken.push_back(ptzNodeToken);
        }
        QList<QString> getName() const
        {
            return m_name;
        }
        void setName(QString name)
        {
            m_name.push_back(name);
        }
        QList<QString> getAppsUri() const
        {
            return m_appsUri;
        }
        void setAppsUri(QString appsUri)
        {
            m_appsUri.push_back(appsUri);
        }
        QList<float> getAppsXRangeMin() const
        {
            return m_appsXRangeMin;
        }
        void setAppsXRangeMin(float appsXRangeMin)
        {
            m_appsXRangeMin.push_back(appsXRangeMin);
        }
        QList<float> getAppsXRangeMax() const
        {
            return m_appsXRangeMax;
        }
        void setAppsXRangeMax(float appsXRangeMax)
        {
            m_appsXRangeMax.push_back(appsXRangeMax);
        }
        QList<float> getAppsYRangeMin() const
        {
            return m_appsYRangeMin;
        }
        void setAppsYRangeMin(float appsYRangeMin)
        {
            m_appsYRangeMin.push_back(appsYRangeMin);
        }
        QList<float> getAppsYRangeMax() const
        {
            return m_appsYRangeMax;
        }
        void setAppsYRangeMax(float appsYRangeMax)
        {
            m_appsYRangeMax.push_back(appsYRangeMax);
        }
        QList<QString> getAzpsUri() const
        {
            return m_azpsUri;
        }
        void setAzpsUri(QString azpsUri)
        {
            m_azpsUri.push_back(azpsUri);
        }
        QList<float> getAzpsXRangeMin() const
        {
            return m_azpsXRangeMin;
        }
        void setAzpsXRangeMin(float azpsXRangeMin)
        {
            m_azpsXRangeMin.push_back(azpsXRangeMin);
        }

        QList<float> getAzpsXRangeMax() const
        {
            return m_azpsXRangeMax;
        }
        void setAzpsXRangeMax(float azpsXRanageMax)
        {
            m_azpsXRangeMax.push_back(azpsXRanageMax);
        }

        QList<QString> getRptsUri() const
        {
            return m_rptsUri;
        }
        void setRptsUri(QString rptsUri)
        {
            m_rptsUri.push_back(rptsUri);
        }
        QList<float> getRptsXRangeMin() const
        {
            return m_rptsXRangeMin;
        }
        void setRptsXRangeMin(float rptsXRangeMin)
        {
            m_rptsXRangeMin.push_back(rptsXRangeMin);
        }
        QList<float> getRptsXRangeMax() const
        {
            return m_rptsXRangeMax;
        }
        void setRptsXRangeMax(float rptsXRangeMax)
        {
            m_rptsXRangeMax.push_back(rptsXRangeMax);
        }
        QList<float> getRptsYRangeMin() const
        {
            return m_rptsYRangeMin;
        }
        void setRptsYRangeMin(float rptsYRangeMin)
        {
            m_rptsYRangeMin.push_back(rptsYRangeMin);
        }

        QList<float> getRptsYRangeMax() const
        {
            return m_rptsYRangeMax;
        }
        void setRptsYRangeMax(float rptsYRangeMax)
        {
            m_rptsYRangeMax.push_back(rptsYRangeMax);
        }
        QList<QString> getRztsUri() const
        {
            return m_rztsUri;
        }
        void setRztsUri(QString rztsUri)
        {
            m_rztsUri.push_back(rztsUri);
        }
        QList<float> getRztsXRangeMin() const
        {
            return m_rztsXRangeMin;
        }
        void setRztsXRangeMin(float rztsXRangeMin)
        {
            m_rztsXRangeMin.push_back(rztsXRangeMin);
        }
        QList<float> getRztsXRangeMax() const
        {
            return m_rztsXRangeMax;
        }
        void setRztsXRangeMax(float rztsXRangeMax)
        {
            m_rztsXRangeMax.push_back(rztsXRangeMax);
        }
        QList<QString> getCpvsUri() const
        {
            return m_cpvsUri;
        }

        void setCpvsUri(QString cpvsUri)
        {
            m_cpvsUri.push_back(cpvsUri);
        }

        QList<float> getCpvsXRangeMin() const
        {
            return m_cpvsXRangeMin;
        }

        void setCpvsXRangeMin(float cpvsXRangeMin)
        {
            m_cpvsXRangeMin.push_back(cpvsXRangeMin);
        }

        QList<float> getCpvsXRangeMax() const
        {
            return m_cpvsXRangeMax;
        }
        void setCpvsXRangeMax(float cpvsXRangeMax)
        {
            m_cpvsXRangeMax.push_back(cpvsXRangeMax);
        }

        QList<float> getCpvsYRangeMin() const
        {
            return m_cpvsYRangeMin;
        }

        void setCpvsYRangeMin(float cpvsYRangeMin)
        {
            m_cpvsYRangeMin.push_back(cpvsYRangeMin);
        }

        QList<float> getCpvsYRangeMax() const
        {
            return m_cpvsYRangeMax;
        }
        void setCpvsYRangeMax(float cpvsYRangeMax)
        {
            m_cpvsYRangeMax.push_back(cpvsYRangeMax);
        }

        QList<QString> getCzvsUri() const
        {
            return m_czvsUri;
        }
        void setCzvsUri(QString czvsUri)
        {
            m_czvsUri.push_back(czvsUri);
        }

        QList<float> getCzvsXRangeMin() const
        {
            return m_czvsXRangeMin;
        }
        void setCzvsXRangeMin(float czvsXRangeMin)
        {
            m_czvsXRangeMin.push_back(czvsXRangeMin);
        }
        QList<float> getCzvsXRangeMax() const
        {
            return m_czvsXRangeMax;
        }
        void setCzvsXRangeMax(float czvsXRangeMax)
        {
            m_czvsXRangeMax.push_back(czvsXRangeMax);
        }

        QList<QString> getPssUri() const
        {
            return m_pssUri;
        }
        void setPssUri(QString pssUri)
        {
            m_pssUri.push_back(pssUri);
        }

        QList<float> getPssXRangeMin() const
        {
            return m_pssXRangeMin;
        }
        void setPssXRangeMin(float pssXRangeMin)
        {
            m_pssXRangeMin.push_back(pssXRangeMin);
        }
        QList<float> getPssXRangeMax() const
        {
            return m_pssXRangeMax;
        }
        void setPssXRangeMax(float pssXRangeMax)
        {
            m_pssXRangeMax.push_back(pssXRangeMax);
        }

        QList<QString> getZssUri() const
        {
            return m_zssUri;
        }
        void setZssUri(QString zssUri)
        {
            m_zssUri.push_back(zssUri);
        }
        QList<float> getZssXRangeMin() const
        {
            return m_zssXRangeMin;
        }
        void setZssXRangeMin(float zssXRangeMin)
        {
            m_zssXRangeMin.push_back(zssXRangeMin);
        }

        QList<float> getZssXRangeMax() const
        {
            return m_zssXRangeMax;
        }

        void setZssXRangeMax(float zssXRangeMax)
        {
            m_zssXRangeMax.push_back(zssXRangeMax);
        }

        QList<int> getMaximumNumberOfPresets() const
        {
            return m_maximumNumberOfPresets;
        }

        void setMaximumNumberOfPresets(float maximumNumberOfPresets)
        {
            m_maximumNumberOfPresets.push_back(maximumNumberOfPresets);
        }

        QList<bool> getHomeSupported() const
        {
            return m_homeSupported;
        }

        void setHomeSupported(int homeSupported)
        {
            m_homeSupported.push_back(homeSupported);
        }

    private:
        QList<QString> m_ptzNodeToken;
        QList<QString> m_name;
        QList<QString> m_appsUri;
        QList<float> m_appsXRangeMin;
        QList<float> m_appsXRangeMax;
        QList<float> m_appsYRangeMin;
        QList<float> m_appsYRangeMax;
        QList<QString> m_azpsUri;
        QList<float> m_azpsXRangeMin;
        QList<float> m_azpsXRangeMax;
        QList<QString> m_rptsUri;
        QList<float> m_rptsXRangeMin;
        QList<float> m_rptsXRangeMax;
        QList<float> m_rptsYRangeMin;
        QList<float> m_rptsYRangeMax;
        QList<QString> m_rztsUri;
        QList<float> m_rztsXRangeMin;
        QList<float> m_rztsXRangeMax;
        QList<QString> m_cpvsUri;
        QList<float> m_cpvsXRangeMin;
        QList<float> m_cpvsXRangeMax;
        QList<float> m_cpvsYRangeMin;
        QList<float> m_cpvsYRangeMax;
        QList<QString> m_czvsUri;
        QList<float> m_czvsXRangeMin;
        QList<float> m_czvsXRangeMax;
        QList<QString> m_pssUri;
        QList<float> m_pssXRangeMin;
        QList<float> m_pssXRangeMax;
        QList<QString> m_zssUri;
        QList<float> m_zssXRangeMin;
        QList<float> m_zssXRangeMax;
        QList<int> m_maximumNumberOfPresets;
        QList<bool> m_homeSupported;
    };
}
#endif // NODES_H
