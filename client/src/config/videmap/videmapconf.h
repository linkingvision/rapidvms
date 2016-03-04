#ifndef __VIDEO_VID_EMAP_CONF_H__
#define __VIDEO_VID_EMAP_CONF_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include "ui_VidEmapConf.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidEmapConfTableItem : public QTableWidgetItem
{
public:
	VidEmapConfTableItem(VidEmap &p, bool bNew)
		: m_sEmap(p), m_bNew(bNew)
	{
	}
	~VidEmapConfTableItem(){}
public:
	bool IsNew(){return m_bNew;}
	VidEmap Get(){ return m_sEmap; }
private:
	VidEmap m_sEmap;
	bool m_bNew;
};

class VidEmapConf : public QWidget
{
    Q_OBJECT

public:
	VidEmapConf(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidEmapConf();

public:
	void TreeWidgetUpdate();
	void SetEmapUI(VidEmap p);
	void GetEmapUI(VidEmap &p);

signals:
	void SignalSectionClicked(int row, int column);

public slots:
	void SlotNew();
	void SlotDelete();
	void SlotApply();
	void SlotCancel();
	void SlotSectionClicked(int row, int column);

public:
    	Ui::VidEmapConf ui;
private:
	ClientFactory &m_pFactory;

};


#endif // __VIDEO_VID_EMAP_CONF_H__
