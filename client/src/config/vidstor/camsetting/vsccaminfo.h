#ifndef VSC_CAM_INFO_H
#define VSC_CAM_INFO_H

#include <QWidget>
#include "ui_vsccaminfo.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCCamInfo : public QWidget
{
    Q_OBJECT

public:
    VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
    ~VSCCamInfo(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCCamInfo ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // VSC_LICENSE_H
