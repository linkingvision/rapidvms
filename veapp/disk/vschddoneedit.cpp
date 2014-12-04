#include "vschddoneedit.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include <QTableWidgetItem>
#include <QMessageBox>

extern Factory *gFactory;

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

VSCHddOneEdit::VSCHddOneEdit(	QStorageInfo hdd, VDBDiskMap &pDisk, 
		VDBDiskStatus &pStatus, QWidget *parent)
    : QWidget(parent), m_pDisk(pDisk), m_pStatus(pStatus), m_hdd(hdd), m_added(false)
{
	s8 DiskTitle[1024];

	ui.setupUi(this);
	m_totalSize = m_hdd.bytesTotal()/(1024 * 1024);
	m_freeSize = m_hdd.bytesFree()/(1024 * 1024);
	astring strName = m_hdd.rootPath().toStdString();
	astring strHdd = m_hdd.device().toStdString();
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	
	VDBDiskMap::iterator it = m_pDisk.find(strHdd), ite = m_pDisk.end();
	if (it == ite)
	{
		m_added = false;
	}else
	{
		m_added = true;
	}
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(strName.c_str()));	
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(GetDiskSizeString(m_totalSize).c_str()));
	ui.tableWidget->setItem(0, 2, new QTableWidgetItem(GetDiskSizeString(m_freeSize).c_str()));
	ui.checkBox->setChecked(m_added);

	ui.horizontalSlider->setMinimum(0); 
	ui.horizontalSlider->setMaximum(m_totalSize); 
	if (m_added == true)
	{
		strLimit = GetDiskSizeString(m_pDisk[strHdd].limit);
		strRecording = GetDiskSizeString(m_pDisk[strHdd].used);
		ui.horizontalSlider->setValue((m_totalSize - m_freeSize) + 
							m_pDisk[strHdd].limit - m_pDisk[strHdd].used);
	}else
	{
		strLimit = GetDiskSizeString(m_freeSize - 256);
		strRecording = "0 M";
		ui.horizontalSlider->setValue(m_totalSize); 
	}
	ui.tableWidget->setItem(0, 3, new QTableWidgetItem(strLimit.c_str()));
	ui.tableWidget->setItem(0, 4, new QTableWidgetItem(strRecording.c_str()));

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setFreeValue(int)));  

	connect(ui.checkBox, SIGNAL( stateChanged(int) ), this, SLOT(diskSelected(int)));

}

void VSCHddOneEdit::diskSelected(int value)
{
	if (m_added == true && value != Qt::Checked)
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
				m_hdd.device().toStdString().c_str(), m_hdd.rootPath().toStdString().c_str());
		gFactory->DelHdd(m_hdd.device().toStdString());
		m_added = false;
		return;
	}
	else if (m_added == false && value == Qt::Checked)
	{
		s64 limitSize = (ui.horizontalSlider->value() - (m_totalSize - m_freeSize));
		VDC_DEBUG( "%s Del Hdd strHdd %s %s limit %lld\n",__FUNCTION__, 
				m_hdd.device().toStdString().c_str(), 
				m_hdd.rootPath().toStdString().c_str(), limitSize);
		gFactory->AddHdd(m_hdd.device().toStdString(), 
						m_hdd.rootPath().toStdString(), limitSize);
		m_added = true;
	}

	return;
}

void VSCHddOneEdit::setFreeValue(int value)
{
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	/* The limit can be less than 2G */
	if (value <= ((m_totalSize - m_freeSize) + 1024 * 2))
	{
		ui.horizontalSlider->setValue((m_totalSize - m_freeSize) + 1024 * 2); 
	}else
	{
		
		if (m_added)
		{
			s64 limitSize = ui.horizontalSlider->value() - (m_totalSize - m_freeSize)
				+ m_pDisk[m_hdd.device().toStdString()].used;
			VDC_DEBUG( "%s HddUpdateSize strHdd %s %s limit %lld\n",__FUNCTION__, 
					m_hdd.device().toStdString().c_str(), 
					m_hdd.rootPath().toStdString().c_str(), limitSize);
			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));
			gFactory->HddUpdateSize(m_hdd.device().toStdString(), limitSize);
			//call api
		}else
		{
			s64 limitSize = (value - (m_totalSize - m_freeSize));
			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));			
		}
		
	}
}



