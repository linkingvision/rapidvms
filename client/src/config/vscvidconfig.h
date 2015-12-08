#ifndef __VSC_VID_CONF_H__
#define __VSC_VID_CONF_H__

#include "common/vscvidinf.h"

class VSCVidConf : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidConf(ClientFactory &pFactory, QMainWindow *parent);
    ~VSCVidConf();
	
public:
	virtual void VidShow(){}
	virtual void VidHide(){}

private:
	QTabWidget * m_pMainArea;
};

#endif
	