#ifndef __VSC_USER_H__
#define __VSC_USER_H__

#include <QWidget>
#include "ui_vscuser.h"
#include "utility.hpp"
#include <QTimer>


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

    
};

#endif // __VSC_USER_H__
