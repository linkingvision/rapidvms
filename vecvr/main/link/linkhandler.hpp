#ifndef __LINK_HANDLER_H__
#define __LINK_HANDLER_H__
#include <string.h>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <google/protobuf/util/json_util.h>
#include "utility/type.hpp"

#include "config/linkproto.pb.h"
#include "config/linksystem.pb.h"
#include "config/vidconf.pb.h"
#include "simplecrypt.hpp"
#include "server/factory.hpp"
#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "XSDK/XMD5.h"
#include "server/eventserver.hpp"

#include "CivetServer.h"

using namespace Poco;


class LinkHandler
{
public:
	LinkHandler(Factory &pFactory, VEventServer &pEvent);
	~LinkHandler();
public:
	static bool CallChange(void* pParam, FactoryCameraChangeData data);
	bool CallChange1(FactoryCameraChangeData data);
	bool NotifyCamAdd(FactoryCameraChangeData data);
	bool NotifyCamDel(FactoryCameraChangeData data);
	bool NotifyCamOnline(FactoryCameraChangeData data);
	bool NotifyCamOffline(FactoryCameraChangeData data);
	bool NotifyCamRecOn(FactoryCameraChangeData data);
	bool NotifyCamRecOff(FactoryCameraChangeData data);

	/* Event */
	void EventHandler1(VEventData data);
	static void EventHandler(VEventData data, void* pParam);
	void SearchEventHandler1(VEventData data);
	static void SearchEventHandler(VEventData data, void* pParam);	
public:	
	bool ProcessMsg(std::string &strMsg, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessRegNotifyReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);

	/* Camera */
	bool ProcessCamListReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessCamReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessAddCamReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessDelCamReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessSetCamSchedReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessGetStreamListReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);

	/* Disk */
	bool ProcessDiskListReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessSysDiskListReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessAddDiskReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessDelDiskReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessUpdateDiskLimitReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);

	/* License & Version */
	bool ProcessGetLicReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessGetVerReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessConfLicReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);


	
	/* User */
	bool ProcessAddUserReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);


	/* Search */
	bool ProcessHasRecordReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessSearchRecordReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);


	/* PTZ command */
	bool ProcessPtzCmdReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);


	/* Cam Search & Event */
	bool ProcessRegEventReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessUnRegEventReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessHandleEventReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	bool ProcessEventSearchReq(Link::LinkCmd &req, CivetServer *server,
	                        struct mg_connection *conn);
	
	
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *server,
	                        struct mg_connection *conn);

	
							  
private:
	Factory &m_pFactory;
	astring m_seesionId;
	bool m_bLogin;
	bool m_bRegNotify;
	bool m_bRealEvent;
	bool m_bSearchEvent;
	VEventServer &m_pEvent;
	CivetServer *m_server;
	struct mg_connection *m_conn;
};


typedef std::map<void *, LinkHandler *> LinkHandlerMap;


#endif /* __LINK_HANDLER_H__ */
