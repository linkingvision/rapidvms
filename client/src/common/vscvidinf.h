#ifndef __VSC_VID_INF_H__
#define __VSC_VID_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "common/vscvidtreeinf.h"

class VSCVidInf : public QObject
{
    Q_OBJECT
public:
    VSCVidInf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidInf();
	
public slots:
	void SlotNewLiveView(){return VidNewLiveView();}
	void SlotNewLivePB(){return VidNewLivePB();}
	void SlotNewEventSearch(){return VidNewEventSearch();}
	void SlotNewMotionSearch(){return VidNewMotionSearch();}
	void SlotNewEmap(std::string strId){return VidNewEmap(strId);}
	
public:
	virtual void VidShow(){}
	virtual void VidHide(){}
	virtual void VidNewLiveView(){}
	virtual void VidNewLivePB(){}
	virtual void VidNewEventSearch(){}
	virtual void VidNewMotionSearch(){}
	virtual void VidNewEmap(std::string strId){}
	virtual VSCVidTreeInf *GetCameraTree (){return NULL;}
	virtual VSCVidTreeInf *GetGroupTree (){return NULL;}
	virtual VSCVidTreeInf *GetEmapTree (){return NULL;}
	virtual VSCVidTreeInf *GetViewTree (){return NULL;}
	virtual VSCVidTreeInf *GetConfTree (){return NULL;}
	virtual QWidget * GetMainView(){return NULL;}
	virtual bool CheckClosed(QWidget * pTab){return false;}

protected:
	ClientFactory &m_pFactory;
	QMainWindow *m_parent;
	QTabWidget & m_pMainArea;
};

#endif