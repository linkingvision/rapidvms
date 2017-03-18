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
#ifndef __VIDEO_EMAP_CAM_H__
#define __VIDEO_EMAP_CAM_H__

#include <QWidget>
#include <QMenu>
#include <QTimer>
#include "utility.hpp"
#include "client/clientfactory.hpp"

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscemapcam.h"

using  namespace tthread;


class VSCEmapCam : public QWidget
{
	Q_OBJECT

public:
	VSCEmapCam(VidEmapCamera &cCam, ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCEmapCam();
	void contextMenuEvent(QContextMenuEvent* e);
	void mousePressEvent(QMouseEvent *event);
	void DragCommon();
	void OnDeviceEvent(int deviceId, int type);
	
public slots:
	void PlayVideo();
	void PlaybackVideo();
	void EmapDelete();
	void DragEmap();
	void showAlarm();
	
signals:
	void signalFocus();
	void EmapCamDel(std::string strCam);

public:
	Ui::VSCEmapCam ui;
private:
	bool m_isRunning;
	ClientFactory &m_pFactory;
	VidEmapCamera m_cCam;
	QAction *m_pDelete;
	QAction *m_pPlay;
	QAction *m_pPlayback;
	QTimer *m_TimerAlarm;
	bool m_alarm;
	int m_alarmCnt;
};

#endif // __VIDEO_EMAP_CAM_H__
