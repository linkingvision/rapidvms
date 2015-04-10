
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XTaskBase_h
#define XSDK_XTaskBase_h

#include "XSDK/Platform.h"
#include "XSDK/Types.h"

#ifdef IS_LINUX
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#else
#ifdef IS_64BIT
#include <intrin.h>
#endif
#endif

#include <list>
#include <map>
#include <vector>

#include "XSDK/Errors.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XCondition.h"
#include "XSDK/XException.h"
#include "XSDK/XWeakRef.h"
#include "XSDK/XMutex.h"

namespace XSDK
{

class XSwimmingThread;
class XSystemThread;
class XTaskBase;
class XThreadGuts;
class XThreadPool;


class XThreadPool : public XBaseObject
{
    class PseudoThread;

    friend class PseudoThread;
    friend class XSwimmingThread;
    friend class XSystemThread;
    friend class XTaskBase;
    friend class XThreadGuts;

public:
    enum { DEFAULT_MAX_THREADS = 5 };

    explicit X_API XThreadPool(int maxThreads = DEFAULT_MAX_THREADS, bool blocking = false);
    X_API virtual ~XThreadPool();
    X_API static XIRef<XThreadPool> GetMainPool();

private:
    X_API XThreadPool(const XThreadPool&);
    X_API XThreadPool& operator = (const XThreadPool&);

    void Start(XIRef<XSwimmingThread> swimmer);
    void CancelThread(XIRef<XSwimmingThread> swimmer);
    void ReturnThread( XIRef<PseudoThread> thread);
    void SwimFree(XIRef<XSwimmingThread> swimmer);

    static XIRef<XThreadPool> _mainPool;
    static XSDK::XMutex _mainPoolLock;


    class PseudoThread : public XBaseObject
    {
    public:
        PseudoThread(XThreadPool* parent);
        ~PseudoThread();

        static void* StartSelf(void* context);
        void StartSwimmer(XIRef<XSwimmingThread> swimmer);
        void CancelThread();

        void ReturnThread();
        void CompleteSwimmingThread();
        void ClearSwimmingThread();
        void RunSwimmer();

        XThreadPool*     _parent;
        XSDK::XMutex     _lock;
        XSDK::XCondition _cond;
        XSDK::XMutex     _swimmerLock;
        XIRef<XSwimmingThread> _swimmer;

        bool _shutdown;

#ifdef IS_LINUX
        pthread_t _thread;
#elif defined(IS_WINDOWS)
        HANDLE    _thread;
#else
        error ">> Unknown OS!"
#endif
    };
};

static const XString DEFAULT_THREAD_NAME = "defaulty";

class XTaskBase
{
    friend class XSystemThread;
    friend class XSwimmingThread;
    friend class XThreadGuts;
    friend class XThreadPool;
    friend class XThreadPool::PseudoThread;

public:
    static const int invalidThreadID = 0;

    /// Creates an XTaskBase which doesn't use a thread pool.
    X_API XTaskBase(const XString& threadName=DEFAULT_THREAD_NAME);

    /// Creates an XTaskBase which uses the given thread pool.
    X_API XTaskBase(XIRef<XThreadPool> pool,const XString& threadName=DEFAULT_THREAD_NAME);

    ///
    X_API virtual ~XTaskBase() throw();

    ///
    X_API virtual void Start();

    ///
    X_API virtual void* Join();

    /// This is where the thread work gets done. Derived classes need to
    /// override this method.
    X_API virtual void* EntryPoint()=0;

    /// Returns the ID of this thread.
    X_API int ThreadID() const { return _threadID; }

    /// Whether the thread is currently running.
    X_API bool IsRunning() const;

    /// @note This method should <b>not</b> be called in normal shutdown circumstances.
    X_API void CancelThread();

    /// @note Do _not_ delete or free returned pointer.
    static X_API XTaskBase* CurrThread();

    /// Returns the ID of the current thread. The difference between
    /// this and calling ThreadID() on the XTaskBase returned from CurrThread()
    /// is that this works even when the current thread was not started
    /// with XTaskBase.
    static X_API int CurrThreadID();

