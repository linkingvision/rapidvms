/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
