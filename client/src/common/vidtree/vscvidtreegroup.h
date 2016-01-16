#ifndef __VSC_VID_TREE_GROUP_H__
#define __VSC_VID_TREE_GROUP_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>

class VSCVidTreeGroup : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeGroup(QWidget *parent = 0);
    ~VSCVidTreeGroup();
	
public:
	virtual void VidFilter(astring strFilter){}
};

#endif