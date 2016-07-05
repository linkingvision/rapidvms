#ifndef __VSC_VID_ITEM_CLIENT_CONF_H__
#define __VSC_VID_ITEM_CLIENT_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemClientConf : public QObject, public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemClientConf(ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemClientConf();
private:
	VidCamera m_cCam;
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_CLIENT_CONF_H__ */