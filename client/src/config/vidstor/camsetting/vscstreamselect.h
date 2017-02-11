#ifndef __VSC_STREAM_SELECT_H__
#define __VSC_STREAM_SELECT_H__

#include <QWidget>
#include "ui_vscstreamselect.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCStreamSelect : public QWidget
{
	Q_OBJECT

public:
	VSCStreamSelect(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCStreamSelect(){}

public slots:   
	void applyConfig();
	void autoSelectClicked(bool checked);
	
public:
	Ui::VSCStreamSelect ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // __VSC_STREAM_SELECT_H__
