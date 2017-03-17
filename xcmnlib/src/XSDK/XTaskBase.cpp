
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XTaskBase.h"
#include "XSDK/Errors.h"
#include "XSDK/XGuard.h"


#ifdef IS_LINUX
#include <sys/syscall.h>
#endif

using namespace std;
using namespace XSDK;

//------------------------------------------------------------------------------
// XTaskBase
//------------------------------------------------------------------------------

#ifdef IS_LINUX
pthread_key_t XTaskBase::_tlsKey;
#elif defined(IS_WINDOWS)
DWORD XTaskBase::_tlsKey = 0;
#else
    #error ">> Unknown OS!"
#endif

pthread_t XTaskBase::invalidThreadID = 0;

XTaskBase::XTaskBase(const XString& threadName)
    : _threadID(0),
      _threadGuts(),
      _threadName(threadName)
{
    if(_threadName.length()>16)
        X_THROW(("XSDK thread names must be 16 characters or less (thread name=%s)",_threadName.c_str()));

    //Windows is stupid and complains about using this in the initializer list
    _threadGuts = new XSystemThread(this);
}

XTaskBase::XTaskBase(XIRef<XThreadPool> pool,const XString& threadName)
    : _threadID(0),
      _threadGuts(),
      _threadName(threadName)
{
    if(_threadName.length()>16)
        X_THROW(("XSDK thread names must be 16 characters or less."));

    //Windows is stupid and complains about using this in the initializer list
    _threadGuts = new XSwimmingThread(this, pool);
}

XTaskBase::~XTaskBase() throw()
{
    try
    {
        Join();
    }
    catch(...) {}
}

XTaskBase* XTaskBase::CurrThread()
{
#ifdef IS_LINUX
    return (XTaskBase*)pthread_getspecific(_tlsKey);
#elif defined(IS_WINDOWS)
    return (XTaskBase*)TlsGetValue(_tlsKey);
#else
    #error ">> Unknown OS!"
#endif
}

int XTaskBase::CurrThreadID()
{
#ifdef IS_LINUX
    const XTaskBase* taskBase = (XTaskBase*)pthread_getspecific(_tlsKey);

    if(taskBase)
        return taskBase->ThreadID();

    return syscall(SYS_gettid);
#elif defined(IS_WINDOWS)
    const XTaskBase* taskBase = (XTaskBase*)TlsGetValue(_tlsKey);

    if(taskBase)
        return taskBase->ThreadID();

    return GetCurrentThreadId();
#else
    #error ">> Unknown OS!"
#endif
}

void XTaskBase::_InitID()
{
#ifdef IS_LINUX
    _threadID = syscall(SYS_gettid);
#elif defined(IS_WINDOWS)
    _threadID = GetCurrentThreadId();
#else
    #error ">> Unknown OS!"
#endif
}

void XTaskBase::Start()
{
    _threadGuts->Start();
}

void* XTaskBase::Join()
{
    return _threadGuts->Join();
}

bool XTaskBase::IsRunning() const
{
    return _threadGuts->IsRunning();
}

void XTaskBase::CancelThread()
{
    _threadGuts->CancelThread();
}

void XTaskBase::Finish()
{
    _threadGuts->Finish();
}

void* XTaskBase::_Start(void* context)
{
    void* retval = 0;

    if(context)
    {
        XTaskBase* taskBase = (XTaskBase*)context;
#ifdef IS_LINUX
        pthread_setspecific(_tlsKey, context);
#ifndef __APPLE__
        prctl( PR_SET_NAME, taskBase->_threadName.c_str(), 0, 0, 0 );
#endif
#elif defined(IS_WINDOWS)
        TlsSetValue(_tlsKey, context);
#else
    #error ">> Unknown OS!"
#endif
        taskBase->_InitID();
        taskBase->_Begin();
        retval = taskBase->EntryPoint();
        taskBase->_End(retval);
        taskBase->Finish();
    }

#ifdef IS_LINUX
#ifndef __APPLE__
    prctl( PR_SET_NAME, "swimmer", 0, 0, 0 );
#endif
    pthread_setspecific(_tlsKey, 0);
#elif defined(IS_WINDOWS)
    TlsSetValue(_tlsKey, 0);
#else
    #error ">> Unknown OS!"
#endif

    return retval;
}


XTaskBase::StaticInit XTaskBase::_cStaticInit;

XTaskBase::StaticInit::StaticInit()
{
#ifdef IS_LINUX
    pthread_key_create(&_tlsKey, 0);
#elif defined(IS_WINDOWS)
    _tlsKey = TlsAlloc();
#else
    #error ">> Unknown OS!"
#endif
}

XTaskBase::StaticInit::~StaticInit() throw()
{
#ifdef IS_LINUX
    pthread_key_delete(_tlsKey);
#elif defined(IS_WINDOWS)
    TlsFree(_tlsKey);
    _tlsKey = 0;
#else
    #error ">> Unknown OS!"
#endif
}


//------------------------------------------------------------------------------
// XThreadPool
//------------------------------------------------------------------------------

XIRef<XThreadPool> XThreadPool::_mainPool;
XMutex XThreadPool::_mainPoolLock;

XThreadPool::XThreadPool(int maxThreads, bool blocking)
{
}
XThreadPool::~XThreadPool()
{
}
XIRef<XThreadPool> XThreadPool::GetMainPool()
{
    {
        XGuard lock(_mainPoolLock);

        if(!_mainPool.IsValid())
            _mainPool = new XThreadPool();
    }

    return _mainPool;
}
void XThreadPool::ReturnThread( XIRef<PseudoThread> thread)
{
}
void XThreadPool::Start(XIRef<XSwimmingThread> swimmer)
{
    SwimFree(swimmer);
}

void XThreadPool::CancelThread(XIRef<XSwimmingThread> swimmer)
{
    X_THROW(("Don't call this!"));
}

void XThreadPool::SwimFree(XIRef<XSwimmingThread> swimmer)
{
    swimmer->_parent->_threadGuts = new XSystemThread(swimmer->_parent);
    swimmer->_parent->_threadGuts->Start();
}


XThreadPool::PseudoThread::PseudoThread(XThreadPool* parent)
    : _parent(parent),
      _lock(),
      _cond(_lock),
      _swimmerLock(),
      _swimmer(),
      _shutdown(false),
      _thread(0)
{
#ifdef IS_LINUX
    int err = pthread_create(&_thread, 0, StartSelf, this);

    if(err < 0)
        X_THROW(("Unable to create thread: Code %d", err));
#elif defined(IS_WINDOWS)
    DWORD id = 0;

    _thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartSelf, this, 0, &id);

    if(!_thread)
        X_THROW(("Unable to create thread: Code %d", GetLastError()));
#else
    #error ">> Unknown OS!"
#endif
}

XThreadPool::PseudoThread::~PseudoThread()
{
    {
        XGuard lock(_lock);
        _shutdown = true;
        _cond.Signal();
        _parent = NULL;
    }

    void* _retval = 0;
#ifdef IS_LINUX
    const int err = pthread_join(_thread, &_retval);

    if(err < 0)
        X_THROW(("Unable to join thread: Code %d", err));
#elif defined(IS_WINDOWS)
    WaitForSingleObject(_thread, INFINITE);
    GetExitCodeThread(_thread, (LPDWORD)&_retval);
    CloseHandle(_thread);
#else
    #error ">> Unknown OS!"
#endif
}

void* XThreadPool::PseudoThread::StartSelf(void* context)
{
    PseudoThread* pseudo = (PseudoThread*)context;
    XGuard lock(pseudo->_lock);

    while(!pseudo->_shutdown)
    {
        pseudo->ReturnThread();
        while(!pseudo->_swimmer.IsValid())
        {
            pseudo->_cond.Wait();

            if(pseudo->_shutdown)
                return 0;
        }
        pseudo->RunSwimmer();
        pseudo->CompleteSwimmingThread();
        pseudo->ClearSwimmingThread();
    }

    return 0;
}
void XThreadPool::PseudoThread::ReturnThread()
{
    if ( _parent != NULL )
        _parent->ReturnThread(this);
}
void XThreadPool::PseudoThread::CompleteSwimmingThread()
{
    XGuard guard(_swimmerLock);
    if ( _swimmer.IsEmpty() )
        return;
    _swimmer->Complete();
}
void XThreadPool::PseudoThread::ClearSwimmingThread()
{
    XGuard guard(_swimmerLock);
    if ( _swimmer.IsEmpty() )
        return;
    _swimmer.Clear();
}
void XThreadPool::PseudoThread::RunSwimmer()
{
    XGuard guard(_swimmerLock);
    if ( _swimmer.IsEmpty() )
        return;
    _swimmer->_retval = XTaskBase::_Start(_swimmer->_parent);
}

