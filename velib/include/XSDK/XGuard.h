
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XGuard_h
#define XSDK_XGuard_h

#include "XSDK/XMutex.h"
#include "XSDK/XLogger.h"

namespace XSDK
{

  class XGuard
  {
  public:
    X_API XGuard( XSDK::XMutex& lok );
    X_API virtual ~XGuard() throw();

    X_API void Acquire();
    X_API void Release();

  private:
    XGuard( const XGuard& obj );
    XGuard& operator = ( const XGuard& obj );

    XSDK::XMutex& _lok;
    int _lokCount;
  };

  /**
   * This class is used to provide stack based cleanup of resources, e.g.
   * if you are working with a FILE*, Socket, etc, and you want to clean
   * said item up when the scope is left you can pass a cleanup method
   * and a pointer to the object and this will be run and you are okay!
   */
    template<class T>
    class XCleanerGuard
    {
    public:
        ///This is the prototype for the clean method passed toe the XCleanerGuard
        typedef void (*CleanupMethod)(T obj);

        X_API XCleanerGuard():
            _obj(),
            _method(NULL)
        {
        }

        X_API XCleanerGuard(T obj, CleanupMethod cleanupMethod):
            _obj(obj),
            _method(cleanupMethod)
        {
        }

        X_API void Set(T obj, CleanupMethod cleanupMethod)
        {
            Clear();

            _obj = obj;
            _method = cleanupMethod;
        }

        X_API void Clear()
        {
            if ( _method != NULL )
            {
                _method(_obj);
                _method = NULL;
            }
        }

        X_API virtual ~XCleanerGuard() throw()
        {
            if ( _method == NULL )
            {
                X_LOG_ERROR("The cleanup function was NULL, so we are now not doing cleanup as requested!");
                return;
            }
            //Put the method in a try/catch block to prevent throwing in a destructor.
            try
            {//Not do or do not there is no try.....
                _method(_obj);
            }catch(std::exception& e)
            {
                X_LOG_ERROR("Failed to cleanup correctly! Exception: %s",e.what());
            }catch(...)
            {
                X_LOG_ERROR("Failed to cleanup correctly! Exception: occured but is of unknown type");
            }
        }

    private:
        XCleanerGuard(const XCleanerGuard& obj);
        XCleanerGuard& operator=(const XCleanerGuard& obj);
        ///Our object type
        T _obj;
        ///The method called a and passed _obj in the destructor for our class
        CleanupMethod _method;
    };
}

#endif
