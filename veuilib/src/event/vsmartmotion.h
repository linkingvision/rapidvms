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
#ifndef __V_SMART_MOTION_EVENT_H
#define __V_SMART_MOTION_EVENT_H

#include <QWidget>
#include "utility.hpp"
#include "server/factory.hpp"
#include "QEvent"
#include <QThread>
#include "vevent.hpp"
#include "vsmotalgo.hpp"

/* Dynamics FPS need get from here, and it the dfps stream, may the stream3
   the stream is only for relay, because the the record system can only record the motion video
   The Dynamics stream can be useful for mobile client*/

class SmartMotionCam
{
public:
	SmartMotionCam(string strId, int nStreamId, astring strName, astring strMotReg,
		Factory &pFactory, VEventServer &pEventServer);
	~SmartMotionCam();
public:
	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);	
	void RawHandler1(RawFrame& frame);
	static void RawHandler(void* pData, RawFrame& frame);	
private:
	string m_strId;
	string m_strName;
	int m_nStreamId;
	fast_mutex m_Lock;
	Factory &m_pFactory;
	VPlay m_vPlay;
	VSMotAlgo m_MotAlgo;
	VEventServer &m_pEventServer;
	s64 m_nLastMotTime; 
	VVidMotReg m_motReg;

};

typedef std::map<string, SmartMotionCam*> SmartMotionCamMap;

class VSmartMotionThread : public QThread
{
    Q_OBJECT
public:
    VSmartMotionThread(Factory &pFactory, VEventServer &pEventServer)
	:m_pFactory(pFactory), m_pEventServer(pEventServer)
    {
    }
    ~VSmartMotionThread()
    {
    }
public:

	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
public:
	void run();
	bool InitDevices();
	bool InitOneDevice(VidCamera  pParam);
	bool DeleteOneDevice(astring strId);
	
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}
	
private:
	fast_mutex m_Lock;
	Factory &m_pFactory;
	SmartMotionCamMap m_SmartMotionMap;
	VEventServer &m_pEventServer;
};

#endif /* __V_SMART_MOTION_EVENT_H */