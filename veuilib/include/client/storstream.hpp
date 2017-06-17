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
#ifndef __VSC_STOR_STREAM_H__
#define __VSC_STOR_STREAM_H__
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "server/camera.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapi/oapic.hpp"
#include "oapi/oapis.hpp"

using namespace cppkit;
using namespace std;

class VE_LIBRARY_API StorStream: public QThread
{
	Q_OBJECT
public:
	StorStream(VidStor &stor, astring strId, unsigned int nStream, 
		bool bPlayback = false, u32 nPlaytime = 0, bool bHWAccel = false);
	~StorStream();
#ifdef WIN32
	bool AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_D3D);
#else
	bool AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_SDL);
#endif
	bool EnableMot(HWND hWnd, bool bEnable, astring strConf);
	bool UpdateWidget(HWND hWnd, int w, int h);
	bool DetachWidget(HWND hWnd);
	bool GetStreamInfo(VideoStreamInfo &pInfo);

	bool RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	bool UnRegDataCallback();

public:
	bool PausePlayback();
	bool ResumePlayback();
	bool SeekPlayback(u32 nPlaytime);
	bool StopPlayback();
signals:
	void SignalPlayTime(unsigned int nTime);

public:
	bool StartStorStream();
	bool StopStorStream();
public:
	void run();
	
private:
	XMutex m_cMutex;
	
private:
	bool m_Quit;
	VPlay m_play;
	astring m_strId;
	unsigned int m_nStream;
	VidStor m_stor;
	unsigned int m_nLastTime;
	
	CameraDataCallbackFunctionPtr m_pCallback; 
	void * m_pParam;

	XRef<XSocket> m_pSocket;
	bool m_bOnline;
private:
	bool m_bPlayback;
	u32 m_nPlaytime;
	bool m_bPbPause;
};

typedef StorStream* LPStorStream;

#endif