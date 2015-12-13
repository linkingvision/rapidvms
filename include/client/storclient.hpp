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


/* Stor Client is the manager of the OAPI,  the thread will loop 
	for all the command from Stor */
class StorClient : public QThread
{
	Q_OBJECT
public:
	inline StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify);
	inline ~StorClient();
public:
	VidCameraList GetVidCameraList();
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
};

typedef StorClient* LPStorClient;


#include "storclientimpl.hpp"

#endif