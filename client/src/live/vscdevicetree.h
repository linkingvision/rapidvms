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

#ifndef VSCDEVICE_TREE_H
#define VSCDEVICE_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

#include "server/factory.hpp"
#include "vscviewtree.h"


class VSCDeviceTree : public QTreeWidget
{
    Q_OBJECT
public:
    VSCDeviceTree(QWidget *parent = 0);
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent * event);
	void createActions();
	void SetupConnections();

public slots:
	void CameraAddClick();
	void CameraEditClick();
	void CameraDeleteClick();
	void CameraRecordClick();
	void CameraStopRecordClick();
	void CameraHdfsRecordClick();
	void CameraStopHdfsRecordClick();

	void VIPCAddClick();
	void VIPCEditClick();
	void VIPCDeleteClick();
	void ViewDeleteClick();

	void VGroupAddClick();
	void VGroupEditClick();
	void VGroupDeleteClick();
	void VGroupMapClick();
	void StartRecordAllClick();
	void StopRecordAllClick();
	void StartHdfsRecordAllClick();
	void StopHdfsRecordAllClick();

	/* VMS */
	void VMSDeleteClick();
	void VMSRefreshClick();

signals:
	void CameraAddClicked();
	void CameraEditClicked(int nId);
	void CameraDeleteClicked(int nId);
	void CameraDoubleClicked(int nId);


	void VIPCAddClicked();
	void VIPCEditClicked(int nId);
	void VIPCDeleteClicked(int nId);

	void DiskEditClicked();
	
	void ViewDeleteClicked(int nId);

	void VGroupAddClicked();
	void VGroupEditClicked(int nId);
	void VGroupDeleteClicked(int nId);
	void VGroupMapClicked();

	void StartRecordAllClicked();
	void StopRecordAllClicked();
	void StartHdfsRecordAllClicked();
	void StopHdfsRecordAllClicked();

	/* VMS */
	void VMSDeleteClicked(int nId);

private:
	QAction *pActDeleteCamera;
	QAction *pActAddCamera;
	QAction *pActEditCamera;
	QAction *pActRecord;
	QAction *pActStopRecord;
	QAction *pActHdfsRecord;
	QAction *pActStopHdfsRecord;

	QAction *pActRecordAll;
	QAction *pActStopRecordAll;
	QAction *pActHdfsRecordAll;
	QAction *pActStopHdfsRecordAll;

	/* Group */
	QAction *pActDeleteCamGroup;
	QAction *pActAddCamGroup;
	QAction *pActEditCamGroup;

	QAction *pActCamGroupMap;

	QAction *pActDiskEdit;
	
	QAction *pActEditVIPC;
	QAction *pActAddVIPC;
	QAction *pActDelVIPC;

	/* View */
	QAction *pActDelView;

	/* VMS */
	QAction *pActDelVMS;
	QAction *pActRefreshVMS;

	QMenu *pPopMenu;
	
    //~VSCDeviceTree();

};

#endif // VSCDEVICE_TREE_H
