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
#include "vservicemgr.hpp"

Factory *gFactory = NULL;

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
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
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
    //QFont splashFont;
    //splashFont.setFamily("Arial");
    //splashFont.setBold(true);

    	splash->setStyleSheet(QStringLiteral("color : red;"));
    //splash->setFont(splashFont);
    
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
#ifndef WIN32
	//gFactory->AddHDD(strPath, size);
#endif
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
	return a.exec();
}
