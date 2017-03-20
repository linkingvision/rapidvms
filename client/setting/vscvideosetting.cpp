#include "vscvideosetting.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"
#include <QFileDialog>

extern Factory *gFactory;

VSCVideoSetting::VSCVideoSetting(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	astring strExportPath = "ve";

	gFactory->GetDefaultHWAccel(m_bHWAccel);

	gFactory->GetExportPath(strExportPath);

	

	if (m_bHWAccel  == 1)
	{
	        ui.hwaccel->setChecked(true);
	}else
	{
		ui.hwaccel->setChecked(false);
	}

	ui.fileLoc->setText(strExportPath.c_str());

	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	connect( this->ui.pushButtonDir, SIGNAL( clicked() ), this, SLOT(dirSelect()));
}

void VSCVideoSetting::dirSelect()
{
	QFileDialog *fileDialog = new QFileDialog(this); 
	fileDialog->setWindowTitle(tr("Select Directory")); 
	fileDialog->setDirectory(ui.fileLoc->text()); 
	fileDialog->setOption( QFileDialog::DontUseNativeDialog, true );
	fileDialog->setFileMode(QFileDialog::Directory);
	QIcon icon;
	icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
	fileDialog->setWindowIcon(icon);
	if(fileDialog->exec() == QDialog::Accepted) { 
		QString path = fileDialog->selectedFiles()[0]; 
		ui.fileLoc->setText(path);
		VDC_DEBUG( "%s  QFileDialog %s\n",__FUNCTION__, path.toStdString().c_str());
	} else 
	{ 
	
	}
}





void VSCVideoSetting::applyConfig()
{
	if (ui.hwaccel->isChecked() == true)
	{
		m_bHWAccel  = 1;
	}else
	{
		m_bHWAccel  = 0;
	}

	gFactory->SetDefaultHWAccel(m_bHWAccel);
	astring strExportPath = ui.fileLoc->text().toStdString();
		
	gFactory->SetExportPath(strExportPath);

	return;

}


