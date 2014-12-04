#include "vschddedit.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddoneedit.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCHddEdit::VSCHddEdit(QWidget *parent)
    : QWidget(parent)
{
	int i = 0;
	s8 DiskTitle[1024];
	ui.setupUi(this);
	QList<QStorageInfo> hdd = QStorageInfo::mountedVolumes();
	gFactory->GetDiskMap(m_mapDisk);
	gFactory->GetDiskStatus(m_mapStatus);
	QListIterator<QStorageInfo> it(hdd);
	while(it.hasNext())
	{
		QStorageInfo  disk = it.next();
		if (i >= VSC_SUPPORTED_DISKS)
		{
		  break;
		}
		astring strHdd = disk.rootPath().toStdString();
		VDC_DEBUG( "%s strHdd %s \n",__FUNCTION__, strHdd.c_str());
		s64 totalSize = disk.bytesTotal() / (1024 * 1024);
		   s64 leftSize = disk.bytesFree()/ (1024 * 1024);
		VDC_DEBUG( "%s Total %lld M Left %lld M \n",__FUNCTION__, totalSize, leftSize);  
		QString strQtHdd = disk.rootPath();
		HddDriveType diskType = HddGetDriveType(strQtHdd);
		VDC_DEBUG( "%s Type %d \n",__FUNCTION__,  diskType);
		if (totalSize/1024 < 16 || leftSize/1024 < 4)
		{
		    continue;
		}
		if (diskType == HddInternalDrive 
			|| diskType == HddRemovableDrive || diskType == HddRemoteDrive)
		{
		    /* Add to UI */
		       m_Disks[i] = new VSCHddOneEdit(disk, m_mapDisk, m_mapStatus, this);
		       ui.verticalLayout->addWidget(m_Disks[i] );
		       //m_TotalDisks ++;
		       //connect(m_Disks[i], SIGNAL(diskSelectedUpdated(int)), this, SLOT(masterDiskSelected(int)));  
		       i ++;
		}
	}
#if 0
	for (int j = i; j < 20; j ++)
	{
	       m_Disks[j] = new VSCHddOneEdit(false, j, "d:", 0, 
	                                 0, this);
	       ui.verticalLayout->addWidget(m_Disks[j] );
	}
#endif

   //connect( this->ui.applyButton, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}
#if 0
void VSCHddEdit::masterDiskSelected(int id)
{
    for (int i = 0; i <m_TotalDisks; i ++ )
    {
        m_Disks[i]->diskSetChecked(false);
    }
    m_Disks[id]->diskSetChecked(true);
    m_SelectedDisk = id;
}

BOOL VSCHddEdit::GetDiskPath(astring &strPath)
{
    m_Disks[m_SelectedDisk]->getStrHdd(strPath);
    return TRUE;
}
s32 VSCHddEdit::GetDiskSize()
{
    s64 diskSize = 0;
    m_Disks[m_SelectedDisk]->getNewUsed(diskSize);
    
    s32 size = diskSize / 1024;

    return size;
}


void VSCHddEdit::applyConfig()
{
	bool bUpdate = false;
	for (int i = 0; i < m_TotalDisks; i ++)
	{
		if (m_Disks[i]->isChecked() == false)
		{
			continue;
		}
#ifdef WIN32
	       astring strPath = "C:\\";
#else
		astring strPath = "ve/";//TODO get the hdd from hdd
#endif
		s64 size = 1;
	       m_Disks[i]->getNewUsed(size);
		size = size / 1024;
	       m_Disks[i]->getStrHdd(strPath);
		//gFactory->AddHDD(strPath, size);
		bUpdate = true;
	}
	if (bUpdate == true)
	{
		emit DiskTreeUpdated();
	}

	return;

}
#endif


