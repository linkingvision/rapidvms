#ifndef VSC_RECORDER_ONE_H
#define VSC_RECORDER_ONE_H

#include <QWidget>
#include "ui_vscrecorderone.h"
#include "utility.hpp"
#include <QTimer>
#include "vscrecorder.h"


class VSCRecorderOne : public QWidget
{
    Q_OBJECT

public:
    VSCRecorderOne(VSCRecorder &recorder, QWidget *parent = 0);
    ~VSCRecorderOne(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCRecorderOne ui;
	
private:
	VSCRecorder m_Recorder;

    
};

#endif // VSC_RECORDER_ONE_H
