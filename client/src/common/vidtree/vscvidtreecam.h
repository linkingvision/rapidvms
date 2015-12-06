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
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
};

#endif