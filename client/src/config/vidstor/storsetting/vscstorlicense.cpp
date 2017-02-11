#include "debug.hpp"
#include "config/vidstor/storsetting/vscstorlicense.h"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include "cppkit/ck_string.h"
#include "QDateTime"

VSCStorLicense::VSCStorLicense(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor)
{
	ui.setupUi(this);
	UpdateUI();
	connect( this->ui.pushButtonImport, SIGNAL( clicked() ), this, SLOT(SlotImportLic()));
	connect( this->ui.pushButtonDefault, SIGNAL( clicked() ), this, SLOT(SlotDefaultLic()));

}

void VSCStorLicense::UpdateUI()
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	
	astring strLicense = "";
	astring strHostId = "";
	astring strExpire = "";
	astring strStart = "";
	char channel[256];
	int ch = 0;
	astring type = " ";
	syncInf.GetLic(strLicense, strHostId, ch, type, strStart, strExpire);

	cppkit::ck_string ckStart = strStart;
	cppkit::ck_string ckEnd = strExpire;
	s64 nStart = ckStart.to_int64(10);
	s64 nEnd = ckEnd.to_int64(10);

	QDateTime time1, time2;
	time1.setTime_t(nStart);
	time2.setTime_t(nEnd);

	ui.startTime->setDateTime(time1);
	ui.endTime->setDateTime(time2);
	
	memset(channel, 0, 256);
	sprintf(channel, "%d", ch);
	ui.lic->setText(strLicense.c_str());
	ui.hostId->setText(strHostId.c_str());
	ui.channel->setText(channel);
	ui.type->setText(type.c_str());
	
	delete pLoading;

	return;
}

void VSCStorLicense::SlotImportLic()
{
	
	QFileDialog *fileDialog = new QFileDialog(this); 
	fileDialog->setWindowTitle(tr("Select License")); 
	fileDialog->setNameFilter(tr("License(*.lic)")); 
	fileDialog->setOption( QFileDialog::DontUseNativeDialog, true );
	QIcon icon;
	icon.addFile(QStringLiteral(":/logo/resources/vscsmall32.png"), QSize(), QIcon::Normal, QIcon::Off);
	fileDialog->setWindowIcon(icon);
	if(fileDialog->exec() == QDialog::Accepted) { 
		QString path = fileDialog->selectedFiles()[0]; 
		VDC_DEBUG( "%s  QFileDialog %s\n",__FUNCTION__, path.toStdString().c_str());
		
		ifstream b_file (path.toStdString().c_str());
		astring strLic;
		b_file>> strLic;
		
		std::string strLicense = strLic;
		
		VSCLoading * pLoading = VSCLoading::Create();
		StorSyncInf syncInf(m_pStor);
		astring pVer;
		astring strInfo;
		syncInf.Connect();

		syncInf.ConfLic(strLicense);
		UpdateUI();
		if (pLoading)
		{
			delete pLoading;
			pLoading = NULL;
		}
		
		UpdateUI();	
		b_file.close();
	} else 
	{ 
	
	}
	

	return;

}

void VSCStorLicense::SlotDefaultLic()
{
	std::string strLicense = "grC7nvfkFN2zGMbMrXjl3rYmdWbbHqhjhxO0uKgKVq2S2pVL/mUhNATcJOYB5dKhKewHGPkG5VA4ubR0qFjOp0d/MadM/Z1uwWnwXLfgj/3HHrdcSXXVZJxdkK1qZ06lzDTdLzWmTJbC/MKKt56ygsvDqOXjW3VF0NCcYPSO1o8Y8ajtDder4L7gTMkrmG6pUqE1EjHUgdiG4KjSHBh75EIgaGSlhdo3lF8imYSW9sA/uumWzOgLKzmjwsOoeabYVWkWhKv0XQ2dNzNRwvwMf/yd6v/89izgsX+77hBQauxgqU6H9n9jgESdakc/TUI12WIa00NbGIBlJ4z0Fd2e7A==";

	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	
	syncInf.ConfLic(strLicense);
	UpdateUI();
	if (pLoading)
	{
		delete pLoading;
		pLoading = NULL;
	}
	return;
}


