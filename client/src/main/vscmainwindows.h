
#ifndef VSCMAINWINDOWS_H
#define VSCMAINWINDOWS_H

#include <QtWidgets/QMainWindow>
#include "ui_vscmainwindows.h"
#include "client/clientfactory.hpp"
#include "live/vscvidlive.h"
#include "config/vscvidconfig.h"
#include "main/vscvidlist.h"
#include "playback/vscvidpb.h"

typedef enum
{
    VSC_VID_IDX_LIVE = 1,
    VSC_VID_IDX_CONF,
    VSC_VID_IDX_PB,
    VSC_VID_IDX_LAST
} VSCVidIdx;

class VSCToolBar;
class VSCMainWindows : public QMainWindow
{
	Q_OBJECT
public:
	VSCMainWindows(ClientFactory &pFactory, QWidget *parent = 0);
	~VSCMainWindows();
	
public:
	void SetupToolBar();
	void ViewHideFocus();
	void closeEvent(QCloseEvent *event); 


public slots:
	void ShowVidLive();
	void ShowVidConf();
	void ShowVidPb();
	void about();
	void UserStatus();
	void SetFullScreen();

	/* Show Login */
	void ShowLogin();
	void ExitOpenCVR();
private:
	QAction *aboutAct;
private:
	VSCToolBar * m_pToolBar;
	VSCVidList * m_pVidList;
	QDockWidget *m_pDockDevicelist;
	VSCVidInf *m_pVidLive;
	VSCVidInf *m_pVidConf;
	VSCVidInf *m_pVidPb;
	VSCVidIdx m_VidIdx;

	QTabWidget * m_pMainArea;

private:
	Ui::VSCMainWindowsClass ui;
	
private:
	ClientFactory &m_pFactory;
};

#endif // VSCMAINWINDOWS_H
