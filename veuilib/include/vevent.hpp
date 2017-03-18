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
#ifndef __V_EVENT_HPP__
#define __V_EVENT_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include "server/eventserver.hpp"
#include <QThread>

using namespace UtilityLib;

class VOnvifEventThread;
class VSmartMotionThread;

class VE_LIBRARY_API VEventMgr
{
public:
	VEventMgr(Factory &pFactory, VEventServer &pEventServer)
	:m_pFactory(pFactory), m_pEventServer(pEventServer), m_OnvifThread(NULL), 
	m_SmartMotionThread(NULL)
	{
	}
	~VEventMgr()
	{
		
	}
public:
	bool Init();
private:
	VOnvifEventThread *m_OnvifThread;
	VSmartMotionThread *m_SmartMotionThread;
	Factory &m_pFactory;
	VEventServer &m_pEventServer;
};


#endif /* __V_EVENT_HPP__ */
