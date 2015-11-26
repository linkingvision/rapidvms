#ifndef __VSC_HDFS_RECORD_H__
#define __VSC_HDFS_RECORD_H__

#include <QWidget>
#include "ui_vschdfsrecord.h"
#include "utility.hpp"
#include <QTimer>
#include <QLineEdit>
#include "factory.hpp"


class VSCHdfsRecord : public QWidget
{
    Q_OBJECT

public:
    VSCHdfsRecord(QWidget *parent = 0);
    ~VSCHdfsRecord(){}
public:
	void updateParamValue(QLineEdit *ld, s8 * pParam)
	{
	    if (pParam && ld)
	    {
	        strcpy(pParam, ld->text().toStdString().c_str());
	    }
	}
	void setupDefaultValue();
public slots:   
	void applyConfig();
	
public:
	Ui::VSCHdfsRecord ui;
	
private:
	VSCHdfsRecordData m_Param;
    
};

#endif // __VSC_USER_H__
