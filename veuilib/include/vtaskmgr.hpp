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

using namespace UtilityLib;

class VTaskMgrImpl;

class VE_LIBRARY_API VTaskItem : QThread
{
public:
	VTaskItem(){}
	~VTaskItem(){}
public:
	/* the result is 0 to 100 */
	virtual int GetPercentage(){return 0;}
	virtual bool HasOpenResult(){return false;}
	virtual bool OpenResult(){return false;}
};

typedef std::list<VTaskItem *> VTaskItemList;

class VE_LIBRARY_API VTaskMgr
{
public:
	VTaskMgr();
	~VTaskMgr();
public:
	static bool GetTaskList(VTaskItemList &pList);
	static bool AddTask(VTaskItem *pTask);
	static bool DeleteTask(VTaskItem *pTask);
	
public:
	VTaskMgrImpl *m_pImpl;
};

#endif /* __V_TASK_MGR_HPP__ */
