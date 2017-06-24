/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
#include "vonvifdismgr.hpp"
#include "devicesearcher.h"

Factory *gFactory = NULL;
astring gAppdir;

#include <Poco/AutoPtr.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Format.h>
#include <Poco/Util/Application.h>


static BOOL WebServerUserChangeNotify(void* pParam, astring strUser, astring strPasswd)
{
	astring strPasswdPath = gAppdir + ".htpasswd";
	SimpleCrypt crypt;
	QString strDePasswd = strPasswd.c_str();

	mg_modify_passwords_file(strPasswdPath.c_str(), "rapidvms.com", strUser.c_str(),
		crypt.decryptToString(strDePasswd).toStdString().c_str());
	return TRUE;
	
}

int main(int argc, char *argv[])
{
	int dummy = errno;
	OAPIServerWrapper *pOAPIServer = NULL;
	Factory *pFactory = NULL;
	char *argv1[] = 
		{"rapidstor", 
		"-platform", 
		"offscreen", 
		NULL};
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;
        char *argv2[] =
                {"rapidstor",
                NULL};
        int argc2 = sizeof(argv2) / sizeof(char*) - 1;
	VidEnv env;
	env.init(argc2, argv2);
	env.run();

	astring strLoggerPath = env.GetAppConfPath("logs");

	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "vidstorlog";
	Debug::init(9100, strLoggerFile);

	Debug::logger().info("vidstor started");
	//Debug::logger().info("vidstor started {} {}", __LINE__, __FUNCTION__);
	//Debug::logger().info("vidstor started {} {}", __LINE__, __FUNCTION__);

    	pFactory = new Factory(env);
	gFactory = pFactory;

	pFactory->Init();
	QApplication a(argc1, argv1);

#if 0
	astring strIdTest = "c9d03a04-6ff0-4733-a5bc-6be41876f080";
	VidCamera camTest;
	pFactory->GetConfDB().GetCameraConf(strIdTest, camTest);
#endif
	
	gAppdir = env.GetAppDir();
	
	astring docRoot = env.GetAppDir()+ "www";
	astring strPasswdPath = env.GetAppDir() + ".htpasswd";

	/* set htpasswd, when start a  */
	astring strPasswd = "admin";
	pFactory->GetAdminPasswd(strPasswd);
	WebServerUserChangeNotify(NULL, "admin", strPasswd);
	
	const char *options[] = {
		"document_root", docRoot.c_str(), 
		"listening_ports", PORT, 
		"global_auth_file", strPasswdPath.c_str(),
		"authentication_domain", "rapidvms.com",
		0};
    
	std::vector<std::string> cpp_options;
	for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
	    cpp_options.push_back(options[i]);
	}
	
	/* Start RTSP server */
	VRTSPServer *pRTSPServer = new VRTSPServer(*pFactory);

	/* Init Event Server */
	VEventServer *pEventServer = new VEventServer(*pFactory);
	pEventServer->Init();

	/* Init Event framework */
	VEventMgr *pEventMgr = new VEventMgr(*pFactory, *pEventServer);
	pEventMgr->Init();

	VEWebServer *  pWebServer = new VEWebServer(cpp_options, *pFactory, *pEventServer);
	
	pFactory->RegUserChangeNotify(pWebServer, WebServerUserChangeNotify);

	/* Start the OpenCVR api server */
	pOAPIServer = new OAPIServerWrapper(*pFactory, *pEventServer);
	pOAPIServer->start();

	pFactory->start();
#if 0
	VONVIFDisMgr *pDisMgr = new VONVIFDisMgr();
	
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();

	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin();i!=hostAddr.end();i++)
	{
		pDisMgr->AddHost(((*i).toString()).toStdString(), "9081", "Network_Video_Storage");
	}
#endif
	VDC_DEBUG("Start successfully !\n");
	
	return a.exec();
}
