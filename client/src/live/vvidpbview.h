#ifndef __VVID_PB_VIEW_H__
#define __VVID_PB_VIEW_H__

#include <QWidget>
#include <QString>
#include "ui_vvidpbview.h"
#include "utility.hpp"
#include "debug.hpp"
#include "vvidpbwidget.h"
#include "vvidpbwall.h"
#include "QTabWidget"

class VVidPBView : public QWidget
{
    Q_OBJECT

public:
    VVidPBView(ClientFactory &pFactory, QWidget *parent, QTabWidget &pTabbed, QString strName);
    ~VVidPBView();
public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void createContentMenu();
    bool event(QEvent *e);
public:
    void SetupConnections();
	
public slots:
	void ShowFocusClicked(int nId);
	void ShowLayout1Clicked(int nId);

	void SetLayoutMode4x4()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/4x4.png")));
	}
	void SetLayoutMode3x3()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/3x3.png")));
	}
	void SetLayoutMode2x2()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/2x2.png")));
	}
	void SetLayoutMode6()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/6.png")));
	}
	void SetLayoutMode12p1()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/12+1.png")));
	}
	void SetLayoutMode1()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/1x1.png")));
	}
	void SetLayoutMode5x5()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/5x5.png")));
	}
	void SetLayoutMode6x6()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/6x6.png")));
	}
	void SetLayoutMode8x8()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/8x8.png")));
	}
	void SetLayoutMode1x3()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/1x3.png")));
	}
	void SetLayoutMode1p6()
	{
	    m_pTabbed.setTabIcon(m_pTabbed.indexOf(this), 
		QIcon(tr(":/view/resources/1p6.png")));
	}
	void DeviceEvent(int deviceId, int type)
	{
		m_pVideo->DeviceEvent(deviceId, (VscEventType)type);
	}

	void CameraDoubleClicked(std::string strStor, std::string strCam, std::string strCamName);
public:
	Ui::VVidPBView ui;
private:
	QWidget *m_pParent;
	QAction *m_pFloating;
	QAction *m_pUnFloating;
	BOOL m_bFloated;
	int m_currentFocus;
	VVidPBWall * m_pVideo;
	//VSCPlayControl * m_pPlayControl;
	BOOL m_bPlayControl;
	QTabWidget &m_pTabbed;

	/* Control Panel  */
	BOOL m_bControlEnable;
	QString m_strName;
	//VVidPBViewDataItem m_ViewItem;
	s32 m_lastHoverTime;
	QTimer *m_Timer;

	/* Video Tour */
	QTimer *m_TimerTour;
	s32 m_TourInterval;
	//VVidPBViewData m_pTourConf;
	s32 m_TourIdx;

	ClientFactory &m_pFactory;
    
};

#endif // VSCVIEW_H
