#ifndef __VSC_VID_TREE_CAM_H__
#define __VSC_VID_TREE_CAM_H__

#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeCam : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeCam(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeCam();

signals:
	void CameraSelected(std::string strStor, std::string strCam, std::string strCamName);
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	
public:
	virtual void VidFilter(astring strFilter);
public:
	/* Take care all the vid Stor state change */
	static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange1(StorFactoryChangeData data);
	
public:
	void TreeUpdate();
	void StorAdd(VidCameraId cId);
	void StorDel(VidCameraId cId);
	void StorOnline(VidCameraId cId, bool bOnline);
private:
	QTreeWidgetItem * m_pRoot;
};

#endif