#ifndef VSC_VERSION_H
#define VSC_VERSION_H

#include <QWidget>
#include "ui_vscversion.h"
#include "utility.hpp"
#include <QTimer>


class VSCVersion : public QWidget
{
    Q_OBJECT

public:
    VSCVersion(QWidget *parent = 0);
    ~VSCVersion(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCVersion ui;
	
private:

    
};

#endif // VSC_LICENSE_H
