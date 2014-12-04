#include "vschdddevice.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"


VSCHddDevice::VSCHddDevice(QWidget *parent)
    : QDialog(parent)
{
   m_SelectedDisk = 0;
   m_TotalDisks = 0;
   int i = 0;
   s8 DiskTitle[1024];
   ui.setupUi(this);
   QList<QStorageInfo> hdd = QStorageInfo::mountedVolumes();
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
	if (diskType == HddInternalDrive)
	{
	    /* Add to UI */
           m_Disks[i] = new VSCHddOne(false, i, strHdd, totalSize, 
                                     leftSize, this);
           ui.verticalLayout->addWidget(m_Disks[i] );
           if (i == 0)
           {
               m_Disks[i]->diskSetChecked(true);
           }
           m_TotalDisks ++;
           connect(m_Disks[i], SIGNAL(diskSelectedUpdated(int)), this, SLOT(masterDiskSelected(int)));  
           i ++;
	}
   }
}

void VSCHddDevice::masterDiskSelected(int id)
{
    for (int i = 0; i <m_TotalDisks; i ++ )
    {
        m_Disks[i]->diskSetChecked(false);
    }
    m_Disks[id]->diskSetChecked(true);
    m_SelectedDisk = id;
}

BOOL VSCHddDevice::GetDiskPath(astring &strPath)
{
    m_Disks[m_SelectedDisk]->getStrHdd(strPath);
    return TRUE;
}
s32 VSCHddDevice::GetDiskSize()
{
    s64 diskSize = 0;
    m_Disks[m_SelectedDisk]->getNewUsed(diskSize);
    
    s32 size = diskSize / 1024;

    return size;
}



