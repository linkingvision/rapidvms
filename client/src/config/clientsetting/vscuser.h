#ifndef __VSC_USER_H__
#define __VSC_USER_H__

#include <QWidget>
#include "ui_vscuser.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCUser : public QWidget
{
	Q_OBJECT

public:
	VSCUser(ClientFactory &pFactory, QWidget *parent = 0);
	~VSCUser(){}

public slots:   
	void applyConfig();
	
public:
	Ui::VSCUser ui;
	
private:
	ClientFactory &m_pFactory;

    
};

#endif // __VSC_USER_H__
