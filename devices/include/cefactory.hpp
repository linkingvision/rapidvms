

#ifndef __V_CE_FACTORY_H_
#define __V_CE_FACTORY_H_
#include "utility.hpp"
#include "device.hpp"

class CEFactory
{
public:
    CEFactory();
    ~CEFactory();
public:
    void Init();

public:


public:


public:
    void Lock(){m_Lock.lock();}
    void UnLock(){m_Lock.unlock();}

private:
    fast_mutex m_Lock;
};


#endif // __V_CE_FACTORY_H_
