#ifndef __RECORD_WRAPPER_HPP__
#define __RECORD_WRAPPER_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

#define VDB_DAYS_IN_WEEK 7
#define VDB_HOURS_IN_DAY 24

#define VDB_SUN 0
#define VDB_MON 1
#define VDB_TUE 2
#define VDB_WEN 3
#define VDB_THR 4
#define VDB_FRI 5
#define VDB_SAT 6



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
	u32 type[VDB_HOURS_IN_DAY];
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
	bool FireAlarm(AlarmType alarm);

private:
	RecordWrapperImpl *m_pImpl;
};

#endif /* __RECORD_WRAPPER_HPP__ */
