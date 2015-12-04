#ifndef __VSC_VID_PB_H__
#define __VSC_VID_PB_H__

#include "common/vscvidinf.h"
#include 

class VSCVidPb : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidPb(ClientFactory &pFactory, QMainWindow *parent);
    ~VSCVidPb();
	
public:
	virtual void VidShow();
	virtual void VidHide();

private:
	QTabWidget * m_pMainArea;
	VSCView *m_pView;
};
#endif
	