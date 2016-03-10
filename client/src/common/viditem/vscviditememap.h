#ifndef __VSC_VID_ITEM_EMAP_H__
#define __VSC_VID_ITEM_EMAP_H__

#include "common/vscviditeminf.h"

class VSCVidItemEmap : public VSCVidItemInf
{
public:
    VSCVidItemEmap(VidEmap cEmap, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemEmap();
public:
	virtual astring GetId(){return m_cEmap.strid();}
	virtual astring GetName(){return m_cEmap.strname();}
	virtual void VidFilter(astring strFilter);
private:
	VidEmap m_cEmap;

};

#endif /* __VSC_VID_ITEM_EMAP_H__ */