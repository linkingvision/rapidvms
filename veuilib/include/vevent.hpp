//------------------------------------------------------------------------------
// File: vevent.hpp
//
// Desc: onvif event - Manage ONVIF IPC event.
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_EVENT_HPP__
#define __V_EVENT_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;

class VE_LIBRARY_API VEventThread : public QThread
{
    Q_OBJECT
public:
	VEventThread();
	~VEventThread();
signals:
	void EventNotify(int deviceId, int type);
	void EventNotifyNoParam();
public:
	static VEventThread * CreateObject();
private:

};

class VE_LIBRARY_API VEvent : public QWidget
{
	Q_OBJECT
public:
	VEvent(QWidget *parent = 0);
	~VEvent();
public:
	static BOOL Init(Factory &pFactory);
	static VEvent * CreateObject(QWidget *parent);
private:
	
};


#endif /* __V_EVENT_HPP__ */
