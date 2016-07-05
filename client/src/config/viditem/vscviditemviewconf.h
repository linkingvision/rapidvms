#ifndef __VSC_VID_ITEM_VIEW_CONF_H__
#define __VSC_VID_ITEM_VIEW_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemViewConf : public QObject, public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemViewConf(ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemViewConf();
public:

};

#endif /* __VSC_VID_ITEM_ADDSTOR_H__ */