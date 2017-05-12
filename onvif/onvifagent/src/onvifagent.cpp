
#include "webserver.hpp"


int main(int argc, char *argv[])
{	
	std::string docRoot = "www";
	
	const char *options[] = {
		"document_root", docRoot.c_str(), 
		"listening_ports", PORT,
		0};
    
	std::vector<std::string> cpp_options;
	for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
	    cpp_options.push_back(options[i]);
	}

	OnvifAgentWebServer *  pWebServer = new OnvifAgentWebServer(cpp_options);
	
	printf("Start successfully !\n");
	while(1)
	{
	    std::chrono::milliseconds dura( 1000 );
	    std::this_thread::sleep_for( dura );
	}
	
	return 0;
}
