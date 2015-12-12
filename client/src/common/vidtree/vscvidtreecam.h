#ifndef __VSC_VID_TREE_CAM_H__
#define __VSC_VID_TREE_CAM_H__

#include "common/vscvidtreeinf.h"

class VSCVidTreeCam : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeCam(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeCam();
	
public:
	virtual void VidFilter(astring strFilter){}
public:
	/* Take care all the vid Stor state change */
	static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange1(StorFactoryChangeData data);
	
public:
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
};

#endif