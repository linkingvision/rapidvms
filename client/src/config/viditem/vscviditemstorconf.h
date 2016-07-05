#ifndef __VSC_VID_ITEM_STOR_CONF_H__
#define __VSC_VID_ITEM_STOR_CONF_H__

#include "common/vscviditeminf.h"
#include "config/viditem/vscviditemaddcam.h"

class VSCVidItemStorConf : public QObject, public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemStorConf(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemStorConf();
public:
	/* Take care all the child state change */
	//static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange(StorFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId, std::string strCam);
	
public:
	/* Get all the child of this VidStor */
	void TreeUpdated(bool bClear = false);	
	void CameraAdd(VidCamera cCam);
	void CameraDelete(astring strId);
	void CameraOnline(astring strId);
	void CameraOffline(astring strId);
	void CameraRecOn(astring strId);
	void CameraRecOff(astring strId);
	virtual astring GetId(){return m_cStor.strid();}
	virtual void VidFilter(astring strFilter);
private:
	VidStor m_cStor;
	VSCVidItemAddCam *m_pItemAddCam;
};

#endif /* __VSC_VID_ITEM_STOR_CONF_H__ */