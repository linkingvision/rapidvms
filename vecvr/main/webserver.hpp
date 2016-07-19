#ifndef __VE_WEB_SERVER_H__
#define __VE_WEB_SERVER_H__

#include "CivetServer.h"
#include "webserverapi.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define DOCUMENT_ROOT "."
#define PORT "9088"
#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"

class ExampleHandler : public CivetHandler
{
  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>\r\n");
		mg_printf(conn,
		          "<h2>This is an example text from a C++ handler</h2>\r\n");
		mg_printf(conn,
		          "<p>To see a page from the A handler <a "
		          "href=\"a\">click here</a></p>\r\n");
		mg_printf(conn,
		          "<p>To see a page from the A handler with a parameter "
		          "<a href=\"a?param=1\">click here</a></p>\r\n");
		mg_printf(conn,
		          "<p>To see a page from the A/B handler <a "
		          "href=\"a/b\">click here</a></p>\r\n");
		mg_printf(conn,
		          "<p>To see a page from the *.foo handler <a "
		          "href=\"xy.foo\">click here</a></p>\r\n");
		mg_printf(conn, "</body></html>\r\n");
		return true;
	}
};

class ExitHandler : public CivetHandler
{
  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "text/plain\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "Bye!\n");
		return true;
	}
};

class AHandler : public CivetHandler
{
  private:
	bool
	handleAll(const char *method,
	          CivetServer *server,
	          struct mg_connection *conn)
	{
		std::string s = "";
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>");
		mg_printf(conn, "<h2>This is the A handler for \"%s\" !</h2>", method);
		if (CivetServer::getParam(conn, "param", s)) {
			mg_printf(conn, "<p>param set to %s</p>", s.c_str());
		} else {
			mg_printf(conn, "<p>param not set</p>");
		}
		mg_printf(conn, "</body></html>\n");
		return true;
	}

  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("GET", server, conn);
	}
	bool
	handlePost(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("POST", server, conn);
	}
};

class ABHandler : public CivetHandler
{
  public:
	bool
	handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>");
		mg_printf(conn, "<h2>This is the AB handler!!!</h2>");
		mg_printf(conn, "</body></html>\n");
		return true;
	}
};


class VEWebServer 
{
public:
	VEWebServer(std::vector<std::string> cpp_options, Factory &pFactory)
		:pServer(new CivetServer(cpp_options)), server(*pServer), m_pFactory(pFactory), 
		h_GetCamList(pFactory)
	{
		server.addHandler("/oapi/GetCamList", h_GetCamList);

		//server.addHandler("/a/b", h_ab);
		//server.addHandler("/a", h_a);

		//server.addHandler("", h_foo);	
		//server.addHandler(EXAMPLE_URI, h_ex);		
	}
	~VEWebServer(){}
	
private:
	ExampleHandler h_ex;
	AHandler h_a;
	ABHandler h_ab;
	WebAPIGetCamListHandler h_GetCamList;
	CivetServer *pServer;
	CivetServer &server;
	Factory &m_pFactory;
};

#endif /* __VE_WEB_SERVER_H__ */
