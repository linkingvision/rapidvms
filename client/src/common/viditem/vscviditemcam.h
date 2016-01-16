#ifndef __VSC_VID_ITEM_CAM_H__
#define __VSC_VID_ITEM_CAM_H__

#include "common/vscviditeminf.h"

class VSCVidItemCam : public VSCVidItemInf
{
public:
    VSCVidItemCam(VidCamera cCam, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemCam();
public:
	virtual astring GetId(){return m_cCam.strid();}
	virtual astring GetName(){return m_cCam.strname();}
	virtual void VidFilter(astring strFilter);
private:
	VidCamera m_cCam;

};

#endif /* __VSC_VID_ITEM_CAM_H__ */