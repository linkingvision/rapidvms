#ifndef __VSC_VID_TREE_CONF_H__
#define __VSC_VID_TREE_CONF_H__

#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeConf : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeConf(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeConf();

signals:
	void SignalStorAddSelectd();
	void SignalStorConfSelectd(std::string strStor);
	void SignalDiskConfSelectd(std::string strStor);
	void SignalClientConfSelectd();
	void SignalViewConfSelectd();
	void SignalEmapConfSelectd();
	void SignalCamAddSelectd(std::string strStor);
	void SignalCamConfSelectd(std::string strStor, std::string strCam);
	
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the child state change */
	//static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange(StorFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId, std::string strCam);
	
public:
	void TreeUpdate();
	void StorAdd(VidCameraId cId);
	void StorDel(VidCameraId cId);
	void StorOnline(VidCameraId cId, bool bOnline);
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif