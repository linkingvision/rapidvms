#include "oapic.hpp"
int main(int argc, char *argv[])
{
	XSocket pSocket;
	
	XSDK::XString host = "127.0.0.1";
	pSocket.Connect(host, 9080);
	
	oapi::DeviceList list;
	OAPIClient pClient(&pSocket);
	
	pClient.DeviceListRequest(list);
}