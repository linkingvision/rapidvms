#ifndef __VSC_USER_STATUS_H__
#define __VSC_USER_STATUS_H__

#include <QDialog>
#include "ui_vscuserstatus.h"
#include "utility.hpp"
#include <QTimer>

class VSCUserStatus : public QDialog
{
    Q_OBJECT

public:
    VSCUserStatus(QWidget *parent = 0);
    ~VSCUserStatus(){}
public:

public slots: 
	void SaveClicked();
	void CancelClicked();

public:
	Ui::VSCUserStatus ui;
	
};

#endif // __VSC_USER_STATUS_H__
