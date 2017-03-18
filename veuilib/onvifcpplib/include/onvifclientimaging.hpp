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
#ifndef __ONVIF_CLIENT_IMAGING__
#define __ONVIF_CLIENT_IMAGING__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "onvifclientdevice.hpp"
#include "soapStub.h"
#include "soapImagingBindingProxy.h"
#include "wsseapi.h"


using namespace std;

class OnvifClientImaging
{
public:
    OnvifClientImaging(OnvifClientDevice &device);
    ~OnvifClientImaging();
public:
	/* Add function to here */
				
private:
	OnvifClientDevice &m_Device;

};

inline OnvifClientImaging::OnvifClientImaging(OnvifClientDevice &device)
: m_Device(device)
{

}

inline OnvifClientImaging::~OnvifClientImaging()
{

}


#endif 