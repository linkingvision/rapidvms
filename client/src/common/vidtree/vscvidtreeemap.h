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

class VSCVidTreeEmap : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeEmap(QWidget *parent = 0);
    ~VSCVidTreeEmap();
	
public:
	virtual void VidFilter(astring strFilter){}
};

#endif