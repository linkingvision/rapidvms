

#ifndef VSCDEVICE_TREE_H
#define VSCDEVICE_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

#include "factory.hpp"
#include "vscvms.h"
#include "vscvipc.h"
#include "vscviewtree.h"


class VSCDeviceTree : public QTreeWidget
{
    Q_OBJECT
public:
    VSCDeviceTree(QWidget *parent = 0);
public:
	void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent * event);
	void createActions();
	void SetupConnections();

public slots:
    void CameraAddClick();
    void CameraEditClick();
    void CameraDeleteClick();
    void CameraRecordClick();
    void CameraStopRecordClick();
	
    void VIPCAddClick();
    void VIPCEditClick();
    void VIPCDeleteClick();

    void ViewDeleteClick();

	void VSCZbIpcDeleteClick();//hjy
	void VSCZbIpcRecordClick();
	void VSCZbIpcStopRecordClick();

	void VGroupAddClick();
	void VGroupEditClick();
	void VGroupDeleteClick();
	void VGroupMapClick();

signals:
	void CameraAddClicked();
	void CameraEditClicked(int nId);
	void CameraDeleteClicked(int nId);

	void VIPCAddClicked();
	void VIPCEditClicked(int nId);
	void VIPCDeleteClicked(int nId);

	void DiskEditClicked();

	void ViewDeleteClicked(int nId);

	void VGroupAddClicked();
	void VGroupEditClicked(int nId);
	void VGroupDeleteClicked(int nId);
	void VGroupMapClicked();

	void VSCZbIpcDeleteClicked(int nId);//hjy
	//void VSCZbIpcRecordClicked(int nId);
	//void VSCZbIpcStopRecordClicked(int nId);

private:
	QAction *pActDeleteCamera;
	QAction *pActAddCamera;
	QAction *pActEditCamera;
	QAction *pActRecord;
	QAction *pActStopRecord;

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
	
	QAction *pActDelVSCZbIpc;
	QAction *pActRecordVSCZbIpc;
	QAction *pActStopRecordVSCZbIpc;

	QMenu *pPopMenu;
	
    //~VSCDeviceTree();

};

#endif // VSCDEVICE_TREE_H
