//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

OAPIServer::OAPIServer(XSocket *pSocket, Factory &pFactory)
:m_pFactory(pFactory), m_pSocket(pSocket)
{

}
OAPIServer::~OAPIServer()
{

}

BOOL OAPIServer::Process(OAPIHeader &header)
{
	return TRUE;
}

#endif