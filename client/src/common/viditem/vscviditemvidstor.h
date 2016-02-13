#ifndef __VSC_VID_ITEM_VIDSTOR_H__
#define __VSC_VID_ITEM_VIDSTOR_H__

#include "common/vscviditeminf.h"

class VSCVidItemVidStor : public VSCVidItemInf
{
public:
	VSCVidItemVidStor(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemVidStor();

public:
	/* Take care all the child state change */
	static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange1(StorFactoryChangeData data);
public:
	/* Get all the child of this VidStor */
	void TreeUpdated(bool bClear = false);
	void CameraAdd(VidCamera cCam);
	void CameraDelete(astring strId);
	void CameraOnline(astring strId);
	void CameraOffline(astring strId);
	void CameraRecOn(astring strId);
	void CameraRecOff(astring strId);
	

public:
	virtual astring GetId(){return m_cStor.strid();}
	virtual void VidFilter(astring strFilter);
private:
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_VIDSTOR_H__ */