#ifndef VIDEOENCODERCONFIGURATIONOPTIONS_H
#define VIDEOENCODERCONFIGURATIONOPTIONS_H
#include <QObject>
#include <QList>
namespace ONVIF {
    class VideoEncoderConfigurationOptions : public QObject
    {
        Q_OBJECT
        Q_ENUMS(H264ProfilesSupported)
    public:
        explicit VideoEncoderConfigurationOptions(QObject *parent = NULL);
        virtual ~VideoEncoderConfigurationOptions();
        enum H264ProfilesSupported {Baseline,Main,Extended,High};
        H264ProfilesSupported stringToEnum(QString s)
        {
            if(s == QString("Baseline"))
                return Baseline;
            if(s == QString("Main"))
                return Main;
            if(s == QString("Extended"))
                return Extended;
            if(s == QString("High"))
                return High;
        }

        int qualityRangeMin() const;
        void setQualityRangeMin(int qualityRangeMin);

        int qulityRangeMax() const;
        void setQulityRangeMax(int qulityRangeMax);

        int govLengthRangeMin() const;
        void setGovLengthRangeMin(int govLengthRangeMin);

        int govLengthRangeMax() const;
        void setGovLengthRangeMax(int govLengthRangeMax);

        int frameRateRangeMin() const;
        void setFrameRateRangeMin(int frameRateRangeMin);

        int frameRateRangeMax() const;
        void setFrameRateRangeMax(int frameRateRangeMax);

        int encodingIntervalRangeMin() const;
        void setEncodingIntervalRangeMin(int encodingIntervalRangeMin);

        int encodingIntervalRangeMax() const;
        void setEncodingIntervalRangeMax(int encodingIntervalRangeMax);

        QList<int> getResAvailableWidth() const
        {
            return m_resAvailableWidth;
        }

        void setResAvailableWidth(int resAvailableWidth)
        {
            m_resAvailableWidth.push_back(resAvailableWidth);
        }

        QList<int> getResAvailableHeight() const
        {
            return m_resAvailableHeight;
        }

        void setResAvailableHeight(int resAvailableHeight)
        {
            m_resAvailableHeight.push_back(resAvailableHeight);
        }

        QList<H264ProfilesSupported> getH264ProfilesSupported() const
        {
            return m_h264ProfilesSupported;
        }

        void setH264ProfilesSupported(H264ProfilesSupported h264ProfilesSupported)
        {
            m_h264ProfilesSupported.push_back(h264ProfilesSupported);
        }

    private:
        int m_qualityRangeMin;
        int m_qulityRangeMax;
        QList<int> m_resAvailableWidth;
        QList<int> m_resAvailableHeight;
        int m_govLengthRangeMin;
        int m_govLengthRangeMax;
        int m_frameRateRangeMin;
        int m_frameRateRangeMax;
        int m_encodingIntervalRangeMin;
        int m_encodingIntervalRangeMax;
        QList<H264ProfilesSupported> m_h264ProfilesSupported;
    };
}
#endif // VIDEOENCODERCONFIGURATIONOPTIONS_H
