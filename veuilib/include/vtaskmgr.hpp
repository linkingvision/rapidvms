//------------------------------------------------------------------------------
// File: vtaskmgr.hpp
//
// Desc: Task manager - Manage Task.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_TASK_MGR_HPP__
#define __V_TASK_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>
#include "Poco/UUIDGenerator.h"

using namespace UtilityLib;

class VTaskMgrImpl;

class VE_LIBRARY_API VTaskItem : public QThread
{
	Q_OBJECT
public:
	VTaskItem()
	{
		Poco::UUIDGenerator uuidCreator;
		m_strId  = uuidCreator.createRandom().toString();
		/* The task will be auto deleted */
	}
	~VTaskItem(){}
public:
	/* the result is 0 to 100 */
	virtual int GetPercentage(){return 0;}
	virtual bool HasOpenResult(){return FALSE;}
	virtual bool OpenResult(){return FALSE;}
	virtual bool StopTask(){return FALSE;}
	virtual bool IsEol(){return FALSE;}
	virtual std::string GetTaskName(){return "none";}
	std::string GetId()
	{
		return m_strId;
	}
private:
	std::string m_strId;
};

typedef std::map<std::string, VTaskItem *> VTaskItemList;

class VE_LIBRARY_API VTaskMgr
{
public:
	VTaskMgr();
	~VTaskMgr();
public:
	static bool GetTaskList(VTaskItemList &pList);
	static bool AddTask(VTaskItem *pTask);
	static bool DeleteTask(VTaskItem *pTask);
	static int GetTaskCnt();
	
public:
	VTaskMgrImpl *m_pImpl;
};

#endif /* __V_TASK_MGR_HPP__ */
