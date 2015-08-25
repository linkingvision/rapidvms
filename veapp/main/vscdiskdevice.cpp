#include <QThread>

#include "vscdiskdevice.h"
#include "vscvwidget.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QApplication>

extern Factory *gFactory;

VSCDiskDevice::VSCDiskDevice(astring strTitle, int usage, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.name->setText(strTitle.c_str());
	ui.progressBar->setValue(usage);

}

VSCDiskDevice::~VSCDiskDevice()
{

}

void VSCDiskDevice::SetupConnections()
{

}

