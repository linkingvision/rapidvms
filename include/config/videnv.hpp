#ifndef _VID_ENV_H_
#define _VID_ENV_H_

#include "utility.hpp"
#include "conf.hpp"
#include "debug.hpp"

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include <Poco/AutoPtr.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Format.h>
#include <Poco/Util/Application.h>
#include <Poco/Environment.h>

using namespace UtilityLib;
using namespace XSDK;
using namespace Poco::Util;

#ifdef _WIN32
#define VID_PATH_SPLIT "\\"
#else
#define VID_PATH_SPLIT "\/"
#endif


//XGuard guard(m_cMutex);
class VidEnv : public Application
{
public:
	VidEnv()
	:m_sys(new Poco::Util::SystemConfiguration)
	{
		m_syshomedir = m_sys->getString("system.homeDir");
		printf("Home dir --> %s\n", m_syshomedir.c_str());
	}
	~VidEnv(){}
protected:
	void initialize(Application& self)
	{
		//Application::initialize(self);
		// add your own initialization code here
	}
	
	void uninitialize()
	{
		// add your own uninitialization code here
		//Application::uninitialize();
	}
	
	void reinitialize(Application& self)
	{
		//Application::reinitialize(self);
		// add your own reinitialization code here
	}

	void defineOptions(OptionSet& options)
	{

		//Application::defineOptions(options);
#if 0
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
				.required(false)
				.repeatable(true)
				.argument("name=value")
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleDefine)));
				
		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleConfig)));

		options.addOption(
			Option("bind", "b", "bind option value to test.property")
				.required(false)
				.repeatable(false)
				.argument("value")
				.binding("test.property"));
#endif
	}
	int main(const ArgVec& args)
	{
		
		m_appdir = this->config().getString("application.dir");
		printf("Application dir --> %s\n", m_appdir.c_str());
		Init();
		return Application::EXIT_OK;	
	}
	void handleOption(const std::string& name, const std::string& value)
	{
	}
public:
	bool Init()
	{
#ifndef _WIN32
		Poco::Environment::set("QT_QPA_FONTDIR", GetAppDir() + "/lib/fonts/");
		Poco::Environment::set("QT_QPA_PLATFORM_PLUGIN_PATH", GetAppDir() + "/plugins/platforms/");
#endif
		//PrintEnv();
		return true;
	}
	
	
	astring GetAppDir()
	{
		return m_appdir + VID_PATH_SPLIT;
	}
	astring GetConfDir()
	{
		return m_syshomedir + VID_PATH_SPLIT;
	}

	astring GetAppConfPath(astring strApp)
	{
#ifdef _WIN32
#ifndef _WIN64
		astring strPath = "vidstor\\" + strApp + "\\";
#else
		astring strPath = "vidstor64\\" + strApp + "\\";
#endif
#else
		astring strPath = "vidstor/" + strApp + "/";
#endif
		 return GetConfDir() + strPath;
	}
	
	bool PrintEnv()
	{
		{
			/* application */
			const char* const properties[] = { "application.path"
				, "application.name"
				, "application.baseName"
				, "application.dir"
				, "application.configDir"
			};

			std::size_t maxLength = 0;
			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::size_t len = std::string(properties[i]).length();
				maxLength = (maxLength < len) ? len : maxLength;
			}

			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::string property(properties[i]);

				std::string padding(" ");
				std::size_t paddingCount = maxLength - property.length();
				while (0 != paddingCount)
				{
					padding += " ";
					--paddingCount;
				}
				printf("properties[i] %s--> %s\n", properties[i], this->config().getString(property).c_str());
			}
		}

		{
			/* system */
			const char* const properties[] = { "system.osName"
				, "system.osVersion"
				, "system.osArchitecture"
				, "system.nodeName"
				, "system.currentDir"
				, "system.homeDir"
				, "system.tempDir"
				, "system.dateTime"
				, "system.pid"
		#if defined(POCO_OS_FAMILY_WINDOWS)
				, "system.env.HOMEPATH"
		#else
				, "system.env.HOME"
		#endif
			};

			std::size_t maxLength = 0;
			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::size_t len = std::string(properties[i]).length();
				maxLength = (maxLength < len) ? len : maxLength;
			}

			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::string property(properties[i]);

				std::string padding(" ");
				std::size_t paddingCount = maxLength - property.length();
				while (0 != paddingCount)
				{
					padding += " ";
					--paddingCount;
				}
				printf("properties[i] %s--> %s\n", properties[i], m_sys->getString(property).c_str());
			}
		}
		return true;
	}

private:
	//Poco::Util::Application *m_app;
	Poco::Util::SystemConfiguration *m_sys;
	astring m_appdir;
	astring m_syshomedir;
};

typedef VidEnv* LPVidEnv;



#endif /* _VID_ENV_H_ */
