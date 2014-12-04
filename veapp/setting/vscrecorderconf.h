#ifndef VSC_RECORDER_CONF_H
#define VSC_RECORDER_CONF_H

#include <QWidget>
#include "ui_vscrecorderconf.h"
#include "utility.hpp"
#include <QTimer>
#include "vscrecorderone.h"
#include "vscrecorder.h"


class VSCRecorderConf : public QWidget
{
    Q_OBJECT

public:
    VSCRecorderConf(RecorderMap &pMap, QWidget *parent = 0);
    ~VSCRecorderConf(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCRecorderConf ui;
	
private:

    
};

#endif // VSC_RECORDER_CONF_H
