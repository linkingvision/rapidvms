

#ifndef __VSC_V_TREE_H__
#define __VSC_V_TREE_H__

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QMimeData>
#include "server/factory.hpp"

class VE_LIBRARY_API VSCQMimeView : public QMimeData
{
public:
    VSCQMimeView(int nViewId);
    ~VSCQMimeView(){}
public:
    int nId;
	
};


class VE_LIBRARY_API VSCViewTree : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    //VSCViewTree(QTreeWidgetItem *parent, VSCViewDataItem &pParam);
	VSCViewTree(QTreeWidgetItem *parent);
    ~VSCViewTree();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//VSCViewDataItem m_Param;

};

#endif // VSC_V_IPC_H
