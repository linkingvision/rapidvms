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
#include "recordsession.hpp"

RecordSession::RecordSession(astring deviceId, astring &strBlockPath, u32 startTime, s64 recordId)
: m_DeviceId(deviceId),m_StrBlockPath(strBlockPath), 
m_MediaFile(strBlockPath, startTime, MEDIA_SESSION_RECORD, MF_FILE_MAX_LENGTH),
m_RecordId(recordId)
{

}
RecordSession::~RecordSession()
{

}

MFStatus RecordSession::PushAFrame(VideoFrame *pFrame)
{
	return m_MediaFile.PushAFrame(pFrame);
}
u32 RecordSession::GetEndTime()
{
	return m_MediaFile.GetEndTime();
}
s64 RecordSession::GetRecordId()
{
	return m_RecordId;
}
astring & RecordSession::GetBlockPath()
{
	return m_StrBlockPath;
}
