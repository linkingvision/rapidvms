#ifndef __VSC_VID_ITEM_CAM_CONF_H__
#define __VSC_VID_ITEM_CAM_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemCamConf : public QObject,public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemCamConf(VidStor cStor, VidCamera cCam, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemCamConf();
public:
	virtual astring GetId(){return m_cCam.strid();}
	virtual void VidFilter(astring strFilter);
	astring GetStorId(){return m_cStor.strid();}
	
private:
	VidCamera m_cCam;
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_CAM_CONF_H__ */