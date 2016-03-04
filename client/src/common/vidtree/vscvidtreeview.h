#ifndef __VSC_VID_TREE_VIEW_H__
#define __VSC_VID_TREE_VIEW_H__

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

class VSCVidTreeView : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeView(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeView();

signals:
	void ViewSelected(std::string strView);
public:
	void mousePressEvent(QMouseEvent *event);	
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the vid Stor state change */
	static bool CallChange(void* pParam, ClientFactoryChangeData data);
	bool CallChange1(ClientFactoryChangeData data);
public:
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif
