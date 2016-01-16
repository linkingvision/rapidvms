#ifndef __VSC_VID_ITEM_ADDCAM_H__
#define __VSC_VID_ITEM_ADDCAM_H__

#include "common/vscviditeminf.h"

class VSCVidItemAddCam : public VSCVidItemInf
{
public:
    VSCVidItemAddCam(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemAddCam();
public:
	VidStor GetStor(){return m_cStor;}
	virtual astring GetId(){return m_cStor.strid();}
private:
	VidStor m_cStor;
};

#endif /* __VSC_VID_ITEM_ADDCAM_H__ */