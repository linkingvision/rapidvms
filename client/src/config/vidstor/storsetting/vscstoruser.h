#ifndef __VSC_STOR_USER_H__
#define __VSC_STOR_USER_H__

#include <QWidget>
#include "ui_vscstoruser.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCStorUser : public QWidget
{
	Q_OBJECT

public:
	VSCStorUser(ClientFactory &pFactory, VidStor &stor, QWidget *parent = 0);
	~VSCStorUser(){}

public slots:   
	void applyConfig();
	
public:
	Ui::VSCStorUser ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;

    
};

#endif // __VSC_USER_H__
