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
#include "vtaskmgr.hpp"
#include <QFileDialog>

Q_DECLARE_METATYPE (std::string) 

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

	char *argv1[] = 
		{"rapidclient", 
		NULL};
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;

        char *argv2[] =
                {"rapidclient",
                NULL};
    int argc2 = sizeof(argv2) / sizeof(char*) - 1;
	VidEnv env;
	env.init(argc2, argv2);
	env.run();
	
	QApplication a(argc1, argv1);
#if 0
	QFileDialog *fileDialog = new QFileDialog();

	if (fileDialog->exec() == QDialog::Accepted) {
	}
#endif
	a.setStyle(new VStyle);
	

	astring strLoggerPath = env.GetAppConfPath("logs");
	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "rapidclient";
	Debug::init(9200, strLoggerFile);

	Debug::logger().info("rapidclient started");
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);

	QPixmap pixmap(":/logo/resources/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	qRegisterMetaType<QVector<int> >("QVector<int>");
	qRegisterMetaType<std::string>("std::string");

	splash->setStyleSheet(QStringLiteral("color : white;"));    
	splash->show();
	
    pFactory = new ClientFactory(env);

	pFactory->Init();

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

	VTaskMgr *pVTaskMgr = new VTaskMgr();

	VSCMainWindows * w = new VSCMainWindows(*pFactory);

	//w.showMaximized();
	w->hide();
	//w.showFullScreen();
	splash->finish(w);
	/* Auto  */
	if (pFactory->GetAutoLogin() == false)
	{
		w->ShowLogin();
	}else
	{
		w->showMaximized();
	}

	VDC_DEBUG("Start successfully !\n");
	delete splash;
	return a.exec();
}
