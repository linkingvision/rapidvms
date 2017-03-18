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

#include "vservicemgr.hpp"

VServiceMgr * g_pVServiceMgr =NULL;
Factory *gUiFactory;

VServiceMgr::VServiceMgr(Factory & pFactory)
:m_pONVIFDisMgr(NULL), 
m_pVRTSPServer(NULL), m_pVTaskMgr(NULL), 
#if defined (WIN32) && !defined (WIN64)
m_pVVIPCMgr(NULL), m_pVONVIFPGMgr(NULL),
#endif
m_pFactory(pFactory)
{
    gUiFactory = &pFactory;
    m_pONVIFDisMgr  = new VONVIFDisMgr();
    m_pVVIPCMgr = new VVIPCMgr(pFactory, *m_pONVIFDisMgr);
    m_pVRTSPServer = new VRTSPServer(pFactory);
#if defined (WIN32__REMOVE) && !defined (WIN64)
    m_pVHTTPServer = new VHTTPServer(pFactory);
    m_pVHLSServer = new VHLSServer(pFactory);
#endif
    m_pVTaskMgr = new VTaskMgr();
    
}

VServiceMgr::~VServiceMgr()
{

}

VServiceMgr * VServiceMgr::CreateObject(Factory & pFactory)
{
	if (g_pVServiceMgr == NULL)
	{
		g_pVServiceMgr = new VServiceMgr(pFactory);
	}

	return g_pVServiceMgr;
}

