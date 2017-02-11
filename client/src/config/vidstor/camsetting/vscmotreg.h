#ifndef __VSC_MOT_REG_H__
#define __VSC_MOT_REG_H__

#include <QWidget>
#include "ui_vscmotreg.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCMotRegConf : public QWidget
{
	Q_OBJECT
public:
	VSCMotRegConf(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCMotRegConf(){}

public slots:   
	void applyConfig();	
public:
	Ui::VSCMotRegConf ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;
};

#endif // __VSC_MOT_REG_H__