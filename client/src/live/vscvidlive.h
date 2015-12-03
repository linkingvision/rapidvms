#ifndef __VSC_VID_LIVE_H__
#define __VSC_VID_LIVE_H__

#include "common/vscvidinf.h"
#include "live/vscview.h"
#include <QDockWidget>

class VSCVidLive : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidLive(ClientFactory &pFactory, QMainWindow *parent);
    ~VSCVidLive();
	
public:
	virtual void VidShow();
	virtual void VidHide();
	
public slots:
	void MainCloseTab(int index);

private:
	QTabWidget * m_pMainArea;
	VSCView *m_pView;
	VSCDeviceList * m_pDeviceList;
	QDockWidget *m_pDockDevicelist;
};
	