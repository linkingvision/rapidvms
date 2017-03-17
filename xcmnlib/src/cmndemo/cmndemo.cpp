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
#include "utility/videotype.hpp"
#include "cppkit/os/ck_time_utils.h"
#include "rapidmedia/rapidmedia.hpp"
int main(int argc, char *argv[])
{
	int i = 0;
	RapidMedia rapid;
	rapid.Init(TRUE, "rtsp://192.168.22.134:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1", "admin", "admin", TRUE);
	rapid.StartRaw();
	while(1)
	{
		cppkit::ck_sleep(10);
		printf("cnt %d\n", i ++);
	}
	while(1)
	{
		
		if (i %2 == 0)
		{
			rapid.Start();
			rapid.StartRaw();
		}else
		{
			rapid.Stop();
			rapid.StopRaw();
		}
		cppkit::ck_sleep(10);
		printf("cnt %d\n", i ++);
	}
}
