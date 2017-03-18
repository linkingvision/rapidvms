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
#ifndef __VSC_PB_EXPORT_H__
#define __VSC_PB_EXPORT_H__

#include "QDateTime"
#include "vtaskmgr.hpp"
#include "pbwrapper.hpp"

#include "utility.hpp"
#include "debug.hpp"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_command_queue.h"
#include "ffkit/av_muxer.h"
#include "client/storsyncinf.hpp"
#include "client/storstream.hpp"

#include "Poco/Path.h"

using namespace std;
using namespace cppkit;
using namespace ffkit;
using namespace Poco;


class VSCPbExportTask : public VTaskItem
{
	Q_OBJECT
public:
	VSCPbExportTask(astring strExportPath, astring strCamName,
					VidStor &pStor, astring strId, u32 nPlaytime, 
					u32 nEndTime);
	~VSCPbExportTask();
public:
	static void PBCallbackHandler(VideoFrame& frame,  void * pParam);
	void PBCallbackHandler1(VideoFrame& frame);
	
public:
	void run();
public:
	virtual int GetPercentage();
	virtual bool HasOpenResult();
	virtual bool OpenResult();
	virtual bool StopTask();
	virtual bool IsEol();
	virtual std::string GetTaskName();

private:

	StorStream *m_StorStream;
	VidStor m_pStor;
	astring m_strId;
	astring m_strExportPath;
	astring m_strCamName;
	u32 m_nPlaytime; 
	u32 m_nEndTime;
	QDateTime m_qStartTime;
	QDateTime m_qEndTime;
	u32 m_currentTime;
	BOOL m_isEol;
	astring m_fileName;

	codec_options m_options;
	av_muxer *m_pMuxer;
	InfoFrameI m_infoData;
	BOOL m_bGotInfoData;
	bool m_bQuit;

	XMutex m_cMutex;
};

#endif // __VSC_PB_EXPORT_H__