   // Returns the CPU ID assigned to this thread
#ifdef IS_LINUX
    static X_API int GetCPU() { return sched_getcpu(); }

#elif defined(IS_WINDOWS)

#ifdef IS_64BIT

    static X_API DWORD GetCPU()
    {
        int CPUInfo[4];
        __cpuid( CPUInfo, 1 );

        return (unsigned)CPUInfo[1] >> 24;
    }

#else

    //This also works for WindowsXP, since Vista and above provide an API call.
    static X_API DWORD GetCPU() {   _asm {mov eax, 1}
                                    _asm {cpuid}
                                    _asm {shr ebx, 24}
                                    _asm {mov eax, ebx}
                                }
#endif

#endif

protected:

    void _InitID();

    /// Called before EntryPoint(). This allows derived classes the opportunity
    /// to do special work right as the thread starts.
    /// @note This is <b>not</b> intended for the normal use-case.
    X_API virtual void _Begin() {}

    /// Called after EntryPoint() returns. This allows derived classes
    /// the opportunity to do special work right before the thread goes away.
    /// @note This is <b>not</b> intended for the normal use-case.
    /// @param retval The value returned by EntryPoint().
    X_API virtual void _End(void* /*retval*/) {}

private:

    X_API XTaskBase(XIRef<XThreadGuts> guts);

    static void* _Start(void* context);

    X_API XTaskBase(const XTaskBase&);
    X_API XTaskBase& operator = (const XTaskBase&);

    X_API void Finish();

    int _threadID;
    XIRef<XThreadGuts> _threadGuts;
    XString _threadName;

protected:

#ifdef IS_LINUX
    static pthread_key_t _tlsKey;
#elif defined(IS_WINDOWS)
    static DWORD _tlsKey;
#else
    error ">> Unknown OS!"
#endif

    class StaticInit
    {
    public:
        StaticInit();
        ~StaticInit() throw();
    };

    static StaticInit _cStaticInit;
};


class XThreadGuts : public XBaseObject
{
    friend class XTaskBase;
    friend class XThreadPool;
    friend class XThreadPool::PseudoThread;
protected:

    X_API XThreadGuts(XTaskBase* parent);
    virtual X_API ~XThreadGuts() throw() {}

    virtual X_API void Start() = 0;
    virtual X_API void* Join() = 0;
    virtual X_API void CancelThread() = 0;

    virtual X_API bool IsRunning() const;
    virtual X_API void Finish();

    mutable XSDK::XMutex _lock;
    XTaskBase*           _parent;
    void*                _retval;
    volatile bool        _started;
    volatile bool        _running;
    volatile bool        _finished;

private:
    X_API XThreadGuts(const XThreadGuts&);
    X_API XThreadGuts& operator = (const XThreadGuts&);
};


class XSystemThread : public XThreadGuts
{
    friend class XTaskBase;
    friend class XThreadPool;
    friend class XThreadPool::PseudoThread;
protected:

    X_API XSystemThread(XTaskBase* parent);
    virtual X_API ~XSystemThread() throw() {}

    virtual X_API void Start();
    virtual X_API void* Join();
    virtual X_API void CancelThread();

private:
    X_API XSystemThread(const XSystemThread&);
    X_API XSystemThread& operator = (const XSystemThread&);

#ifdef IS_LINUX
    pthread_t _thread;
#elif defined(IS_WINDOWS)
    HANDLE    _thread;
#else
    error ">> Unknown OS!"
#endif
};


class XSwimmingThread : public XThreadGuts
{
    friend class XTaskBase;
    friend class XThreadPool;
    friend class XThreadPool::PseudoThread;
protected:

    X_API XSwimmingThread(XTaskBase* parent, XIRef<XThreadPool> pool);
    virtual X_API ~XSwimmingThread() throw() {}

    virtual X_API void Start();
    virtual X_API void* Join();
    virtual X_API void CancelThread();
    virtual X_API void Complete();

private:
    X_API XSwimmingThread(const XSwimmingThread&);
    X_API XSwimmingThread& operator = (const XSwimmingThread&);

    XSDK::XCondition   _cond;
    XIRef<XThreadPool> _pool;
};

}

#endif
