//------------------------------------------------------------------------------
// File: main.cpp
//
// Desc: main - main.
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "server/factory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "server/cmnoapiserver.hpp"
#include "vevent.hpp"
#include "webserver.hpp"
#include "vrtspserver.hpp"
#include "server/eventserver.hpp"

Factory *gFactory = NULL;

inline string GetProgramRunningDir()
{
#ifdef WIN32
    char exeFullPath[MAX_PATH]; // Full path

    string strPath = "";

    GetModuleFileNameA(NULL,exeFullPath, MAX_PATH);
    strPath=(string)exeFullPath;    // Get full path of the file

    int pos = strPath.find_last_of('\\', strPath.length());

    return strPath.substr(0, pos) + "\\";  // Return the directory without the file name
#else
    return "ve/";
#endif
}


int main(int argc, char *argv[])
{
	int dummy = errno;
	OAPIServerWrapper *pOAPIServer = NULL;
	Factory *pFactory = NULL;

	QApplication a(argc, argv);
	astring strVSCDefaultPath = VSC_DEFAULT_SYSPATH;
#ifdef WIN32
#ifndef _WIN64
	astring strLoggerPath = strVSCDefaultPath + "\\vidstor\\logs\\";
#else
	astring strLoggerPath = strVSCDefaultPath + "\\vidstor64\\logs\\";
#endif
#else
astring strLoggerPath = strVSCDefaultPath + "/vidstor/logs/";
#endif

	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "vidstorlog";
	Debug::init(9100, strLoggerFile);

	Debug::logger().info("vidstor started");
	//Debug::logger().info("vidstor started {} {}", __LINE__, __FUNCTION__);
	//Debug::logger().info("vidstor started {} {}", __LINE__, __FUNCTION__);

    pFactory = new Factory;
	gFactory = pFactory;

	if (pFactory->Init() == FALSE)
	{
		astring strPath = VSC_DEFAULT_SYSPATH;
		pFactory->SetSystemPath(strPath);
		pFactory->Init();
	}
	
	astring docRoot = GetProgramRunningDir() + "www";
	
	const char *options[] = {
	    "document_root", docRoot.c_str(), "listening_ports", PORT, 0};
    
    std::vector<std::string> cpp_options;
    for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
        cpp_options.push_back(options[i]);
    }

	VEWebServer server(cpp_options, *pFactory);
	
	/* Start RTSP server */
	VRTSPServer *pRTSPServer = new VRTSPServer(*pFactory);

	/* Init Event Server */
	VEventServer *pEventServer = new VEventServer(*pFactory);
	pEventServer->Init();

	/* Init Event framework */
	VEventMgr *pEventMgr = new VEventMgr(*pFactory, *pEventServer);
	pEventMgr->Init();

	/* Start the OpenCVR api server */
	pOAPIServer = new OAPIServerWrapper(*pFactory, *pEventServer);
	pOAPIServer->start();

	pFactory->start();
	VDC_DEBUG("Start successfully !\n");
	
	return a.exec();
}
