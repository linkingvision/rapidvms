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
#include "vpoco.hpp"

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

static bool VHTTPClient(astring strHost, astring strPath, astring strUser, astring strPasswd, astring strBody
							astring &pResponse, const std::string& method)
{
	try {
		Poco::Net::HTTPCredentials creds(strUser, strPasswd);
		
		Poco::Net::HTTPClientSession session(strHost);
		Poco::Net::HTTPRequest request(method, strPath, HTTPMessage::HTTP_1_1);
		session.sendRequest(request);
		Poco::Net::HTTPResponse response;
		std::istream& firstResponseStream = session.receiveResponse(response);
		std::stringstream firstStrStream;
		Poco::StreamCopier::copyStream(firstResponseStream, firstStrStream);
		
		if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
		{
			creds.authenticate(request, response);
			session.sendRequest(request);
		}
		
		std::istream& bodyStream = session.receiveResponse(response);
		if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
		{
			return false;
		}
		
		Poco::StreamCopier::copyToString(bodyStream, pResponse);
		
	}catch (Poco::Exception& e) {
		std::cout << e.displayText() << std::endl;
		return false;
	}
	
	return true;
}

bool VHttpClientGet(astring strHost, astring strPath, astring strUser, astring strPasswd, 
							astring &pResponse)
{
	astring strBody = "dummy";
	return VHTTPClient(strHost, strPath, strUser, strPasswd, strBody, pResponse, HTTPRequest::HTTP_GET);
}
bool VHttpClientPost(astring strHost, astring strPath, astring strUser, astring strPasswd, astring strBody
							astring &pResponse)
{
	return VHTTPClient(strHost, strPath, strUser, strPasswd, strBody, pResponse, HTTPRequest::HTTP_POST);
}

