#ifndef __VSC_STOR_CMN_H_
#define __VSC_STOR_CMN_H_
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

typedef enum
{
    STOR_FACTORY_STOR_ADD = 1,
    STOR_FACTORY_STOR_DEL,
    STOR_FACTORY_STOR_ONLINE,
    STOR_FACTORY_STOR_OFFLINE,
    STOR_FACTORY_CAMERA_ADD,
    STOR_FACTORY_CAMERA_DEL,
    STOR_FACTORY_CAMERA_ONLINE,
    STOR_FACTORY_CAMERA_OFFLINE,
    STOR_FACTORY_CAMERA_RECORDING_ON,
    STOR_FACTORY_CAMERA_RECORDING_OFF,
    STOR_FACTORY_LAST
} StorFactoryChangeType;


class StorFactoryChangeData
{
public:
	StorFactoryChangeType type;
	VidCameraId cId;
};

typedef bool (*StorFactoryChangeNotify)(void* pParam, 
		StorFactoryChangeData data);
		
typedef std::map<void *, StorFactoryChangeNotify> StorChangeNofityMap;

class StorFactoryNotifyInterface
{
public:
virtual bool CallChange(StorFactoryChangeData data){return FALSE;}
};


#endif /* __VSC_STOR_CMN_H_ */