//------------------------------------------------------------------------------
// File: vhlsserver.hpp
//
// Desc: hls server - HLS server.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_HLS_SERVER_HPP__
#define __V_HLS_SERVER_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VHLSServerImpl;
class VE_LIBRARY_API VHLSServer
{
public:
	VHLSServer(Factory & pFactory);
	~VHLSServer();
public:
private:
    Factory & m_pFactory;
    VHLSServerImpl *m_pImpl;
};

#endif /* __V_HLS_SERVER_HPP__ */
