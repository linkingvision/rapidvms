#ifndef VSC_LICENSE_H
#define VSC_LICENSE_H

#include <QWidget>
#include "ui_vsclicense.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCLicense : public QWidget
{
    Q_OBJECT

public:
    VSCLicense(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCLicense(){}
public:
	void UpdateUI();

public slots:   
  void applyConfig();
	
public:
    Ui::VSCLicense ui;
	
private:
	ClientFactory &m_pFactory;

    
};

#endif // VSC_LICENSE_H
