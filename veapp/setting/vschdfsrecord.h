#ifndef __VSC_HDFS_RECORD_H__
#define __VSC_HDFS_RECORD_H__

#include <QWidget>
#include "ui_vschdfsrecord.h"
#include "utility.hpp"
#include <QTimer>


class VSCHdfsRecord : public QWidget
{
    Q_OBJECT

public:
    VSCHdfsRecord(QWidget *parent = 0);
    ~VSCHdfsRecord(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCHdfsRecord ui;
	
private:

    
};

#endif // __VSC_USER_H__
