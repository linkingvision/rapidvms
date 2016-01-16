#ifndef __VSC_VID_LIVE_H__
#define __VSC_VID_LIVE_H__

#include "common/vscvidinf.h"
#include "live/vscview.h"
#include <QDockWidget>

class VSCVidLive : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidLive(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidLive();
	
public:
	virtual void VidShow();
	virtual void VidHide();
	virtual void VidNewLiveView();
	virtual void VidNewEmap();

	virtual VSCVidTreeInf *GetCameraTree();
	virtual VSCVidTreeInf *GetGroupTree();
	virtual VSCVidTreeInf *GetEmapTree();
	virtual VSCVidTreeInf *GetViewTree();
	
public slots:
	void MainCloseTab(int index);

private:
	VSCView *m_pView;
	VSCVidTreeInf *m_pCameraTree;
	VSCVidTreeInf *m_pGroupTree;
	VSCVidTreeInf *m_pViewTree;
	VSCVidTreeInf *m_pEmapTree;
	
};
	
#endif