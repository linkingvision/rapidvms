#ifndef VSC_HDD_ONE_EDIT_H
#define VSC_HDD_ONE_EDIT_H

#include <QDialog>
#include "ui_vschddoneedit.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"

class VSCHddOneEdit : public QWidget
{
    Q_OBJECT

public:
	VSCHddOneEdit(VidStor &stor, VidDisk disk, VidDiskList diskMap, QWidget *parent = 0);
	~VSCHddOneEdit(){}
public slots:  
	void setFreeValue(int value);  
	void diskSelected(bool value);  
public:
	bool isChecked()
	{
		return ui.checkBox->isChecked();
	}
public:
	Ui::VSCHddOneEdit ui;
private:
	VidDiskList m_DiskMap;
	VidDisk m_Disk;
	bool m_added;
	s64 m_totalSize;
	s64 m_freeSize;
	VidStor m_pStor; 
};

#endif // VSC_HDD_ONE_H
