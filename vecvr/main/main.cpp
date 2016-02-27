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

Factory *gFactory = NULL;


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

	pFactory->start();
	/* Init Mining framework */

	/* Start the OpenCVR api server */
	pOAPIServer = new OAPIServerWrapper(*pFactory);
	pOAPIServer->start();
	
	return a.exec();
}
