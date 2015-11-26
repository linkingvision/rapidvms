#ifndef VSC_LICENSE_H
#define VSC_LICENSE_H

#include <QWidget>
#include "ui_vsclicense.h"
#include "utility.hpp"
#include <QTimer>


class VSCLicense : public QWidget
{
    Q_OBJECT

public:
    VSCLicense(QWidget *parent = 0);
    ~VSCLicense(){}
public:
	void UpdateUI();

public slots:   
  void applyConfig();
	
public:
    Ui::VSCLicense ui;
	
private:

    
};

#endif // VSC_LICENSE_H
