#ifndef __VSC_OAPI_H__
#define __VSC_OAPI_H__

#include <QWidget>
#include "ui_vscoapi.h"
#include "utility.hpp"
#include <QTimer>


class VSCOAPI : public QWidget
{
    Q_OBJECT

public:
    VSCOAPI(QWidget *parent = 0);
    ~VSCOAPI(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCOAPI ui;
	
private:
	u16 m_port;
	u16 m_HTTPPort;
	u16 m_HTTPSSLPort;
	u16 m_HLSPort;

    
};

#endif // __VSC_OAPI_H__
