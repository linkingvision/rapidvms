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
}

typedef StorClient* LPStorClient;


#include "storclientimpl.hpp"

#endif