#include "config/vidstor/disksetting/vschddoneedit.h"
#include "debug.hpp"
#include <QTableWidgetItem>
#include <QMessageBox>
#include "client/storsyncinf.hpp"
#include "vscloading.hpp"

static astring GetDiskSizeString(s64 size) 
{
	s8 DiskTitle[1024];
	if (size/1024 > 0)
	{
		if (size / (1024 * 1024) > 0)
		{
	   		sprintf(DiskTitle, "%lld T",  size / (1024 * 1024));
		}else
		{
	   		sprintf(DiskTitle, "%lld G",  size /1024);
		}
	}else
	{
	   sprintf(DiskTitle, "%lld M",  size );
	}

	astring strSize = DiskTitle;
	return strSize;
}

VSCHddOneEdit::VSCHddOneEdit(VidStor &stor, VidDisk disk, VidDiskList diskMap, QWidget *parent)
    : QWidget(parent), m_Disk(disk), m_DiskMap(diskMap), m_added(false), m_pStor(stor)
{
	s8 DiskTitle[1024];
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	ui.setupUi(this);
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
	ui.tableWidget->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	m_totalSize = m_Disk.ntotalsize();
	m_freeSize = m_Disk.nfreesize();
	astring strName = m_Disk.strpath();
	astring strHdd = m_Disk.strid();
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	VidDisk addedDisk;

	m_added = false;

	for (s32 i = 0; i < m_DiskMap.cviddisk_size(); i ++)
	{
		VidDisk disk = m_DiskMap.cviddisk(i);
		if (disk.strid() == m_Disk.strid())
		{
			m_added = true;
			addedDisk = disk;
			break;
		}

	}

	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(strName.c_str()));	
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(GetDiskSizeString(m_totalSize).c_str()));
	ui.tableWidget->setItem(0, 2, new QTableWidgetItem(GetDiskSizeString(m_freeSize).c_str()));
	ui.checkBox->setChecked(m_added);

	ui.horizontalSlider->setMinimum(0); 
	ui.horizontalSlider->setMaximum(m_totalSize/1024); 
	if (m_added == true)
	{
		strLimit = GetDiskSizeString(addedDisk.nstorlimit());
		strRecording = GetDiskSizeString(addedDisk.nstorused());
		ui.horizontalSlider->setValue(addedDisk.nstorlimit()/1024);
	}else
	{
		strLimit = GetDiskSizeString(m_totalSize - 1024);
		strRecording = "0 M";
		ui.horizontalSlider->setValue(m_totalSize/1024); 
	}
	ui.tableWidget->setItem(0, 3, new QTableWidgetItem(strLimit.c_str()));
	ui.tableWidget->setItem(0, 4, new QTableWidgetItem(strRecording.c_str()));

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setFreeValue(int)));  

	connect(ui.checkBox, SIGNAL( toggled(bool) ), this, SLOT(diskSelected(bool)));

}

void VSCHddOneEdit::diskSelected(bool value)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	
	if (m_added == true && value != true)
	{
		QMessageBox msgBox(this);
		//Set text
		msgBox.setText("Delete the Hdd, The Data on this disk will be lost ... ");
		msgBox.setIcon(QMessageBox::Information);

		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);

		int ret = msgBox.exec();
		switch (ret) {
		case QMessageBox::Ok:
		   break;
		default:
			ui.checkBox->setChecked(m_added);
			return;
		   break;
		}
		VDC_DEBUG( "%s Del Hdd strHdd %s %s\n",__FUNCTION__, 
				m_Disk.strid().c_str(), m_Disk.strpath().c_str());
		syncInf.DeleteVidDisk(m_Disk.strid());
		m_added = false;
		delete pLoading;
		return;
	}
	else if (m_added == false && value == true)
	{
		s64 limitSize = ui.horizontalSlider->value() * 1024;
		VDC_DEBUG( "%s Add Hdd strHdd %s %s\n",__FUNCTION__, 
				m_Disk.strid().c_str(), m_Disk.strpath().c_str());
		m_Disk.set_nstorlimit(limitSize);
		
		syncInf.AddVidDisk(m_Disk);
		m_added = true;
	}

	delete pLoading;
	return;
}

void VSCHddOneEdit::setFreeValue(int value)
{
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	/* The limit can be less than 2G */
	if (value <= ((m_totalSize - m_freeSize)/1024 + 1))
	{
		ui.horizontalSlider->setValue((m_totalSize - m_freeSize)/1024 + 1); 
	}else
	{
		
		if (m_added)
		{
			VSCLoading * pLoading = VSCLoading::Create();
			StorSyncInf syncInf(m_pStor);
			astring pVer;
			astring strInfo;
			syncInf.Connect();
			s64 limitSize = ui.horizontalSlider->value() * 1024;
			VDC_DEBUG( "%s HddUpdateSize strHdd %s %s limit %lld\n",__FUNCTION__, 
					m_Disk.strid().c_str(), m_Disk.strpath().c_str(), limitSize);
			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));
			syncInf.UpdateVidDiskLimit(m_Disk.strid(), limitSize);
			delete pLoading;
		}else
		{
			s64 limitSize = (value * 1024);
			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));			
		}
		
	}
}



