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
#include "client/clientfactory.hpp"
#include <QThread>

using namespace UtilityLib;


class VE_LIBRARY_API VEMap : public QWidget
{
	Q_OBJECT
public:
	VEMap(QWidget *parent = 0);
	~VEMap();
public:
	static VEMap * CreateObject(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent);
	
};


#endif /* __V_EMAP_HPP__ */
