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
    STOR_FACTORY_CAMERA_REC_ON,
    STOR_FACTORY_CAMERA_REC_OFF,
    STOR_FACTORY_LAST
} StorFactoryChangeType;


class StorFactoryChangeData
{
public:
	StorFactoryChangeType type;
	VidCameraId cId;
	VidCamera cCam;
};

class StorFactoryNotifyInterface
{
public:
virtual bool CallChange(StorFactoryChangeData data){return FALSE;}
virtual bool OnEvent(VidEvent &pEvent,  VidStor &pStor){return FALSE;}
};


#endif /* __VSC_STOR_CMN_H_ */