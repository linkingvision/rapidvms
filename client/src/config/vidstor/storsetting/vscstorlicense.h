#ifndef VSC_STOR_LICENSE_H
#define VSC_STOR_LICENSE_H

#include <QWidget>
#include "ui_vscstorlicense.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCStorLicense : public QWidget
{
    Q_OBJECT

public:
    VSCStorLicense(ClientFactory &pFactory, VidStor &stor, QWidget *parent = 0);
    ~VSCStorLicense(){}
public:
	void UpdateUI();

public slots:   
  void SlotImportLic();
  void SlotDefaultLic();
	
public:
    Ui::VSCStorLicense ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;

    
};

#endif // VSC_LICENSE_H
