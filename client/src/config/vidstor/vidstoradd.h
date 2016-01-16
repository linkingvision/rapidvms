#ifndef __VIDEO_VIDSTORADD_H__
#define __VIDEO_VIDSTORADD_H__

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
#include "ui_vidstoradd.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidStorTableItem : public QTableWidgetItem
{
public:
	VidStorTableItem(VidStor &pStor, bool bNew)
	: m_sStor(pStor), m_bNew(bNew)
	{
	}
	~VidStorTableItem(){}
public:
	bool IsNew(){return m_bNew;}
	VidStor GetStor(){return m_sStor;}
private:
	VidStor m_sStor;
	bool m_bNew;
};

class VidStorAdd : public QWidget
{
    Q_OBJECT

public:
	VidStorAdd(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidStorAdd();

public:
	void TreeWidgetUpdate();
	void SetStorUI(VidStor pStor);
	void GetStorUI(VidStor &pStor);

signals:
	void SignalSectionClicked(int row, int column);

public slots:
	void SlotNewStor();
	void SlotDeleteStor();
	void SlotApplyStor();
	void SlotCancelStor();
	void SlotSectionClicked(int row, int column);

public:
    	Ui::VidStorAdd ui;
private:
	ClientFactory &m_pFactory;

};


#endif // __VIDEO_VIDSTORADD_H__
