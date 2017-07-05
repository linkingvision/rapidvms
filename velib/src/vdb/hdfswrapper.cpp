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
#include "hdfswrapper.hpp"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "vdbfile.hpp"

using namespace Poco;

HdfslibDelegate::HdfslibDelegate()
{
#ifdef WIN32
	ck_string strLib = "hdfs.dll";
#else
	ck_string strLib = "libhdfs.so";
#endif
	ck_string strhdfsConnectAsUserDelegate = "hdfsConnectAsUser";
	ck_string strhdfsConnectDelegate = "hdfsConnect";
	ck_string strhdfsOpenFileDelegate = "hdfsOpenFile";
	ck_string strhdfsWriteDelegate = "hdfsWrite";
	ck_string strhdfsFlushDelegate = "hdfsFlush";
	ck_string strhdfsCloseFileDelegate = "hdfsCloseFile";
	ck_string strhdfsDisconnectDelegate = "hdfsDisconnect";
#if 0
	try
	{
		//VDC_DEBUG( "%s Connect HDFS  lib %s !.\n",__FUNCTION__, strLib.c_str());
		m_dynLib.load(strLib);
		hdfsConnectAsUser = (hdfsConnectAsUserDelegate)m_dynLib.resolve_symbol(strhdfsConnectAsUserDelegate);
		hdfsConnect = (hdfsConnectDelegate)m_dynLib.resolve_symbol(strhdfsConnectDelegate);
		hdfsOpenFile = (hdfsOpenFileDelegate)m_dynLib.resolve_symbol(strhdfsOpenFileDelegate);
		hdfsWrite = (hdfsWriteDelegate)m_dynLib.resolve_symbol(strhdfsWriteDelegate);
		hdfsFlush = (hdfsFlushDelegate)m_dynLib.resolve_symbol(strhdfsFlushDelegate);
		hdfsCloseFile = (hdfsCloseFileDelegate)m_dynLib.resolve_symbol(strhdfsCloseFileDelegate);
		hdfsDisconnect = (hdfsDisconnectDelegate)m_dynLib.resolve_symbol(strhdfsDisconnectDelegate);

		m_Init = TRUE;
		VDC_DEBUG( "%s HDFS lib load successfully.\n",__FUNCTION__);
	} catch( ... )
	{
		m_Init = FALSE;
		//VDC_DEBUG( "%s HDFS lib load error \n",__FUNCTION__);
	}
#else
	m_Init = FALSE;
#endif

}

HdfslibDelegate::~HdfslibDelegate()
{
#if 0
	m_dynLib.unload();
#endif
}

HdfsRecWrapper::HdfsRecWrapper(ck_string &pNameNode, ck_string &pPort,
		ck_string &pUser)
:m_hdfs(NULL), m_bConnected(FALSE), m_pNameNode(pNameNode),
  m_pPort(pPort), m_pUser(pUser)
{
	m_queue.start();
	Connecting();
	this->m_pThread = new tthread::thread(HdfsRecWrapper::Run, (void *)this);
}
HdfsRecWrapper::~HdfsRecWrapper()
{
	m_bExit = TRUE;
	m_pThread->join();
	if (m_HdfslibDelegate.m_Init == TRUE && m_bConnected == TRUE)
	{
		m_HdfslibDelegate.hdfsDisconnect(m_hdfs);
	}
}

BOOL HdfsRecWrapper::Connecting()
{
	if (m_HdfslibDelegate.m_Init == TRUE)
	{
		m_hdfs = m_HdfslibDelegate.hdfsConnectAsUser(m_pNameNode.c_str(),
					m_pPort.to_uint16(10), m_pUser.c_str());
		if (m_hdfs == NULL)
		{
			//VDC_DEBUG( "%s Connect HDFS  failed !.\n",__FUNCTION__);
		}else
		{
			VDC_DEBUG( "%s Connect HDFS  successfully(%s,  %d).\n",__FUNCTION__,
				m_pNameNode.c_str(), m_pPort.to_uint16(10));
			m_bConnected = TRUE;
		}
	}else
	{
		//VDC_DEBUG( "%s HDFS lib load error \n",__FUNCTION__);
	}

	return TRUE;
}

BOOL HdfsRecWrapper::Post(HdfsCmd &cmd)
{
	int qSize = m_queue.size();
	if (qSize > 50)
	{
		VDC_DEBUG( "%s packet HDFS queue size is too long %d\n",__FUNCTION__, qSize);
		return true;
	}
	m_queue.post(cmd);
	return true;
}

BOOL HdfsRecWrapper::ProcessCmd(HdfsCmd &cmd)
{
	VDC_DEBUG( "%s HDFS cmd %d\n",__FUNCTION__, cmd.cmd);
	switch (cmd.cmd)
	{
		case HDFS_CMD_FILE:
		{
			VDC_DEBUG("%s HDFS cmd %s\n", __FUNCTION__, cmd.path);
			ck_byte_ptr ptr = cmd.pData->map();
#if 0
			{
				Poco::Path path(cmd.path, Path::PATH_UNIX);
				path.makeFile();
				astring strFile = cmd.file;
				Poco::File file1(path);
				file1.createDirectories();

				path.append(strFile);
				Poco::File file2(path);
				file2.createFile();

				VdbFile fileWrite(path.toString());
				s32 ret = fileWrite.Write((void *)(ptr.get_ptr()), ptr.length());
				VDC_DEBUG("%s Filesystem write size %d\n", __FUNCTION__, ret);
			}
#endif
			if (m_bConnected == TRUE)
			{
#if 1
				VDC_DEBUG("%s Write to  HDFS %s\n", __FUNCTION__, cmd.path);
				Poco::Path path(cmd.path, Path::PATH_UNIX);
				astring strFile = cmd.file;

				path.append(strFile);

				astring strPath = path.toString(Path::PATH_UNIX);
				VDC_DEBUG("%s HDFS file %s\n", __FUNCTION__, strPath.c_str());

				hdfsFile writeFile = m_HdfslibDelegate.hdfsOpenFile(m_hdfs,
											strPath.c_str(), O_WRONLY, 0, 0, 0);
				if (writeFile != NULL)
				{
					tSize ret = m_HdfslibDelegate.hdfsWrite(m_hdfs, writeFile,
									(void *)(ptr.get_ptr()), ptr.length());
					m_HdfslibDelegate.hdfsFlush(m_hdfs,writeFile);
					m_HdfslibDelegate.hdfsCloseFile(m_hdfs,writeFile);
					VDC_DEBUG("%s HDFS write size %d\n", __FUNCTION__, ret);

				}else
				{
					VDC_DEBUG("%s HDFS hdfsOpenFile Fail !\n", __FUNCTION__);
				}
#endif
			}else
			{
				VDC_DEBUG("%s HDFS no connect %s. Try to connecting\n",
							__FUNCTION__, cmd.path);
				Connecting();
			}
			delete cmd.pData;
			break;
		}
		case HDFS_CMD_CONF:
			break;
		default:
			break;
	}

	return true;

}

 void HdfsRecWrapper::Run(void * pParam)
{
	int dummy = errno;
	HdfsRecWrapper * pThread = (HdfsRecWrapper *)pParam;

	if (pThread)
	{
	    pThread->Run1();
	}
}
 void HdfsRecWrapper::Run1()
{
	HdfsCmd cmd;
	while (m_bExit != TRUE)
	{
		if (m_queue.size() > 0)
		{
			cmd = m_queue.pop();
			ProcessCmd(cmd);
			continue;
		}else
		{
			ve_sleep(1000);
		}
	}
 }
