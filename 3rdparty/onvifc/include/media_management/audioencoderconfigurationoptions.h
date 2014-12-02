#ifndef AUDIOENCODERCONFIGURATIONOPTIONS_H
#define AUDIOENCODERCONFIGURATIONOPTIONS_H
#include <QObject>
#include <QList>
namespace ONVIF {
    class AudioEncoderConfigurationOptions : public QObject
    {
        Q_OBJECT
        Q_ENUMS(Encoding)
    public:
        explicit AudioEncoderConfigurationOptions(QObject *parent = NULL);
        virtual ~AudioEncoderConfigurationOptions();
        enum Encoding {G711,G726,AAC};

        Encoding stringToEnum(QString s)
        {
            if(s == QString("G711"))
                return G711;
            if(s == QString("G726"))
                return G726;
            if(s == QString("AAC"))
                return AAC;
        }

        QList<Encoding> getEncoding()
        {
            return m_encoding;
        }

        void setEncoding(Encoding encoding)
        {
            m_encoding.push_back(encoding);
        }

        QList< QList<int> > getBitrateList()
        {
            return m_bitratList;
        }

        void setBitrateList(QList<int> bitrateList)
        {
            m_bitratList.push_back(bitrateList);
        }

        QList< QList<int> > getSampleRateList()
        {
            return m_sampleRateList;
        }

        void setSampleRateList(QList<int> sampleRateList)
        {
            m_sampleRateList.push_back(sampleRateList);
        }
    private:
        QList<Encoding> m_encoding;
        QList< QList<int> > m_bitratList;
        QList< QList<int> > m_sampleRateList;

    };
}
#endif // AUDIOENCODERCONFIGURATIONOPTIONS_H
