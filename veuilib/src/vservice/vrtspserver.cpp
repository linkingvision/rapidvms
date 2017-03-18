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
#include "vrtspserver.hpp"
#include "server/factory.hpp"
#ifndef ARM_LINUX
//#include "rtspsapi.hpp"
#endif

class VRTSPServerImpl : public QThread
{
public:
	VRTSPServerImpl(Factory & pFactory);
	~VRTSPServerImpl();
public:
	void run();
	
private:
	bool m_bQuit;
	Factory & m_pFactory;
	BOOL m_pAuth;
	astring m_pMultiAddr;
	u16 m_pPort;
  
};

VRTSPServerImpl::VRTSPServerImpl(Factory & pFactory)
:m_bQuit(false), m_pFactory(pFactory), m_pPort(9554)
{
	//m_pFactory.GetRTSPServer(m_pAuth, m_pMultiAddr, m_pPort);
}

VRTSPServerImpl::~VRTSPServerImpl()
{

}

void VRTSPServerImpl::run()
{
#ifndef ARM_LINUX
#ifndef __APPLE__
    //VSCRTSPRun(m_pPort);
#endif
#endif
    return;
}



VRTSPServer::VRTSPServer(Factory & pFactory)
: m_pFactory(pFactory)
{
#ifndef ARM_LINUX
#ifndef __APPLE__
    //VSCRTSPSetFactory(pFactory);
#endif
#endif
    m_pImpl = new VRTSPServerImpl(pFactory);
    m_pImpl->start();
}

VRTSPServer::~VRTSPServer()
{

}


