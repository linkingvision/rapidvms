

#ifndef VSC_V_IPC_H
#define VSC_V_IPC_H

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "factory.hpp"

class VSCVIPC : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
    VSCVIPC(QTreeWidgetItem *parent, VIPCDeviceParam &pParam);
    ~VSCVIPC();
	
public:
	u32 GetDeviceId()
	{
	    return m_Param.m_Conf.data.conf.nId;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	VIPCDeviceParam m_Param;

};

#endif // VSC_V_IPC_H