void XThreadPool::PseudoThread::StartSwimmer(XIRef<XSwimmingThread> swimmer)
{
    {
        XGuard lock(_lock);
        XGuard guard(_swimmerLock);
        swimmer->_running = true;
        _swimmer = swimmer;
        _cond.Signal();
    }
}

void XThreadPool::PseudoThread::CancelThread()
{
    X_THROW(("Don't call this!"));
}


//------------------------------------------------------------------------------
// XThreadGuts
//------------------------------------------------------------------------------

XThreadGuts::XThreadGuts(XTaskBase* parent)
    : _lock(),
      _parent(parent),
      _retval(0),
      _started(false),
      _running(false),
      _finished(false)
{}

bool XThreadGuts::IsRunning() const
{
    XGuard lock(_lock);
    return _running;
}

void XThreadGuts::Finish()
{
    XGuard lock(_lock);
    _running = false;
    _finished = true;
}


//------------------------------------------------------------------------------
// XSystemThread
//------------------------------------------------------------------------------

XSystemThread::XSystemThread(XTaskBase* parent)
    : XThreadGuts(parent),
      _thread(0)
{}

void XSystemThread::Start()
{
    XGuard lock(_lock);
    if(_running)
        return;

#ifdef IS_LINUX
    int err = pthread_create(&_thread, 0, _parent->_Start, _parent);

    if(err < 0)
        X_THROW(("Unable to create thread: Code %d", err));
#elif defined(IS_WINDOWS)
    DWORD id = 0;

    _thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_parent->_Start, _parent, 0, &id);

    if(!_thread)
        X_THROW(("Unable to create thread: Code %d", GetLastError()));

#else
    #error ">> Unknown OS!"
#endif

    _running = true;
}

void* XSystemThread::Join()
{
    if(_thread != XTaskBase::invalidThreadID)
    {
        _retval = 0;
#ifdef IS_LINUX
        const int err = pthread_join(_thread, &_retval);
        _thread = XTaskBase::invalidThreadID;

        if(err < 0)
            X_THROW(("Unable to join thread: Code %d", err));
#elif defined(IS_WINDOWS)
        WaitForSingleObject(_thread, INFINITE);
        GetExitCodeThread(_thread, (LPDWORD)&_retval);
        CloseHandle(_thread);
        _thread = XTaskBase::invalidThreadID;
#else
    #error ">> Unknown OS!"
#endif
    }

    return _retval;
}

void XSystemThread::CancelThread()
{
    XGuard lock(_lock);

#ifdef IS_LINUX
    pthread_cancel(_thread);
#elif defined(IS_WINDOWS)
    TerminateThread(_thread, 0);
#else
    #error ">> Unknown OS!"
#endif

    _running = false;
}

//------------------------------------------------------------------------------
// XSwimmingThread
//------------------------------------------------------------------------------

XSwimmingThread::XSwimmingThread(XTaskBase* parent, XIRef<XThreadPool> pool)
    : XThreadGuts(parent),
      _cond(_lock),
      _pool(pool)
{}

void XSwimmingThread::Start()
{
    _started = true;
    _finished = false;
    if ( _pool.IsValid() )
      _pool->Start(this);
    else
      {
          _started = false;
          _finished = true;	
      }
}

void* XSwimmingThread::Join()
{
    XGuard lock(_lock);

    if(!_started)
        return 0;

    while(!_finished)
        _cond.Wait();

    _started = false;
    return _retval;
}

void XSwimmingThread::CancelThread()
{
    _pool->CancelThread(this);
}

void XSwimmingThread::Complete()
{
    XGuard guard(_lock);
    Finish();
    _cond.Signal();
}
