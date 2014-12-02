#include "videoencoderconfigurationoptions.h"

using namespace ONVIF;
VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions(QObject *parent):QObject(parent)
{

}

VideoEncoderConfigurationOptions::~VideoEncoderConfigurationOptions()
{

}

int VideoEncoderConfigurationOptions::encodingIntervalRangeMax() const
{
    return m_encodingIntervalRangeMax;
}

void VideoEncoderConfigurationOptions::setEncodingIntervalRangeMax(int encodingIntervalRangeMax)
{
    m_encodingIntervalRangeMax = encodingIntervalRangeMax;
}

int VideoEncoderConfigurationOptions::encodingIntervalRangeMin() const
{
    return m_encodingIntervalRangeMin;
}

void VideoEncoderConfigurationOptions::setEncodingIntervalRangeMin(int encodingIntervalRangeMin)
{
    m_encodingIntervalRangeMin = encodingIntervalRangeMin;
}

int VideoEncoderConfigurationOptions::frameRateRangeMax() const
{
    return m_frameRateRangeMax;
}

void VideoEncoderConfigurationOptions::setFrameRateRangeMax(int frameRateRangeMax)
{
    m_frameRateRangeMax = frameRateRangeMax;
}

int VideoEncoderConfigurationOptions::frameRateRangeMin() const
{
    return m_frameRateRangeMin;
}

void VideoEncoderConfigurationOptions::setFrameRateRangeMin(int frameRateRangeMin)
{
    m_frameRateRangeMin = frameRateRangeMin;
}

int VideoEncoderConfigurationOptions::govLengthRangeMax() const
{
    return m_govLengthRangeMax;
}

void VideoEncoderConfigurationOptions::setGovLengthRangeMax(int govLengthRangeMax)
{
    m_govLengthRangeMax = govLengthRangeMax;
}

int VideoEncoderConfigurationOptions::govLengthRangeMin() const
{
    return m_govLengthRangeMin;
}

void VideoEncoderConfigurationOptions::setGovLengthRangeMin(int govLengthRangeMin)
{
    m_govLengthRangeMin = govLengthRangeMin;
}

int VideoEncoderConfigurationOptions::qulityRangeMax() const
{
    return m_qulityRangeMax;
}

void VideoEncoderConfigurationOptions::setQulityRangeMax(int qulityRangeMax)
{
    m_qulityRangeMax = qulityRangeMax;
}

int VideoEncoderConfigurationOptions::qualityRangeMin() const
{
    return m_qualityRangeMin;
}

void VideoEncoderConfigurationOptions::setQualityRangeMin(int qualityRangeMin)
{
    m_qualityRangeMin = qualityRangeMin;
}
