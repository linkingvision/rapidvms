#ifndef __VSC_VID_TREE_EMAP_H__
#define __VSC_VID_TREE_EMAP_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeEmap : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeEmap(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeEmap();
signals:
	void EmapSelected(std::string strEmap);
public:
	void mousePressEvent(QMouseEvent *event);	
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the vid Stor state change */
	bool CallChange(ClientFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId);
public:
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif