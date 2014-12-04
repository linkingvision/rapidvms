//------------------------------------------------------------------------------
// File: main.cpp
//
// Desc: main - main.
//
// Copyright (c) 2014-2018. All rights reserved.
//------------------------------------------------------------------------------


#include "vscmainwindows.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "factory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include "vschdddevice.h"
#include "vevent.hpp"
#include "vservicemgr.hpp"

Factory *gFactory = NULL;

int main(int argc, char *argv[])
{
    int dummy = errno;
    QApplication a(argc, argv);
    Debug::init(0);
    //mediaPipeline::init(argc, argv);

	// Set the Application Language
#if 0
	//if (QLocale::system().name() == "zh_CN")
	{
		QTranslator *translator = new QTranslator(&a);
		bool ok = translator->load("vscloudnode_zh.qm",
			QCoreApplication::applicationDirPath() + "/translations");
		
		if (ok)
		{
			qDebug("Translation Files loaded.");
			a.installTranslator(translator);
		}
	}
#endif

    QPixmap pixmap(":/logo/resources/splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    QFont splashFont;
    //splashFont.setFamily("Arial");
    splashFont.setBold(true);

    splash->setStyleSheet(QStringLiteral("color : red;"));
    splash->setFont(splashFont);
    
    splash->show();

    //QApplication::setStyle(QStyleFactory::create("Fusion"));
	//QApplication::setStyle(QStyleFactory::create("Plastique"));
    //QApplication::setStyle("WindowsVista"); 
#if 0
	QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else 
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		a.setStyleSheet(ts.readAll());
	}
#endif


    gFactory = new Factory;
    splash->showMessage("Starting ...");

   if (gFactory->Init() == FALSE)
   {
#ifdef WIN32
       astring strPath = "C:\\";//TODO get the hdd from hdd
       VSCHddDevice hdd;
       hdd.show();
       hdd.exec();
       s32 size = hdd.GetDiskSize();
       hdd.GetDiskPath(strPath);
#else

	astring strPath = "ve/";//TODO get the hdd from hdd
	s32 size = 2;
#endif
       gFactory->SetSystemPath(strPath);
       splash->showMessage("Create Video Database ...");
       gFactory->Init();
#ifndef WIN32
	gFactory->AddHDD(strPath, size);
#endif
   }

   VEvent::Init(*gFactory);
   VServiceMgr *pServiceMgr = VServiceMgr::CreateObject(*gFactory);

    VSCMainWindows w;
    //w.setWindowIcon(QIcon(":/logo/resources/vscsmall.jpg"));
    //w.show();
    //Sleep(1000);
    //w.setWindowFlags(Qt::FramelessWindowHint );
    //w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint );
    w.showMaximized();
    //w.showFullScreen();
    splash->finish(&w);

	//stacked s;
	//s.show();

    delete splash;
    /* Every thread is ready, start the factory thread */
    gFactory->start();
    return a.exec();
}
