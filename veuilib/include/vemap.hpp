//------------------------------------------------------------------------------
// File: vemap.hpp
//
// Desc: EMAP 
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_EMAP_HPP__
#define __V_EMAP_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include <QTabWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VE_LIBRARY_API VEMap : public QWidget
{
	Q_OBJECT
public:
	VEMap(QWidget *parent = 0);
	~VEMap();
public slots:
	void DeviceEvent(int deviceId, int type);
public:
	virtual void OnDeviceEvent(int deviceId, int type) = 0;
public:
	static BOOL Init(Factory &pFactory);
	static VEMap * CreateObject(QTabWidget &pTabbed, QWidget *parent);
private:
	
};


#endif /* __V_EMAP_HPP__ */
