#ifndef __VSC_VID_ITEM_VIDSTOR_H__
#define __VSC_VID_ITEM_VIDSTOR_H__

#include "common/vscviditeminf.h"

class VSCVidItemVidStor : public VSCVidItemInf
{
public:
	VSCVidItemVidStor(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemVidStor();
private:
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_VIDSTOR_H__ */