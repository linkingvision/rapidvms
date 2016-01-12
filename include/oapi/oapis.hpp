//------------------------------------------------------------------------------
// File: oapis.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_H_
#define __VSC_OAPIS_H_
#include "oapicmn.hpp"
#include "server/factory.hpp"
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

using namespace VidConf;

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
};

typedef std::map<astring, DeviceSearcher *> DeviceSearcherMap;

/* a internal thread to process all the request of XSocket */
class OAPIServer:public QObject 
{
	Q_OBJECT
public:
	inline OAPIServer(XRef<XSocket> pSocket, Factory &pFactory);
	inline ~OAPIServer();
public:
	inline BOOL Process(OAPIHeader &header);

public:
	inline BOOL ProcessGetDevice(s32 len);	
	inline BOOL ProcessGetLic(s32 len);
	inline BOOL ProcessConfLic(s32 len);
	inline BOOL ProcessConfAdmin(s32 len);
	inline BOOL ProcessStartLive(s32 len);
	inline BOOL ProcessStopLive(s32 len);
	inline BOOL ProcessGetDisk(s32 len);
	inline BOOL ProcessGetSysDisk(s32 len);
	inline BOOL ProcessLogin(s32 len);
	inline bool ProcessAddCam(s32 len);
	inline bool ProcessDeleteCam(s32 len);
	inline bool ProcessRegNotify(s32 len);
	inline bool ProcessAddDisk(s32 len);
	inline bool ProcessDeleteDisk(s32 len);
	inline bool ProcessCamSearchStart(s32 len);
	inline bool ProcessCamSearchStop(s32 len);	
	inline bool NotifyCamAdd(FactoryCameraChangeData data);
	inline bool NotifyCamDel(FactoryCameraChangeData data);
	inline bool NotifyCamOnline(FactoryCameraChangeData data);
	inline bool NotifyCamOffline(FactoryCameraChangeData data);
	inline bool NotifyCamRecOn(FactoryCameraChangeData data);
	inline bool NotifyCamRecOff(FactoryCameraChangeData data);

public slots:
	void SlotSearchReceiveData(const QHash<QString, QString> &data);

public:
	inline static bool CallChange(void* pParam, FactoryCameraChangeData data);
	inline bool CallChange1(FactoryCameraChangeData data);
public:
	inline void DataHandler1(VideoFrame& frame);
	inline static void DataHandler(VideoFrame& frame, void * pParam);	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
	inline bool SendCmnRetRsp(OAPICmd nCmd, bool bRet);
	inline bool ClearCamSearcherMap();
	inline bool CamSearcherStart();

private:
	fast_mutex m_Lock;
	XRef<XSocket> m_pSocket;
	Factory &m_pFactory;
	astring m_strLiveviewId;
	unsigned int m_nStream;
	bool m_bStreaming;
	bool m_bRegNotify;
	int m_cnt;
	BOOL m_bLogin;
	astring m_seesionId;
	DeviceSearcherMap m_CamSearcherMap;
};

#include "oapisimpl.hpp"
#endif