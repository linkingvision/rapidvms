#ifndef __VSC_VID_SEARCH_PB_H__
#define __VSC_VID_SEARCH_PB_H__

#include "common/vscvidinf.h"

class VSCVidSearchPB : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidSearchPB(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidSearchPB();
	
public:
	virtual void VidShow();
	virtual void VidHide();

private:

};
#endif
	