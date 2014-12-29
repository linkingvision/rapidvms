#ifndef __VSC_RTSP_RTP_H__
#define __VSC_RTSP_RTP_H__

#include <QWidget>
#include "ui_vscrtsprtp.h"
#include "utility.hpp"
#include <QTimer>


class VSCRtspRtp : public QWidget
{
    Q_OBJECT

public:
    VSCRtspRtp(QWidget *parent = 0);
    ~VSCRtspRtp(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCRtspRtp ui;
	
private:

    
};

#endif // __VSC_USER_H__
