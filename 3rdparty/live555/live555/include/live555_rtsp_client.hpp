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

#ifndef __LIVE555_RTSP_CLIENT__
#define __LIVE555_RTSP_CLIENT__

#include <string.h>
#include <vector>
#include <iostream>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "DigestAuthentication.hh"
#include "H264VideoRTPSource.hh"

#define LIVE555_LOG std::cout

#if defined(_WIN32) || defined(_WIN64) 
#define snprintf _snprintf 
#define vsnprintf _vsnprintf 
#define strcasecmp _stricmp 
#define strncasecmp _strnicmp 
#endif

#if defined(_WIN32)
#if defined(LIVE555_EXPORTS)
#define LIVE555_LIBRARY_API __declspec(dllexport)
#else
#define LIVE555_LIBRARY_API __declspec(dllimport)
#endif
#else
#define LIVE555_LIBRARY_API
#endif


#define RTSP_CALLBACK(uri, resultCode, resultString) \
static void continueAfter ## uri(RTSPClient* rtspClient, int resultCode, char* resultString) { static_cast<Live555RTSPConnection*>(rtspClient)->continueAfter ## uri(resultCode, resultString); } \
void continueAfter ## uri (int resultCode, char* resultString); \

typedef enum __Live555CodecType
{
        LIVE555_CODEC_PCMU = 0, 
        LIVE555_CODEC_PCMA = 8,
        LIVE555_CODEC_G711A = 19,
        LIVE555_CODEC_G711U = 20, 
        LIVE555_CODEC_H264 = 96,
        LIVE555_CODEC_H265 = 98,
        LIVE555_CODEC_MPEG4 = 97,
        LIVE555_CODEC_MJPEG = 26,
        LIVE555_CODEC_AAC = 100,
        LIVE555_CODEC_NONE = 254,
        LIVE555_CODEC_LAST = 1000
}Live555CodecType;

inline Live555CodecType GetSessionCodecType(const char* media, const char* codec)
{
	Live555CodecType nCodec = LIVE555_CODEC_NONE;
	if ( (strcasecmp(media, "video") == 0) && (strcasecmp(codec, "H264") == 0) )
	{
		nCodec = LIVE555_CODEC_H264;
	}else if ((strcasecmp(media, "video") == 0) && (strcasecmp(codec, "H265") == 0))
	{
		nCodec = LIVE555_CODEC_H265;
	}else
	{
		nCodec = LIVE555_CODEC_NONE;
	}

	return nCodec;
}

inline  void Live555DumpHex(unsigned char *pNuf, int nLen)
{
	int i = 0;
	for (i = 0; i < nLen; i ++)
	{
		printf(" 0x%x", pNuf[i]);
	}
	printf("\n\n");
}

class LIVE555_LIBRARY_API Live555Environment : public BasicUsageEnvironment
{
	public:
		Live555Environment();
		~Live555Environment();
	
	
		void mainloop()
		{
			this->taskScheduler().doEventLoop(&m_stop);
		}
				
		void stop() { m_stop = 1; };	
		
	protected:
		char                     m_stop;		
};


/* ---------------------------------------------------------------------------
**  RTSP client connection interface
** -------------------------------------------------------------------------*/
class LIVE555_LIBRARY_API Live555RTSPConnection : public RTSPClient
{
	public:
		/* ---------------------------------------------------------------------------
		**  RTSP client callback interface
		** -------------------------------------------------------------------------*/
		class Callback
		{
			public:
				virtual bool    onNewSession(Live555CodecType nCodec, const char* media, const char* codec) = 0;
				/* TODO Add a param steam type video/audio/meda data */
				virtual bool    onData(unsigned char* buffer, int size, unsigned long long secs, 
											unsigned long long msecs, Live555CodecType codec) = 0;
				/*
					H264 "sprop-parameter-sets"
					H265 "sprop-vps" "sprop-sps" "sprop-pps"
				*/
				virtual bool onParameter(std::string strKey, std::string strValue) = 0;
				virtual int onNewBuffer(unsigned char* buffer, int size) { return 0; };
		};

	protected:
		/* ---------------------------------------------------------------------------
		**  RTSP client Sink
		** -------------------------------------------------------------------------*/
		class SessionSink: public MediaSink 
		{
			public:
				static SessionSink* createNew(UsageEnvironment& env, Callback* callback, 
					Live555CodecType codec, char const* spropparametersets) 
					{ return new SessionSink(env, callback, codec, spropparametersets); }

			private:
				SessionSink(UsageEnvironment& env, Callback* callback, Live555CodecType codec, 
					char const* spropparametersets);
				virtual ~SessionSink();

				void allocate(int bufferSize);

				static void afterGettingFrame(void* clientData, unsigned frameSize,
							unsigned numTruncatedBytes,
							struct timeval presentationTime,
							unsigned durationInMicroseconds)
				{
					static_cast<SessionSink*>(clientData)->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
				}
				
				void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds);

				virtual Boolean continuePlaying();

			private:
				size_t                 m_bufferSize;
				u_int8_t*              m_buffer;
				Callback*              m_callback; 	
				int                m_markerSize;
				Live555CodecType m_codecType;
				std::string m_fmtp_spropparametersets;//"sprop-parameter-sets"
				bool 		m_bFirstFrame;
		};
	
	public:
		Live555RTSPConnection(UsageEnvironment& env, Callback* callback, const std::string & rtspURL, 
			std::string strUser, std::string strPasswd, bool bTCP = true, int verbosityLevel = 255);
		virtual ~Live555RTSPConnection();

	protected:
		void sendNextCommand(); 
				
		RTSP_CALLBACK(DESCRIBE,resultCode,resultString);
		RTSP_CALLBACK(SETUP,resultCode,resultString);
		RTSP_CALLBACK(PLAY,resultCode,resultString);
		
	protected:
		MediaSession*            m_session;                   
		MediaSubsession*         m_subSession;             
		MediaSubsessionIterator* m_subSessionIter;
		Callback*                m_callback; 
		Authenticator m_Authenticator;
		bool m_bTCP;
};

class LIVE555_LIBRARY_API Live555RTSPClient : public Live555RTSPConnection::Callback
{
	public:
		Live555RTSPClient(const std::string uri, 
			std::string strUser, std::string strPasswd, bool bTCP = true) 
		: m_connection(m_env,this,uri.c_str(), strUser, strPasswd, bTCP)
		{
			LIVE555_LOG << "===========================Live555RTSPClient" << uri.c_str();
		}
	  
		virtual ~Live555RTSPClient() 
		{
		}
		
		virtual bool onNewSession(Live555CodecType nCodec, const char* media, const char* codec)
		{
			LIVE555_LOG << "===========================onNewSession" << media << "/" << codec;
			bool success = false;
			if ( (strcasecmp(media, "video") == 0) && (strcasecmp(codec, "H264") == 0) )
			{
				success = true;
			}
			return success;			
		}
		
		virtual bool onData(unsigned char* buffer, int size, unsigned long long secs, 
											unsigned long long msecs, Live555CodecType codec)
		{		

			std::cout << "onData" << "  " << codec << "  " << "  " << secs << "  " << msecs  << "  " << size << std::endl;

			return true;
		}
		virtual bool onParameter(std::string strKey, std::string strValue)
		{
			printf("%s key %s value %s\n", __FUNCTION__, strKey.c_str(), strValue.c_str());
			return true;			
		}
	  
		virtual void RTSPStop() 
		{
			m_env.stop();
		}
		
		void Run()
		{	
			m_env.mainloop();
		}

	private:
		Live555Environment    m_env;
		Live555RTSPConnection m_connection;
		
};

#endif 

