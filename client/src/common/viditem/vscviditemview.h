#ifndef __VSC_VID_ITEM_VIEW_H__
#define __VSC_VID_ITEM_VIEW_H__

#include "common/vscviditeminf.h"

class VSCVidItemView : public VSCVidItemInf
{
public:
    VSCVidItemView(VidView cView, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemView();
public:
	virtual astring GetId(){return m_cView.strid();}
	virtual astring GetName(){return m_cView.strname();}
	virtual void VidFilter(astring strFilter);
private:
	VidView m_cView;

};

#endif /* __VSC_VID_ITEM_VIEW_H__ */