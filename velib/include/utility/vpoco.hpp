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
#ifndef _V_POCO_H_
#define _V_POCO_H_

#include "utility.hpp"
#include "fast_mutex.h"

/* 
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"
#include "Poco/Net/HTTPCredentials.h" 


using Poco::NotificationCenter;
using Poco::NotificationQueue;
using Poco::Notification;
using Poco::AutoPtr;
*/



VE_LIBRARY_API bool VHttpClientGet(astring strHost, astring strPath, astring strUser, astring strPasswd, 
							astring &pResponse);
VE_LIBRARY_API bool VHttpClientPost(astring strHost, astring strPath, astring strUser, astring strPasswd, , astring strBody
							astring &pResponse);


#endif /* _VDC_UTILITY_H_ */
