#include "vscrecorderconf.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCRecorderConf::VSCRecorderConf(RecorderMap &pMap, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	int i = 0;
	RecorderMap::iterator it = pMap.begin(); 
	for(; it!= pMap.end(); ++it)
	{
		ui.gridLayout->addWidget(new VSCRecorderOne((*it).second), i /3,  i%3);
		i ++;
	}
}




void VSCRecorderConf::applyConfig()
{

	return;

}


