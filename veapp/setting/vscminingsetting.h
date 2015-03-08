#ifndef __VSC_MINING_SETTING_H__
#define __VSC_MINING_SETTING_H__

#include <QWidget>
#include "ui_vscminingsetting.h"
#include "utility.hpp"
#include <QTimer>


class VSCMiningSetting : public QWidget
{
    Q_OBJECT

public:
    VSCMiningSetting(QWidget *parent = 0);
    ~VSCMiningSetting(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCMiningSetting ui;
	
private:
	BOOL m_bMining;

    
};

#endif // __VSC_VIDEO_SETTING_H__
