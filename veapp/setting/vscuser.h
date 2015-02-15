#ifndef __VSC_USER_H__
#define __VSC_USER_H__

#include <QWidget>
#include "ui_vscuser.h"
#include "utility.hpp"
#include <QTimer>
#include "factory.hpp"


class VSCUser : public QWidget
{
	Q_OBJECT

public:
	VSCUser(QWidget *parent = 0);
	~VSCUser(){}

public slots:   
	void applyConfig();
	
public:
	Ui::VSCUser ui;
	
private:
	VSCUserData m_Param;

    
};

#endif // __VSC_USER_H__
