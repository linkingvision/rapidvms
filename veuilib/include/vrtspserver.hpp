//------------------------------------------------------------------------------
// File: vrtspserver.hpp
//
// Desc: rtsp server - RTSP server.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_RTSP_SERVER_HPP__
#define __V_RTSP_SERVER_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VRTSPServerImpl;
class VE_LIBRARY_API VRTSPServer
{
public:
	VRTSPServer(Factory & pFactory);
	~VRTSPServer();
public:
private:
    Factory & m_pFactory;
    VRTSPServerImpl *m_pImpl;
};

#endif /* __V_RTSP_SERVER_HPP__ */
