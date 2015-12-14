#ifndef __VSC_STOR_CLIENT_H_
#define __VSC_STOR_CLIENT_H_
#include "config/confdb.hpp"
#include "config/sysdb.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "client/storcmn.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "oapi/oapic.hpp"
#include "oapi/oapis.hpp"

using namespace cppkit;
using namespace std;


/* Stor Client is the manager of the OAPI,  the thread will loop 
	for all the command from Stor */
class StorClient : public QThread
{
	Q_OBJECT
public:
	StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify);
	~StorClient();
public:
	VidCameraList GetVidCameraList();
	void UpdateVidCameraList(oapi::OAPICameraListRsp list);
	bool GetOnline(){return m_bOnline;}
public:
	/* Start Stop Refresh the Stor client Thread */
	bool StartStorClient();
	bool StopStorClient();
public:
	void run();
private:
	XMutex m_cMutex;
	VidStor m_stor;
	bool m_Quit;
	VidCameraList m_cCamList;
	StorFactoryNotifyInterface &m_pNotify;
	bool m_bOnline;
};

typedef StorClient* LPStorClient;


#include "storclientimpl.hpp"

#endif