

#ifndef VSCDEVICELIST_H
#define VSCDEVICELIST_H

#include <QWidget>
#include "ui_vscdevicelist.h"
#include "factory.hpp"
#include "QTimer"


#define VSC_DEVICE_INDEX_IPC 0
#define VSC_DEVICE_INDEX_RECORDER 1
#define VSC_DEVICE_INDEX_SITE 1

#define VSC_DEVICE_INDEX_VIEW 2
#define VSC_DEVICE_INDEX_DSIK 3
#define VSC_DEVICE_INDEX_V_IPC 4

class VSCDeviceList : public QWidget
{
    Q_OBJECT

public:
    //QSize sizeHint()
    //{
    //    return QSize(300, 300);
    //}

public:
    VSCDeviceList(QWidget *parent);
    ~VSCDeviceList();
public:
	void SetupConnections();
	static bool DeviceChangeCallbackFunc(void* pData, FactoryDeviceChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);
	void NotifyAddCamera(int nId);
	void NotifyDelCamera(int nId);
	void NotifyCameraOnlineOffline(int nId, BOOL bOnline);
	void NotifyCameraRecording(int nId, BOOL bRecording);
	void NotifyAddVms(int nId);
	void NotifyDelVms(int nId);
	void NotifyAddVIPC(int nId);
	void NotifyDelVIPC(int nId);
	void NotifyAddView(int nId);
	void NotifyDelView(int nId);
	/* Camera Group */
	void NotifyAddVGroup(int nId);
	void NotifyDelVGroup(int nId);

public:
	void AddIPCamera(DeviceParam &pParam, QTreeWidgetItem *qtreewidgetitem);
	void AddIPCameraByGroup(DeviceParam &pParam);
	void DelIPCameraByGroup(s32 nId);
	void AddSite(VSCVmsDataItem &pParam);
	void AddView(VSCViewDataItem &pParam);
	void AddVIPC(VIPCDeviceParam &pParam);
	void AddDisk(astring strTitle, int usage);
	void AddVGroup(VSCVGroupDataItem &pParam);
	void RemoveAllCamera();

public slots:
	void CameraTreeUpdated();
	void VIPCTreeUpdated();
	void SurveillanceClick();
	void CameraAddClick();
	void EmapClick();
	void RecordPlanClick();
	void DminingClick();
	void SearchClick();
	void RecorderClick();

	void VIPCAddClick();

	void DiskTreeUpdated();
	void UpdateOnline();
	void VmsTreeUpdated();
	void ViewTreeUpdated();
	void VGroupTreeUpdated();
	void StartRecordAll();
	void StopRecordAll();
	void StartHdfsRecordAll();
	void StopHdfsRecordAll();

	/* VMS */

signals:
	void SurveillanceClicked();
	void EmapClicked();
	void RecordPlanClicked();
	void DminingClicked();
	void SearchClicked();
	void RecorderClicked();

	/*ipc*/
	void CameraAddClicked();
	void CameraEditClicked(int nId);
	void CameraDeleteClicked(int nId);

	void CameraDoubleClicked(int nId);


	/* VIPC */
	void VIPCAddClicked();
	void VIPCEditClicked(int nId);
	void VIPCDeleteClicked(int nId);

	void DiskEditClicked();

	/* View */
	void ViewDeleteClicked(int nId);

	/* Cam Group */
	void VGroupAddClicked();
	void VGroupEditClicked(int nId);
	void VGroupDeleteClicked(int nId);
	void VGroupMapClicked();

	/* VMS */
	void VMSDeleteClicked(int nId);
private:
    Ui::VSCDeviceList ui;
private:
    QTimer *m_Timer;
	QTimer *m_TimerDisk;
    
};

#endif // VSCDEVICELIST_H
