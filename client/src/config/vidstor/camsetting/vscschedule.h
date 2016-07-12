#ifndef __VSC_SCHEDULE_H__
#define __VSC_SCHEDULE_H__

#include <QWidget>
#include "ui_vscschedule.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCSchedule : public QWidget
{
	Q_OBJECT

public:
	VSCSchedule(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCSchedule(){}

public slots:   
	void applyConfig();	
public:
	Ui::VSCSchedule ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // __VSC_SCHEDULE_H__