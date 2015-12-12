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

class VSCVidItemInf : public QTreeWidgetItem
{
public:
	VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent);
	~VSCVidItemInf();
protected:
	ClientFactory &m_pFactory;

};

#endif /* __VSC_VID_ITEM_INF_H__ */