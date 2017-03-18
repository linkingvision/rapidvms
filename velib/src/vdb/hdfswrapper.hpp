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
#ifndef __HDFS_WRAPPER_HPP__
#define __HDFS_WRAPPER_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "ffkit/av_muxer.h"
#include "cppkit/ck_dynamic_library.h"
#include "cppkit/ck_byte_ptr.h"


using namespace std;
using namespace cppkit;
using namespace ffkit;

#define HDFS_PATH_MAX 4096
#define HDFS_CONF_MAX 1024

typedef enum __HdfsCmdType
{
	HDFS_CMD_FILE = 1,
	HDFS_CMD_CONF,
	HDFS_CMD_LAST
}HdfsCmdType;

/* Record Wrapper for HDFS */
typedef struct __HdfsCmd
{
	HdfsCmdType cmd;
	s8 path[HDFS_PATH_MAX];
	s8 file[HDFS_PATH_MAX];
	s8 nameNode[HDFS_CONF_MAX];
	s8 port[HDFS_CONF_MAX];
	s8 user[HDFS_CONF_MAX];
	ck_memory   *pData;
}HdfsCmd;


/**
 * Some utility decls used in libhdfs.
 */
#ifndef O_RDONLY
#define O_RDONLY 1
#endif

#ifndef O_WRONLY 
#define O_WRONLY 2
#endif

#ifndef EINTERNAL
#define EINTERNAL 255 
#endif
struct hdfsBuilder;
typedef int32_t   tSize; /// size of data for read/write io ops 
typedef time_t    tTime; /// time type in seconds
typedef int64_t   tOffset;/// offset within the file
typedef uint16_t  tPort; /// port
typedef enum tObjectKind {
	kObjectKindFile = 'F',
	kObjectKindDirectory = 'D',
} tObjectKind;


/**
 * The C reflection of org.apache.org.hadoop.FileSystem .
 */
struct hdfs_internal;
typedef struct hdfs_internal* hdfsFS;

struct hdfsFile_internal;
typedef struct hdfsFile_internal* hdfsFile;

struct hadoopRzOptions;

struct hadoopRzBuffer;
/**
 * libhdfs function.
 */	

/** 
* hdfsConnectAsUser - Connect to a hdfs file system as a specific user
* Connect to the hdfs.
* @param nn   The NameNode.  See hdfsBuilderSetNameNode for details.
* @param port The port on which the server is listening.
* @param user the user name (this is hadoop domain user). Or NULL is equivelant to hhdfsConnect(host, port)
* @return Returns a handle to the filesystem or NULL on error.
* @deprecated Use hdfsBuilderConnect instead. 
*/
typedef hdfsFS (*hdfsConnectAsUserDelegate)(const char* nn, tPort port, const char *user);

/** 
 * hdfsConnect - Connect to a hdfs file system.
 * Connect to the hdfs.
 * @param nn   The NameNode.  See hdfsBuilderSetNameNode for details.
 * @param port The port on which the server is listening.
 * @return Returns a handle to the filesystem or NULL on error.
 * @deprecated Use hdfsBuilderConnect instead. 
 */
typedef hdfsFS (*hdfsConnectDelegate)(const char* nn, tPort port);

/** 
 * hdfsOpenFile - Open a hdfs file in given mode.
 * @param fs The configured filesystem handle.
 * @param path The full path to the file.
 * @param flags - an | of bits/fcntl.h file flags - supported flags are O_RDONLY, O_WRONLY (meaning create or overwrite i.e., implies O_TRUNCAT), 
 * O_WRONLY|O_APPEND. Other flags are generally ignored other than (O_RDWR || (O_EXCL & O_CREAT)) which return NULL and set errno equal ENOTSUP.
 * @param bufferSize Size of buffer for read/write - pass 0 if you want
 * to use the default configured values.
 * @param replication Block replication - pass 0 if you want to use
 * the default configured values.
 * @param blocksize Size of block - pass 0 if you want to use the
 * default configured values.
 * @return Returns the handle to the open file or NULL on error.
 */
typedef hdfsFile (*hdfsOpenFileDelegate)(hdfsFS fs, const char* path, int flags,
					  int bufferSize, short replication, tSize blocksize);

/** 
 * hdfsWrite - Write data into an open file.
 * @param fs The configured filesystem handle.
 * @param file The file handle.
 * @param buffer The data.
 * @param length The no. of bytes to write. 
 * @return Returns the number of bytes written, -1 on error.
 */
typedef tSize (*hdfsWriteDelegate)(hdfsFS fs, hdfsFile file, const void* buffer,
				tSize length);
/** 
 * hdfsWrite - Flush the data. 
 * @param fs The configured filesystem handle.
 * @param file The file handle.
 * @return Returns 0 on success, -1 on error. 
 */
typedef int (*hdfsFlushDelegate)(hdfsFS fs, hdfsFile file);
/** 
 * hdfsCloseFile - Close an open file. 
 * @param fs The configured filesystem handle.
 * @param file The file handle.
 * @return Returns 0 on success, -1 on error.  
 *         On error, errno will be set appropriately.
 *         If the hdfs file was valid, the memory associated with it will
 *         be freed at the end of this call, even if there was an I/O
 *         error.
 */
typedef int (*hdfsCloseFileDelegate)(hdfsFS fs, hdfsFile file);
/** 
 * hdfsDisconnect - Disconnect from the hdfs file system.
 * Disconnect from hdfs.
 * @param fs The configured filesystem handle.
 * @return Returns 0 on success, -1 on error.
 *         Even if there is an error, the resources associated with the
 *         hdfsFS will be freed.
 */
typedef int (*hdfsDisconnectDelegate)(hdfsFS fs);


class HdfslibDelegate
{
public:
	HdfslibDelegate();
	~HdfslibDelegate();
	
public:
	BOOL m_Init;
	hdfsConnectAsUserDelegate hdfsConnectAsUser;
	hdfsConnectDelegate hdfsConnect;
	hdfsOpenFileDelegate hdfsOpenFile;
	hdfsWriteDelegate hdfsWrite;
	hdfsFlushDelegate hdfsFlush;
	hdfsCloseFileDelegate hdfsCloseFile;
	hdfsDisconnectDelegate hdfsDisconnect;
private:
	ck_dynamic_library m_dynLib;
};


typedef command_queue<HdfsCmd> _HdfsCmdQueue;

class HdfsRecWrapper
{
public:
	HdfsRecWrapper(ck_string &pNameNode, ck_string &pPort, 
		ck_string &user);
	~HdfsRecWrapper();
public:
	BOOL Post(HdfsCmd &cmd);
	BOOL ProcessCmd(HdfsCmd &cmd);
	BOOL Connecting(); 
public:
	static void Run(void * pParam);
	void Run1();
private:
	_HdfsCmdQueue m_queue;
	tthread::thread *m_pThread;
	bool m_bExit;
	HdfslibDelegate m_HdfslibDelegate;
	hdfsFS m_hdfs;
	bool m_bConnected;
	ck_string m_pNameNode;
	ck_string m_pPort; 
	ck_string m_pUser;
	s32 m_inerval;/*time in sec */

};

#endif /* __HDFS_WRAPPER_HPP__ */
