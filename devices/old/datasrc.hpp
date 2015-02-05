#ifndef __V_DATA_SRC_H_
#define __V_DATA_SRC_H_
#include "vpoco.hpp"
#include "datasink.hpp"


class  BasicSrc
{
public:
    virtual void handleSink(SinkNotification* pNf)
    {
        std::cout<<"handleBase: "<<pNf->name()<<std::endl;
        Debug::DebugPrint("ID %d\n", pNf->nId);
        pNf->release();
    }
	
public:
	
public:
	NotificationCenter mNc;
};

#if 0
    nc.addObserver(
        Observer<Target,BaseNotification>(target,&Target::handleBase)
    );
    nc.addObserver(
        NObserver<Target,SubNotification>(target,&Target::handleSub)
    );
    nc.postNotification(new BaseNotification);
    nc.postNotification(new SubNotification);
    nc.removeObserver(
        Observer<Target,BaseNotification>(target,&Target::handleBase)
    );
    nc.removeObserver(
        NObserver<Target,SubNotification>(target,&Target::handleSub)
    );
#endif

#endif /* __V_DATA_SRC_H_ */