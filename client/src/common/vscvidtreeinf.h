#ifndef __VSC_VID_TREE_INF_H__
#define __VSC_VID_TREE_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>
#include "common/vscviditeminf.h"



/*  QTreeWidget::itemClicked for get the item is checked or not */

class VSCVidTreeInf : public QTreeWidget
{
    Q_OBJECT
public:
    VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeInf();
	
public:
	virtual void VidFilter(astring strFilter){}
	virtual void Init(){}
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked){}
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap){}
public slots:
	void SlotItemClicked(QTreeWidgetItem * item, int column);
	
protected:
	ClientFactory &m_pFactory;
};

#endif