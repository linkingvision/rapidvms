#ifndef _HDD_H_
#define _HDD_H_

#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"
#include <QtCore/QStorageInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>

#ifdef WIN32
#include "winbase.h"
#endif

using namespace UtilityLib;

/* This is same with QStorageInfo define */
enum HddDriveType {
	HddUnknownDrive = 0,
	HddInternalDrive,
	HddRemovableDrive,
	HddRemoteDrive,
	HddCdromDrive,
	HddRamDrive
};

#ifdef WIN32
inline HddDriveType HddGetDriveType(const QString &drive)
{
    UINT type = GetDriveType((WCHAR *)drive.utf16());
    switch (type) {
    case DRIVE_REMOVABLE:
        return HddRemovableDrive;
    case DRIVE_FIXED:
        return HddInternalDrive;
    case DRIVE_REMOTE:
        return HddRemoteDrive;
    case DRIVE_CDROM:
        return HddCdromDrive;
    case DRIVE_RAMDISK:
        return HddRamDrive;
    case DRIVE_UNKNOWN:
    case DRIVE_NO_ROOT_DIR:
    	default:
        return HddUnknownDrive;
    };
}
#else /* Linux */
inline HddDriveType HddGetDriveType(const QString &drive)
{
	return HddInternalDrive;
}

#endif 


#endif /* _HDD_H_ */
