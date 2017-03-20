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
#ifndef __VSC_VID_LIST_H__
#define __VSC_VID_LIST_H__

#include <QWidget>
#include <QVBoxLayout>
#include "ui_vscvidlist.h"
#include "server/factory.hpp"
#include "common/vscvidinf.h"
#include "QTimer"

class VSCVidList : public QWidget
{
	Q_OBJECT
public:
	VSCVidList(QWidget *parent);
	~VSCVidList();
	void HideAll();
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);

public:
	/* set the live conf or Pb Vid interface */
	void SetCurrVidInf(VSCVidInf *pVidInf);
	
public slots:
	void SlotShowCameraTree();
	void SlotShowGroupTree();
	void SlotShowViewTree();
	void SlotShowEmapTree();
	void FilterChanged(const QString & text);
	void FilterCancel();

private:
	Ui::VSCVidList ui;
private:
	VSCVidInf * m_pVidInf;
	QVBoxLayout* m_treeLayout;
	VSCVidTreeInf *m_pCameraTree;
	VSCVidTreeInf *m_pGroupTree;
	VSCVidTreeInf *m_pViewTree;
	VSCVidTreeInf *m_pEmapTree;
	VSCVidTreeInf *m_pConfTree;
    
};

#endif // __VSC_VID_LIST_H__
