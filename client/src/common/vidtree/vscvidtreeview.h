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

class VSCVidTreeInf : public QTreeWidget
{
    Q_OBJECT
public:
    VSCVidTreeInf(QWidget *parent = 0);
    ~VSCVidTreeInf();
	
public:
	virtual void VidFilter(astring strFilter){}
};

#endif