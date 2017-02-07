#ifndef __VSC_VID_ITEM_INF_H__
#define __VSC_VID_ITEM_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

typedef std::map<astring, VidCameraId> VidCameraIdMap;

class VSCVidItemInf : public QTreeWidgetItem
{
public:
	VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemInf();
public:
	void UpdateOnline(bool bOnline);
	void UpdateRec(bool bRec);

public:
	virtual  void VidSetChecked(QTreeWidgetItem * item, bool bChecked);
	virtual  bool VidGetChecked();
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked){}
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap){}
	
public:
	virtual astring GetId(){return __FUNCTION__;}
	virtual astring GetName(){return __FUNCTION__;}
	virtual void VidFilter(astring strFilter){}

protected:
	ClientFactory &m_pFactory;
	bool m_bOnline;
	bool m_bRec;
	/* if the check has been called 
	    use for live and searchpb, search pb will call bsetchecked
	*/
	bool m_bSetChecked;

};

#endif /* __VSC_VID_ITEM_INF_H__ */