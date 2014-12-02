#ifndef MEDIAMANAGEMENT_H
#define MEDIAMANAGEMENT_H
#include "service.h"
#include <QDateTime>
#include "media_management/videosourceconfigurations.h"
#include "media_management/videoencoderconfigurations.h"
#include "media_management/profiles.h"
#include "media_management/profile.h"
#include "media_management/audiosourceconfigurations.h"
#include "media_management/audioencoderconfigurations.h"
#include "media_management/videosourceconfiguration.h"
#include "media_management/videoencoderconfiguration.h"
#include "media_management/audioencoderconfiguration.h"
#include "media_management/audioencoderconfigurationoptions.h"
#include "media_management/videoencoderconfigurationoptions.h"
#include "media_management/streamuri.h"

namespace ONVIF {
    class MediaManagement : public Service {
        Q_OBJECT
    public:
        explicit MediaManagement(const QString & wsdlUrl, const QString &username, const QString &password);
        VideoSourceConfigurations *getVideoSourceConfigurations();
        VideoEncoderConfigurations *getVideoEncoderConfigurations();
        Profiles *getProfiles();
        Profile *getProfile720P();
        Profile *getProfileD1();
        AudioSourceConfigurations *getAudioSourceConfigurations();
        AudioEncoderConfigurations *getAudioEncoderConfigurations();
        VideoSourceConfiguration *getVideoSourceConfiguration();
        VideoEncoderConfiguration *getVideoEncoderConfiguration();
        AudioEncoderConfiguration *getAudioEncoderConfiguration();
        AudioEncoderConfigurationOptions *getAudioEncoderConfigurationOptions();
        VideoEncoderConfigurationOptions *getVideoEncoderConfigurationOptions();
        StreamUri *getStreamUri(const QString &token);
    protected:
        Message *newMessage();
        QHash<QString, QString> namespaces(const QString &key);

    };
}
#endif // MEDIAMANAGEMENT_H
