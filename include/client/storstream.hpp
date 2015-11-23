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

class StorStream: public QThread
{
	Q_OBJECT
public:
	inline StorStream(VidStor &stor, astring strStream);
	inline ~StorStream();
}

typedef StorStream* LPStorStream;


#include "storstreamimpl.hpp"

#endif