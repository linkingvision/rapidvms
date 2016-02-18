#ifndef __VSC_VID_PB_H__
#define __VSC_VID_PB_H__

#include "common/vscvidinf.h"

class VSCVidPb : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidPb(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidPb();
	
public:
	virtual void VidShow();
	virtual void VidHide();

private:

};
#endif
	