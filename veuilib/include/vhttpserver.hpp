//------------------------------------------------------------------------------
// File: vhttpserver.hpp
//
// Desc: http server - HTTP server.
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_HTTP_SERVER_HPP__
#define __V_HTTP_SERVER_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VHTTPServerImpl;
class VHTTPsServerImpl;
class VE_LIBRARY_API VHTTPServer
{
public:
	VHTTPServer(Factory & pFactory);
	~VHTTPServer();
public:
private:
	Factory & m_pFactory;
	VHTTPServerImpl *m_pImpl;
	VHTTPsServerImpl *m_pImplHttps;
};

#endif /* __V_HTTP_SERVER_HPP__ */
