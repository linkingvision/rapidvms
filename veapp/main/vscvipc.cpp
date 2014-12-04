
#include "vscvipc.h"
#include "factory.hpp"
#include "QColor"

extern Factory *gFactory;

VSCVIPC::VSCVIPC(QTreeWidgetItem *parent, VIPCDeviceParam &pParam)
    : QTreeWidgetItem(parent)
{
	m_Param = pParam;
}

void VSCVIPC::mousePressEvent(QMouseEvent *event)
{
	//VDC_DEBUG( "%s \n",__FUNCTION__);
}


void VSCVIPC::UpdateOnline(BOOL bonline)
{

}

void VSCVIPC::UpdateRecord(BOOL bRecording)
{

}

VSCVIPC::~VSCVIPC()
{

}
