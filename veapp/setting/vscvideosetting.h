#ifndef __VSC_VIDEO_SETTING_H__
#define __VSC_VIDEO_SETTING_H__

#include <QWidget>
#include "ui_vscvideosetting.h"
#include "utility.hpp"
#include <QTimer>


class VSCVideoSetting : public QWidget
{
    Q_OBJECT

public:
    VSCVideoSetting(QWidget *parent = 0);
    ~VSCVideoSetting(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCVideoSetting ui;
	
private:
	BOOL m_bHWAccel;

    
};

#endif // __VSC_VIDEO_SETTING_H__
