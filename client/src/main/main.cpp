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
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "server/cmnoapiserver.hpp"
#include "vstyle.hpp"
#include "vsclogin.h"

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
	ClientFactory *pFactory = NULL;

	QApplication a(argc, argv);

	a.setStyle(new VStyle); 
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
	astring strLoggerFile = strLoggerPath + "opencvrclient";
	Debug::init(9200, strLoggerFile);

	Debug::logger().info("opencvrclient started");
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);

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
	
    pFactory = new ClientFactory;

	if (pFactory->Init() == FALSE)
	{
		astring strPath = VSC_DEFAULT_SYSPATH;
		pFactory->SetSystemPath(strPath);
		pFactory->Init();
	}

	VidLanguage m_lang;
	pFactory->GetLang(m_lang);
	if (m_lang == VID_LANG_AUTO)
	{
		if (QLocale::system().name() == "zh_CN")
		{
			LoadLangZH(a);
		}
	}
	else if (m_lang == VID_ZH_CN)
	{
		LoadLangZH(a);
	}//else if add more language to here

	
	splash->showMessage(QObject::tr("Starting ..."));

	VSCMainWindows w(*pFactory);

	//w.showMaximized();
	w.hide();
	//w.showFullScreen();
	splash->finish(&w);
	/* Auto  */
	if (pFactory->GetAutoLogin() == false)
	{
		w.ShowLogin();
	}else
	{
		w.showMaximized();
	}

	delete splash;
	return a.exec();
}
