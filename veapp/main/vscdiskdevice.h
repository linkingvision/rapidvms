#ifndef VSC_DISK_DEVICE_H
#define VSC_DISK_DEVICE_H

#include <QWidget>
#include "ui_vscdiskdevice.h"
#include "utility.hpp"
#include "factory.hpp"
#include "QEvent"


class VSCDiskDevice : public QWidget
{
    Q_OBJECT
public:
    VSCDiskDevice(astring strTitle, int usage, QWidget *parent = 0);
    ~VSCDiskDevice();

public:
    void SetupConnections();
	
public:

public slots:

	
signals:
    //void CameraTreeUpdated();
	void NewDiskDeviceedItem(astring IP, astring Port, astring Manufacturer, astring Model, astring FirmwareVersion, astring ONVIFAddress);
public slots:


public:
    Ui::VSCDiskDevice ui;    

};

#endif // VSCVIEW_H
