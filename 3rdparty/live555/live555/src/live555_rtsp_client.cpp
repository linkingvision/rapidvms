/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 2014-2018 veyesys.com. All rights reserved.
// Live555 RTSP client wrapper

#include "live555_rtsp_client.hpp"
#include "GroupsockHelper.hh"


Live555Environment::Live555Environment() : BasicUsageEnvironment(*BasicTaskScheduler::createNew()), m_stop(0)
{
}

Live555Environment::~Live555Environment()
{
	TaskScheduler* scheduler = &this->taskScheduler();
	this->reclaim();
	delete scheduler;	
}

Live555RTSPConnection::SessionSink::SessionSink(UsageEnvironment& env, Callback* callback, 
	Live555CodecType codec, char const* spropparametersets) 
	: MediaSink(env)
	, m_bufferSize(0)
	, m_buffer(NULL)
	, m_callback(callback) 
	, m_markerSize(0)
	, m_codecType(codec)
	, m_bFirstFrame(true)
{
	if (spropparametersets)
	{
		m_fmtp_spropparametersets = spropparametersets;
		printf("fmtp spropparametersets %s\n", m_fmtp_spropparametersets.c_str());
	}
	allocate(1024*1024);
}

Live555RTSPConnection::SessionSink::~SessionSink()
{
	delete [] m_buffer;
}

void Live555RTSPConnection::SessionSink::allocate(int bufferSize)
{
	m_bufferSize = bufferSize;
	m_buffer = new u_int8_t[m_bufferSize];
	if (m_callback)
	{
		m_markerSize = m_callback->onNewBuffer(m_buffer, m_bufferSize);
		LIVE555_LOG << "markerSize:" << m_markerSize;
	}
}


void Live555RTSPConnection::SessionSink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, 
				struct timeval presentationTime, unsigned durationInMicroseconds)
{
	//LIVE555_LOG << "NOTIFY size:" << frameSize;
	if (numTruncatedBytes != 0)
	{
		delete [] m_buffer;
		LIVE555_LOG << "buffer too small " << m_bufferSize << " allocate bigger one\n";
		allocate(m_bufferSize*2);
	}
	else if (m_callback)
	{
		/* first frame set the  m_strSpropparametersets TODO update for H265*/
		if (m_bFirstFrame == true)
		{
			m_callback->onParameter("sprop-parameter-sets", m_fmtp_spropparametersets);
			m_bFirstFrame = false;
		}
		//printf("durationInMicroseconds %d\n", durationInMicroseconds);
		//gettimeofday(&presentationTime, NULL);
		if (!m_callback->onData(m_buffer, frameSize+m_markerSize, 
						presentationTime.tv_sec, presentationTime.tv_usec/1000, m_codecType))
		{
			LIVE555_LOG << "NOTIFY failed\n" ;
		}
	}
	this->continuePlaying();
}

Boolean Live555RTSPConnection::SessionSink::continuePlaying()
{
	Boolean ret = False;
	if (source() != NULL)
	{
		source()->getNextFrame(m_buffer+m_markerSize, m_bufferSize-m_markerSize,
				afterGettingFrame, this,
				onSourceClosure, this);
		ret = True;
	}
	return ret;	
}


		
Live555RTSPConnection::Live555RTSPConnection(UsageEnvironment& env, Callback* callback, const std::string & rtspURL, 
	std::string strUser, std::string strPasswd, bool bTCP, int verbosityLevel) 
				: RTSPClient(env, rtspURL.c_str(), verbosityLevel, NULL, 0
#if LIVEMEDIA_LIBRARY_VERSION_INT > 1371168000 
					,-1
#endif
					)
				, m_session(NULL)
				, m_subSessionIter(NULL)
				, m_callback(callback)
				, m_Authenticator(strUser.c_str(), strPasswd.c_str())
				, m_bTCP(bTCP)
{
	// initiate connection process
	this->sendNextCommand();
}

Live555RTSPConnection::~Live555RTSPConnection()
{
	delete m_subSessionIter;
	Medium::close(m_session);
}
		
void Live555RTSPConnection::sendNextCommand() 
{
	if (m_subSessionIter == NULL)
	{
		// no SDP, send DESCRIBE
		this->sendDescribeCommand(continueAfterDESCRIBE, &m_Authenticator); 
	}
	else
	{
		m_subSession = m_subSessionIter->next();
		if (m_subSession != NULL) 
		{
			// still subsession to SETUP
			if (!m_subSession->initiate()) 
			{
				LIVE555_LOG << "Failed to initiate " << m_subSession->mediumName() << "/" << m_subSession->codecName() << " subsession: " << envir().getResultMsg();
				this->sendNextCommand();
			} 
			else 
			{					
				LIVE555_LOG << "Initiated " << m_subSession->mediumName() << "/" << m_subSession->codecName() << " subsession";
			}

			/* Change the multicast here */
			this->sendSetupCommand(*m_subSession, continueAfterSETUP, False, m_bTCP, False, &m_Authenticator);
		}
		else
		{
			// no more subsession to SETUP, send PLAY
			this->sendPlayCommand(*m_session, continueAfterPLAY, (double)0, (double)-1, (float)0, &m_Authenticator);
		}
	}
}

void Live555RTSPConnection::continueAfterDESCRIBE(int resultCode, char* resultString)
{
	if (resultCode != 0) 
	{
		LIVE555_LOG << "Failed to DESCRIBE: " << resultString;
	}
	else
	{
		LIVE555_LOG << "Got SDP:\n" << resultString;
		m_session = MediaSession::createNew(envir(), resultString);
		m_subSessionIter = new MediaSubsessionIterator(*m_session);
		this->sendNextCommand();  
	}
	delete[] resultString;
}

void Live555RTSPConnection::continueAfterSETUP(int resultCode, char* resultString)
{
	if (resultCode != 0) 
	{
		LIVE555_LOG << "Failed to SETUP: " << resultString;
	}
	else
	{	
		Live555CodecType codec = GetSessionCodecType(m_subSession->mediumName(), m_subSession->codecName());
		m_subSession->sink = SessionSink::createNew(envir(), m_callback, codec, 
			m_subSession->fmtp_spropparametersets());
		if (m_subSession->sink == NULL) 
		{
			LIVE555_LOG << "Failed to create a data sink for " << m_subSession->mediumName() << "/" << m_subSession->codecName() << " subsession: " << envir().getResultMsg() << "\n";
		}
		else if (m_callback->onNewSession(codec, m_subSession->mediumName(), m_subSession->codecName()))
		{
			LIVE555_LOG << "Created a data sink for the \"" << m_subSession->mediumName() << "/" << m_subSession->codecName() << "\" subsession";
			m_subSession->sink->startPlaying(*(m_subSession->readSource()), NULL, NULL);
		}
	}
	delete[] resultString;
	this->sendNextCommand();  
}	

void Live555RTSPConnection::continueAfterPLAY(int resultCode, char* resultString)
{
	if (resultCode != 0) 
	{
		LIVE555_LOG << "Failed to PLAY: \n" << resultString;
	}
	else
	{
		LIVE555_LOG << "PLAY OK\n";
	}
	delete[] resultString;
}


