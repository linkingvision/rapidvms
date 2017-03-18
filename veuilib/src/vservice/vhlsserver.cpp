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
#include "vhlsserver.hpp"


#if defined (WIN32__REMOVE) && !defined (WIN64)
#include "hls-server.h"
#include "sock.h"
#include "aio-socket.h"
#include "thread-pool.h"


thread_pool_t g_thpool;

static void HandleDataHls(VideoFrame& frame, void * pParam)
{
	u8  *dataBuf = NULL;
	u32 dataSize = 0;

	/* write frame to file */
	//VDC_DEBUG("HandleDataHls Size %d stream %d frame %d (%d, %d)\n", frame.dataLen,
	//	frame.streamType, frame.frameType, frame.secs, frame.msecs);

	if (frame.frameType == VIDEO_FRM_I)
	{

		dataBuf = frame.dataBuf + sizeof(InfoFrameI);
		dataSize = frame.dataLen - sizeof(InfoFrameI);
		InfoFrameI *pI = (InfoFrameI *)frame.dataBuf;
		/* Cache I frame for audio decoder */
	}

	if (frame.frameType == VIDEO_FRM_P)
	{
		dataBuf = frame.dataBuf + sizeof(InfoFrameP);
		dataSize = frame.dataLen - sizeof(InfoFrameP);
	}


	/* Skip info frame */
	if (frame.streamType == VIDEO_STREAM_AUDIO)
	{
		return;
	}

	bool keyFrame = false;
	if (frame.frameType == VIDEO_FRM_I)
	{
		keyFrame = true;
	}

	hsl_server_input(pParam, (void *)(dataBuf),
		(int)(dataSize), HLS_VIDEO_H264);

}

/* the id is the /1 and url is http://127.0.0.1:9001/livehls/1.m3u8 */
#if 0
int test_hls_live_open(void* param, void* camera, const char* id, const char* key, const char* publicId)
{
	printf("%s id %s key %s publicId %s\n", __FUNCTION__, id, key, publicId);
	gUiFactory->RegDataCallback(1, (DeviceDataCallbackFunctionPtr)HandleDataHls,
		(void *)camera);
	return 0;
	
}
int test_hls_live_close(void* param, const char* id)
{
	printf("%s id %s\n", __FUNCTION__, id);

	return 0;
}
#endif


static void AioWorker(void *param)
{
	while (1)
	{
		aio_socket_process(2 * 60 * 1000);
	}
}

class VHLSServerImpl : public QThread
{
public:
	VHLSServerImpl(Factory & pFactory);
	~VHLSServerImpl();
public:
	void run();

public:
	static int hlsLiveOpen(void* param, void* camera, 
						const char* id, const char* key, const char* publicId);
	int hlsLiveOpen1(void* camera, const char* id, const char* key, 
						const char* publicId);

	static int hlsLiveClose(void* param, const char* id);
	int hlsLiveClose1(const char* id);
	
private:
	bool m_bQuit;
	Factory & m_pFactory;
	u16 m_pPort;
  
};

VHLSServerImpl::VHLSServerImpl(Factory & pFactory)
:m_bQuit(false), m_pFactory(pFactory)
{
	m_pFactory.GetVHLSSPort(m_pPort);
	aio_socket_init(1);
	g_thpool = thread_pool_create(1, 1, 4);
	thread_pool_push(g_thpool, AioWorker, NULL); // start worker
}

VHLSServerImpl::~VHLSServerImpl()
{

}

int VHLSServerImpl::hlsLiveOpen(void* param, void* camera, 
					const char* id, const char* key, const char* publicId)
{
    int dummy = errno;
    VHLSServerImpl* pObject = (VHLSServerImpl *)param;

    if (pObject)
    {
        return pObject->hlsLiveOpen1(camera, id, key, publicId);
    }

	return -1;
}
int VHLSServerImpl::hlsLiveOpen1(void* camera, const char* id, const char* key, 
					const char* publicId)
{
	int deviceId;
	if (id == NULL)
	{
		return -1;
	}
	int ret = sscanf(id, "%d", &deviceId);

	if (ret <= 0)
	{
		return -1;
	}

	VDC_DEBUG("%s id Device ID %s %d key %s publicId %s\n", __FUNCTION__, id, 
					deviceId, key, publicId);
	m_pFactory.RegDataCallback(deviceId, (DeviceDataCallbackFunctionPtr)HandleDataHls,
		(void *)camera);

	return 0;
}

int VHLSServerImpl::hlsLiveClose(void* param, const char* id)
{
    int dummy = errno;
    VHLSServerImpl* pObject = (VHLSServerImpl *)param;

    if (pObject)
    {
        return pObject->hlsLiveClose1(id);
    }
	return -1;
}

int VHLSServerImpl::hlsLiveClose1(const char* id)
{
	//TODO add a task in the hls_server_t loop to check whitch hls_live_t is not used
	return -1;
}

void VHLSServerImpl::run()
{
	hls_server_init();
	void* hls = hls_server_create("0.0.0.0", m_pPort);
	hsl_server_set_handle(hls, VHLSServerImpl::hlsLiveOpen, 
					VHLSServerImpl::hlsLiveClose, (void *)this);

	while(!m_bQuit)
	{
		ve_sleep(10 * 1000);
	}
	//TODO Stop hls server
	return;
}



VHLSServer::VHLSServer(Factory & pFactory)
: m_pFactory(pFactory)
{
    m_pImpl = new VHLSServerImpl(pFactory);
    m_pImpl->start();
}

VHLSServer::~VHLSServer()
{

}

#endif

