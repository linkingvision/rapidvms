#ifndef VSC_HDD_EDIT_H
#define VSC_HDD_EDIT_H

#include <QWidget>
#include "ui_vschddedit.h"
#include "utility.hpp"
#include <QTimer>
#include "hdddevice.hpp"
#include "vschddoneedit.h"

#define VSC_SUPPORTED_DISKS 128

class VSCHddEdit : public QWidget
{
	Q_OBJECT

public:
	VSCHddEdit(QWidget *parent = 0);
	~VSCHddEdit(){}
	
public:
	Ui::VSCHddEdit ui;
	
private:
	VSCHddOneEdit *  m_Disks[VSC_SUPPORTED_DISKS];
	VDBDiskMap m_mapDisk;
	VDBDiskStatus m_mapStatus;

    
};

#endif // VSC_HDD_DEVICE_H
