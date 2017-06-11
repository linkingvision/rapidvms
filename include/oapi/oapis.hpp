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
#ifndef __VSC_OAPIS_H_
#define __VSC_OAPIS_H_
#include "oapicmn.hpp"
#include "server/factory.hpp"
#include "server/eventserver.hpp"
#include "oapiproto.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "Poco/UUIDGenerator.h"
#include "XSDK/XMD5.h"

#include "config/vidconf.pb.h"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "devicesearcher.h"
#include <QHostAddress>
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "pbwrapper.hpp"
#include <QTimer>

using namespace XSDK;
using namespace std;
using namespace oapi;
using namespace Poco;
using namespace ONVIF;

class OAPIConverter
{
public:
	inline static bool Converter(VidCamera &from, oapi::OAPICamera &to);
	inline static bool Converter(oapi::OAPICamera &from, VidCamera &toVid);
	inline static bool Converter(oapi::OAPIDisk &from, VidDisk &to);
	inline static bool Converter(VidDisk &from, oapi::OAPIDisk &to);
	inline static bool Converter(oapi::OAPIStreamListRsp &from, VidStreamList &to);
	inline static bool Converter(VidStreamList &from, oapi::OAPIStreamListRsp &to);
};

typedef std::map<astring, DeviceSearcher *> DeviceSearcherMap;

class OAPICamSearchInterface
{
public:
	virtual bool NewCam(astring strIP, astring strPort, 
			astring strModel, astring strONVIFAddr){return false;}
};

class OAPIPlaybackInterface
{
public:
	virtual bool NewFrame(VideoFrame& frame){return false;}
};

class OAPIServerCamSearch: public QThread
{
	Q_OBJECT
public:
	inline OAPIServerCamSearch(OAPICamSearchInterface &pCamInf, 
		Factory &pFactory);
	inline ~OAPIServerCamSearch();
	
public:
	inline void run();
	inline bool ClearCamSearcherMap();
	inline bool CamSearcherStart();
	inline bool QuitSearch();
public slots:
	inline void SlotSearchReceiveData(const QHash<QString, QString> &data);
	inline void SlotTimeout();


private:
	Factory &m_pFactory;
	OAPICamSearchInterface &m_pCamInf;
	DeviceSearcherMap m_CamSearcherMap;
	XMutex m_cMutex;
	bool m_bQuit;
	QTimer *m_Timer;
};

class OAPIServerPlayback: public QThread
{
	Q_OBJECT
public:
	inline OAPIServerPlayback(OAPIPlaybackInterface &pPbInf, 
		Factory &pFactory, astring strId, u32 nPlaytime);
	inline ~OAPIServerPlayback();

public:
	inline void DataHandler1(VideoFrame& frame)
	{
		XGuard guard(m_cMutex);
		if (m_bQuit == false)
		{
			m_pPbInf.NewFrame(frame);
		}
	}

	inline static void DataHandler(VideoFrame& frame, void * pParam)
	{
	    OAPIServerPlayback *pOapi = static_cast<OAPIServerPlayback *> (pParam);
	    
	    return pOapi->DataHandler1(frame);
	}
public:
	inline void run();
	inline bool SeekToTime(u32 seekTime);
	inline bool StartPlay();
	inline bool PausePlay();
	inline bool QuitPlay();/* End of play */

private:
	Factory &m_pFactory;
	OAPIPlaybackInterface &m_pPbInf;
	astring m_strId;
	u32 m_nPlaytime;
	PlaybackWrapper m_pbWrapper;
	XMutex m_cMutex;
	bool m_bQuit;
};

/* a internal thread to process all the request of XSocket */
class OAPIServer:public QObject ,public OAPICamSearchInterface, 
				public OAPIPlaybackInterface
{
	Q_OBJECT
public:
	inline OAPIServer(XRef<XSocket> pSocket, Factory &pFactory, VEventServer &pEvent);
	inline ~OAPIServer();
public:
	inline BOOL Process(OAPIHeader &header);

public:
	inline BOOL ProcessGetDevice(s32 len);	
	inline BOOL ProcessGetLic(s32 len);
	inline BOOL ProcessGetVer(s32 len);
	inline BOOL ProcessConfLic(s32 len);
	inline BOOL ProcessConfAdmin(s32 len);
	inline BOOL ProcessStartLive(s32 len);
	inline BOOL ProcessStopLive(s32 len);
	inline BOOL ProcessGetDisk(s32 len);
	inline BOOL ProcessGetCam(s32 len);
	inline BOOL ProcessGetSysDisk(s32 len);
	inline BOOL ProcessLogin(s32 len);
	inline bool ProcessAddCam(s32 len);
	inline bool ProcessSetCamSched(s32 len);
	inline bool ProcessDeleteCam(s32 len);
	inline bool ProcessPtzCmd(s32 len);
	inline bool ProcessRegNotify(s32 len);
	inline bool ProcessAddDisk(s32 len);
	inline bool ProcessDeleteDisk(s32 len);
	inline bool ProcessUpdateDiskLimit(s32 len);
	inline bool ProcessCamSearchStart(s32 len);
	inline bool ProcessCamSearchStop(s32 len);
	inline bool ProcessSearchRec(s32 len);
	inline bool ProcessSearchHasRec(s32 len);

	inline bool ProcessGetStreamList(s32 len);	

	/* Playback */
	inline bool ProcessStartPlayback(s32 len);
	inline bool ProcessPausePlayback(s32 len);
	inline bool ProcessResumePlayback(s32 len);
	inline bool ProcessSeekPlayback(s32 len);
	inline bool ProcessStopPlayback(s32 len);
	inline bool ProcessSearchEvent(s32 len);
	inline bool ProcessRegEvent(s32 len);
	inline bool ProcessUnRegEvent(s32 len);
	inline bool ProcessHandleEvent(s32 len);
	
	inline bool NotifyCamAdd(FactoryCameraChangeData data);
	inline bool NotifyCamDel(FactoryCameraChangeData data);
	inline bool NotifyCamOnline(FactoryCameraChangeData data);
	inline bool NotifyCamOffline(FactoryCameraChangeData data);
	inline bool NotifyCamRecOn(FactoryCameraChangeData data);
	inline bool NotifyCamRecOff(FactoryCameraChangeData data);

public:
	inline static bool CallChange(void* pParam, FactoryCameraChangeData data);
	inline bool CallChange1(FactoryCameraChangeData data);
public:
	inline void DataHandler1(VideoFrame& frame);
	inline static void DataHandler(VideoFrame& frame, void * pParam);
public:
	inline void EventHandler1(VEventData data);
	inline static void EventHandler(VEventData data, void* pParam);
	inline void SearchEventHandler1(VEventData data);
	inline static void SearchEventHandler(VEventData data, void* pParam);	
	
public:
	inline bool SendCmnRetRsp(OAPICmd nCmd, bool bRet);

public:
	inline virtual bool NewFrame(VideoFrame& frame);
	inline virtual bool NewCam(astring strIP, astring strPort, 
			astring strModel, astring strONVIFAddr);

private:
	XMutex m_cMutex;
	XRef<XSocket> m_pSocket;
	Factory &m_pFactory;
	VEventServer &m_pEvent;
	astring m_strLiveviewId;
	unsigned int m_nStream;
	bool m_bStreaming;
	bool m_bRealEvent;
	bool m_bSearchEvent;
	bool m_bRegNotify;
	int m_cnt;
	BOOL m_bLogin;
	astring m_seesionId;

private:
	OAPIServerPlayback *m_pPlayback;
	OAPIServerCamSearch *m_pCamSearch;
	
};

#include "oapisimpl.hpp"
#endif
