#include "oapic.hpp"
int main(int argc, char *argv[])
{
	XRef<XSocket> pSocket = new XSocket;
	
	try
    {
		XSDK::XString host = "127.0.0.1";
		pSocket->Connect(host, 9080);
		
		oapi::DeviceList list;
		OAPIClient pClient(pSocket);
		
		pClient.DeviceListRequest(list);
    }
	catch( XSDK::XException& ex )
	{
		
	}
	
	x_sleep(10000);
}