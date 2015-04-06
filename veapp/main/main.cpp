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
#include <QTextCodec>
#include "vschdddevice.h"
#include "vevent.hpp"
#include "vemap.hpp"
#include "vservicemgr.hpp"
#include "mframework.hpp"
#include "cmnoapiserver.hpp"

Factory *gFactory = NULL;
MFramework *gMFramework = NULL;
OAPIServer *gOAPIServer = NULL;

void LoadLangZH(QApplication &a)
{
	QTranslator *translator = new QTranslator(&a);
	bool ok = translator->load("opencvr_zh.qm",
		QCoreApplication::applicationDirPath() + "/translations");

	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translator);
	}
	QTranslator *translatorVe = new QTranslator(&a);
	ok = translatorVe->load("veuilib_zh.qm",
		QCoreApplication::applicationDirPath() + "/translations");

	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translatorVe);
	}
	return ;
}

int main(int argc, char *argv[])
{
	int dummy = errno;

	QApplication a(argc, argv);
	Debug::init(0);

#ifdef WIN32
	QFont font;
	font.setPointSize(10); 
	font.setFamily(("Î¢ÈíÑÅºÚ"));
	font.setBold(false);

	a.setFont(font);
#else
	QFont font;
	font.setPointSize(10); 
	font.setFamily(("WenQuanYi Zen Hei"));
	font.setBold(false);

	a.setFont(font);
#endif

	QPixmap pixmap(":/logo/resources/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);

    	splash->setStyleSheet(QStringLiteral("color : white;"));    
    	splash->show();

    	gFactory = new Factory;

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
		//splash->showMessage(QObject::tr("Create Video Database ..."));
		gFactory->Init();
	}
	VSCLangType m_lang;
	gFactory->GetLang(m_lang);
	if (m_lang == VSC_LANG_AUTO)
	{
		if (QLocale::system().name() == "zh_CN")
		{
			LoadLangZH(a);
		}
	}
	else if (m_lang == VSC_LANG_ZH)
	{
		LoadLangZH(a);
	}//else if add more language to here
	
	splash->showMessage(QObject::tr("Starting ..."));
	VEvent::Init(*gFactory);
	VEMap::Init(*gFactory);
	VServiceMgr *pServiceMgr = VServiceMgr::CreateObject(*gFactory);
	
	VSCMainWindows w;

	//w.showMaximized();
	w.hide();
	//w.showFullScreen();
	splash->finish(&w);
	/* Auto  */
	if (gFactory->GetAutoLogin() == FALSE)
	{
		w.ShowLogin();
	}else
	{
		w.showMaximized();
	}

	delete splash;
	/* Every thread is ready, start the factory thread */
	
	gFactory->start();
	/* Init Mining framework */
	gMFramework = new MFramework(*gFactory);
	gMFramework->Init();

	gMFramework->start();

	/* Start the OpenCVR api server */
	gOAPIServer = new OAPIServer(*gFactory);
	gOAPIServer->start();
	
	return a.exec();
}
