#ifndef __VSC_VID_INF_H__
#define __VSC_VID_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"

class VSCVidInf : public QObject
{
    Q_OBJECT
public:
    VSCVidInf(ClientFactory &pFactory, QMainWindow *parent);
    ~VSCVidInf();
	
public slots:
	void SlotShow(){return VidShow();}
	void SlotHide(){return VidHide();}
	
public:
	virtual void VidShow(){}
	virtual void VidHide(){}

protected:
	ClientFactory &m_pFactory;
	QMainWindow *m_parent;
};
	