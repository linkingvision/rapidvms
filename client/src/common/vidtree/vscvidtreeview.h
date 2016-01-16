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

class VSCVidTreeView : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeView(QWidget *parent = 0);
    ~VSCVidTreeView();
	
public:
	virtual void VidFilter(astring strFilter){}
};

#endif