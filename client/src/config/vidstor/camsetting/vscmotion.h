#ifndef __VSC_MOTION_H__
#define __VSC_MOTION_H__

#include <QWidget>
#include "ui_vscmotion.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCMotion : public QWidget
{
	Q_OBJECT

public:
	VSCMotion(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCMotion(){}

public slots:   
	void applyConfig();	
public:
	Ui::VSCMotion ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // __VSC_MOTION_H__