#ifndef __VSC_VID_CONF_H__
#define __VSC_VID_CONF_H__

#include "common/vscvidinf.h"
#include "config/vscvidtreeconf.h"

class VSCVidConf : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidConf();
	
public:
	virtual void VidShow();
	virtual void VidHide();
public:
	virtual VSCVidTreeInf *GetConfTree();

public slots:
	void SlotVidStorAdd();
	void SlotVidCamAdd(std::string strStorId);
	void SlotVidClientConf();
	void SlotVidStorConf(std::string strStorId);
	void SlotVidDiskConf(std::string strStorId);
	void SlotVidCamConf(std::string strStor, std::string strCam);

private:
	VSCVidTreeInf *m_pConfTree;

};

#endif
	