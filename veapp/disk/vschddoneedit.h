#ifndef VSC_HDD_ONE_EDIT_H
#define VSC_HDD_ONE_EDIT_H

#include <QDialog>
#include "ui_vschddoneedit.h"
#include "utility.hpp"
#include <QTimer>
#include "hdddevice.hpp"
#include "factory.hpp"

class VSCHddOneEdit : public QWidget
{
    Q_OBJECT

public:
	VSCHddOneEdit(QStorageInfo hdd, VDBDiskMap &pDisk,VDBDiskStatus &pStatus, QWidget *parent = 0);
	~VSCHddOneEdit(){}
public slots:  
	void setFreeValue(int value);  
	void diskSelected(int value);  
public:
	bool isChecked()
	{
		return ui.checkBox->isChecked();
	}
public:
	Ui::VSCHddOneEdit ui;
private:
	VDBDiskMap &m_pDisk;
	VDBDiskStatus &m_pStatus;
	QStorageInfo m_hdd;
	bool m_added;
	s64 m_totalSize;
	s64 m_freeSize;
};

#endif // VSC_HDD_ONE_H
