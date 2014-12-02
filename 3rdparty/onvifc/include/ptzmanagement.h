#ifndef PTZMANAGEMENT_H
#define PTZMANAGEMENT_H
#include "service.h"
#include "ptz_management/configurations.h"
#include "ptz_management/presets.h"
#include "ptz_management/removepreset.h"
#include "ptz_management/preset.h"
#include "ptz_management/continuousmove.h"
#include "ptz_management/absolutemove.h"
#include "ptz_management/relativemove.h"
#include "ptz_management/stop.h"
#include "ptz_management/nodes.h"
#include "ptz_management/gotopreset.h"
#include "ptz_management/gotohomeposition.h"
#include "ptz_management/configuration.h"
#include "ptz_management/node.h"

namespace ONVIF {
    class PtzManagement : public Service {
        Q_OBJECT
    public:
        explicit PtzManagement(const QString & wsdlUrl, const QString &username, const QString &password);
        Configurations *getConfigurations();
        void getConfiguration(Configuration *configuration);
        void getNode(Node *node);
        Presets *getPresets();
        Nodes *getNodes();

        void removePreset(RemovePreset *removePreset);
        void setPreset(Preset *preset);
        void continuousMove(ContinuousMove *continuousMove);
        void absoluteMove(AbsoluteMove *absoluteMove);
        void relativeMove(RelativeMove *relativeMove);
        void stop(Stop *stop);
        void gotoPreset(GotoPreset *gotoPreset);
        void gotoHomePosition(GotoHomePosition *gotoHomePosition);

    protected:
        Message *newMessage();
        QHash<QString, QString> namespaces(const QString &key);
    };
}
#endif // PTZMANAGEMENT_H
