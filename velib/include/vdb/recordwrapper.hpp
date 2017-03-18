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
#ifndef __RECORD_WRAPPER_HPP__
#define __RECORD_WRAPPER_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

#define VDB_DAYS_IN_WEEK 7
#define VDB_HOURS_IN_DAY 24

#define VDB_MON 0
#define VDB_TUE 1
#define VDB_WEN 2
#define VDB_THR 3
#define VDB_FRI 4
#define VDB_SAT 5
#define VDB_SUN 6



class RecordSchedDay
{
public:
	RecordSchedDay()
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i++)
		{
			type[i] = R_OFF;
		}
	}
	RecordSchedDay(RecordingType allType)
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			type[i] = allType;
		}
	}
	RecordSchedDay(const RecordSchedDay &pDay)
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			type[i] = pDay.type[i];
		}
	}
	RecordSchedDay  operator +  (const RecordSchedDay& pDay) const
	{
		RecordSchedDay dayType;
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			dayType.type[i] = pDay.type[i] | type[i];
		}

		return dayType;
	}
public:
	u8 type[VDB_HOURS_IN_DAY];
};

class RecordSchedWeek
{
public:
	RecordSchedWeek()
	{
		RecordSchedDay dayType;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = dayType;
		}
	}
	RecordSchedWeek(RecordSchedDay allType)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = allType;
		}
	}
	RecordSchedWeek(const RecordSchedWeek &pWeek)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = pWeek.week[i];
		}
	}
	RecordSchedWeek(std::string &pWeek)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			for (int j = 0; j < VDB_HOURS_IN_DAY; j ++)
			{
				week[i].type[j] = pWeek[i * VDB_HOURS_IN_DAY + j];
			}
			
		}
	}
	std::string ToString()
	{
		std::string sched;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			for (int j = 0; j < VDB_HOURS_IN_DAY; j ++)
			{
				char c = week[i].type[j];
				sched.push_back(c);
			}
			
		}

		return sched;
	}
	RecordSchedWeek  operator +  (const RecordSchedWeek& pWeek) const
	{
		RecordSchedWeek weekType;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			weekType.week[i] = pWeek.week[i] + week[i];
		}
		return weekType;
	}
public:
	static RecordSchedWeek CreateOff()
	{
		RecordSchedDay day(R_OFF);
		
		RecordSchedWeek week(day);
		
		return week;
	}
	static RecordSchedWeek CreateWorkDay()
	{
		RecordSchedDay day(R_SCHED);
		RecordSchedDay dayOff(R_OFF);
		
		RecordSchedWeek week(day);
		week.week[VDB_SAT] = dayOff;
		week.week[VDB_SUN] = dayOff;
		
		return week;
	}
	static RecordSchedWeek CreateAllDay()
	{
		RecordSchedDay day(R_SCHED);
		
		RecordSchedWeek week(day);
		return week;
	}
	static bool CheckStringForSched(std::string &pSched)
	{
		if (pSched.length() != VDB_DAYS_IN_WEEK * VDB_HOURS_IN_DAY)
		{
			return false;
		}else
		{
			return true;
		}
	}
public:
	RecordSchedDay week[VDB_DAYS_IN_WEEK];
};

class VDB;
class RecordWrapperImpl;
typedef std::map<astring, RecordSchedWeek> RecordSchedWeekMap;
typedef void ( * RecChangeFunctionPtr)(astring strId, bool bRec, void * pParam);

class VE_LIBRARY_API RecordWrapper
{
public:
    RecordWrapper(VDB &pVdb, astring deviceId, 
				RecordSchedWeekMap cSchedMap, RecChangeFunctionPtr pCallback, 
				void *pParam);
    ~RecordWrapper();
public:
	MFStatus PushAFrame(VideoFrame &frame);
	bool UpdateSchedMap(RecordSchedWeekMap cSchedMap);
	bool FireAlarm(s64 nStartTime);
	bool ManualRecStart();
	bool ManualRecStop();
	bool SetPrePostRecordTime(s32 nPreTime, s32 nPostTime);

private:
	RecordWrapperImpl *m_pImpl;
};

#endif /* __RECORD_WRAPPER_HPP__ */
