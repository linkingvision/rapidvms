#ifndef VSC_VERSION_H
#define VSC_VERSION_H

#include <QWidget>
#include "ui_vscversion.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCVersion : public QWidget
{
    Q_OBJECT

public:
    VSCVersion(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVersion(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCVersion ui;
	
private:
	ClientFactory &m_pFactory;

    
};

#endif // VSC_LICENSE_H
