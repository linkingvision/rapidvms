#ifndef __VSC_VID_ITEM_DISK_CONF_H__
#define __VSC_VID_ITEM_DISK_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemDiskConf : public VSCVidItemInf
{
public:
	VSCVidItemDiskConf(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemDiskConf();
public:
	virtual astring GetId(){return m_cStor.strid();}
private:
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_DISK_CONF_H__ */