#ifndef VSC_HDD_DEVICE_H
#define VSC_HDD_DEVICE_H

#include <QDialog>
#include "ui_vschdddevice.h"
#include "utility.hpp"
#include <QTimer>
#include "hdddevice.hpp"
#include "vschddone.h"

#define VSC_SUPPORTED_DISKS 128

class VSCHddDevice : public QDialog
{
    Q_OBJECT

public:
    VSCHddDevice(QWidget *parent = 0);
    ~VSCHddDevice(){}
public:
    BOOL GetDiskPath(astring &strPath);
    s32 GetDiskSize();
public slots:   
  void masterDiskSelected(int id);  
public:
    Ui::VSCHddDevice ui;
	
private:
    //HddDevice m_Device;
    VSCHddOne *  m_Disks[VSC_SUPPORTED_DISKS];
    s32 m_TotalDisks;
    s32 m_SelectedDisk;

    
};

#endif // VSC_HDD_DEVICE_H
