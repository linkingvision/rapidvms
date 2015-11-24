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
	Debug::init(9100);

    pFactory = new Factory;
	gFactory = pFactory;

	if (pFactory->Init() == FALSE)
	{
#ifdef WIN32
		astring strPath = "C:\\";//TODO get the hdd from hdd
#else

		astring strPath = "ve/";//TODO get the hdd from hdd
		s32 size = 2;
#endif
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
