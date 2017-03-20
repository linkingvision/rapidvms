/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef VSCMAINWINDOWS_H
#define VSCMAINWINDOWS_H

#include <QtWidgets/QMainWindow>
#include "ui_vscmainwindows.h"
#include "client/clientfactory.hpp"
#include "live/vscvidlive.h"
#include "config/vscvidconfig.h"
#include "main/vscvidlist.h"
#include "searchpb/vscvidsearchpb.h"
#include "main/vscdashboard.h"
#include "main/vsceventconsole.h"

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
#ifdef __APPLE__
	void changeEvent(QEvent *event) 
	{ 
		if(event->type() == QEvent::WindowStateChange) 
		{ 
			//if(windowState() & Qt::WindowMaximized) 

			if(windowState() & Qt::WindowFullScreen)  
			{ 
				hide();
				this->setWindowState(Qt::WindowMaximized);
				show();
			}
		} 
		QWidget::changeEvent(event);  	
	} 
#endif

public slots:
	void MainCloseTab(int index);
	
public slots:
	void ShowVidLive();
	void ShowVidConf();
	void ShowVidPb();
	void ShowDashBoard();
	void ShowEventConsole();
	void about();
	void UserStatus();
	void SetFullScreen();
	void ShowAbout();

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
	QWidget *m_pMainView;
	VSCDashBoard *m_pDashBoard;
	VSCEventConsole *m_pEventConsole;
	QToolBar *m_pQToolBar;
	Qt::WindowFlags m_flag;

private:
	Ui::VSCMainWindowsClass ui;
	
private:
	ClientFactory &m_pFactory;
};

#endif // VSCMAINWINDOWS_H
