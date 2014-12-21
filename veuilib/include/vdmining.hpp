//------------------------------------------------------------------------------
// File: vdmining.hpp
//
// Desc: vdmining 
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_DMINING_HPP__
#define __V_DMINING_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VE_LIBRARY_API VDMining : public QWidget
{
	Q_OBJECT
public:
	VDMining(QWidget *parent = 0);
	~VDMining();
public:
	static BOOL Init(Factory &pFactory);
	static VDMining * CreateObject(QWidget *parent);
private:
	
};


#endif /* __V_DMINING_HPP__ */
