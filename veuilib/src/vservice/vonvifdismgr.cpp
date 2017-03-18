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

#include "vonvifdismgr.hpp"
#include "onvifserver.hpp"
#include "onvifserverdiscover.hpp"

class VONVIFDisMgrImpl : public QThread
{
public:
	VONVIFDisMgrImpl();
	~VONVIFDisMgrImpl();
public:
	void run();
	bool AddHost(astring host, astring port, astring type)
	{
		return m_pTask->AddHost(host, port,  type);
	}
	bool DelHost(astring host, astring port)
	{
		return m_pTask->DelHost(host, port);
	}
	
private:
	struct soap * m_pSoap;
	OnvifServerDiscoverTask * m_pTask;
	bool m_bQuit;

};

VONVIFDisMgrImpl::VONVIFDisMgrImpl()
:m_pSoap(soap_new()), m_bQuit(false)
{
	m_pTask = new OnvifServerDiscoverTask(m_pSoap);
}

VONVIFDisMgrImpl::~VONVIFDisMgrImpl()
{

}

void VONVIFDisMgrImpl::run()
{
	m_pTask->Run(m_bQuit);
}


VONVIFDisMgr::VONVIFDisMgr()
:m_pImpl(NULL)
{
	m_pImpl = new VONVIFDisMgrImpl();
	m_pImpl->start();
}

bool VONVIFDisMgr::AddHost(astring host, astring port, astring type)
{
    return m_pImpl->AddHost(host, port, type);
}
bool VONVIFDisMgr::DelHost(astring host, astring port)
{
    return m_pImpl->DelHost(host, port);
}

VONVIFDisMgr::~VONVIFDisMgr()
{

}

