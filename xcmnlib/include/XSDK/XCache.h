
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_Cache_h
#define XSDK_Cache_h

#include <list>
#include "XSDK/XHash.h"
#include "XSDK/XRef.h"
#include "XSDK/IXIterate.h"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"

namespace XSDK
{

/// XCache implements a very simple least recently used cache (discards least recently used
/// items first). When you create an XCache instance, you specify the maximum number of items
/// you'd like stored in the cache. Upon initial insertion an item is considered to be the
/// most recently used. As other elements of the cache are accessed (via Get()) this item
/// becomes relatively older. Eventually, if the item is never accessed a new item being
/// added will cause this item to be destroyed and removed from the cache.
///
/// Note: This class is NOT threadsafe, either provide your own locking or use TSCache<>
/// define below if you need thread safety.

template<class T>
class XCache : public XSDK::IXIterate<T>
{
private:
    struct node
    {
        T data;
        typename std::list<XRef<struct node> >::iterator pos;
        XSDK::XString key;
    };

public:
    X_API XCache( size_t cacheSize ) :
                  _cacheSize( cacheSize ),
                  _itemsInCache( 0 ),
                  _storage(),
                  _index()
    {
    }

    X_API ~XCache() throw()
    {
    }

    X_API inline bool IsCached( const XSDK::XString& key ) const
    {
        return (_index.Find( key )) ? true : false;
    }

    X_API bool Get( const XSDK::XString& key, T& data )
    {
        if( !IsCached( key ) )
            return false;

        XRef<struct node> cn = *_index.Find( key );

        _storage.erase( cn->pos );
        _storage.push_front( cn );
        _storage.front()->pos = _storage.begin();

        data = cn->data;

        return true;
    }

    X_API void Put( const XSDK::XString& key, const T& data )
    {
        // If the item is in the cache already just update the data, otherwise create a new slot.
        if( IsCached( key ) )
        {
            XRef<struct node> cn = *_index.Find( key );

            cn->data = data;

            _storage.erase( cn->pos );
            _storage.push_front( cn );
            _storage.front()->pos = _storage.begin();
        }
        else
        {
            if( _Full() )
            {
                XRef<struct node> cn = _storage.back();
                _storage.pop_back();
                _index.Remove( cn->key );
                _itemsInCache--;
            }

            XRef<struct node> cn = new struct node;
            cn->data = data;
            cn->key = key;

            _storage.push_front( cn );
            _storage.front()->pos = _storage.begin();

            _index.Add( key, cn );
            _itemsInCache++;
        }
    }

    X_API void Drop( const XSDK::XString& key )
    {
        if( IsCached( key ) )
        {
            XRef<struct node> cn = *_index.Find( key );

            _storage.erase( cn->pos );

            _index.Remove( key );

            _itemsInCache--;
        }
    }

    X_API virtual size_t GetElemCount() const
    {
        return _index.GetElemCount();
    }

    X_API virtual void IterStart( bool forward = true )
    {
        _index.IterStart( forward );
    }

    X_API virtual void Iterate()
    {
        _index.Iterate();
    }

    X_API virtual XString IterKey() const
    {
        return _index.IterKey();
    }

    X_API virtual T* IterData() const
    {
        return &((*_index.IterData())->data);
    }

    X_API virtual bool IterValid() const
    {
        return _index.IterValid();
    }

private:

    inline bool _Full() const
    {
        return (_itemsInCache >= _cacheSize) ? true : false;
    }

    XCache( const XCache& obj );
    XCache& operator = ( const XCache& obj );

    size_t _cacheSize;
    size_t _itemsInCache;
    std::list<XRef<struct node> > _storage;
    XSDK::XHash<XRef<struct node> > _index;
};

template<class T>
class XTSCache
{
public:
    X_API XTSCache( size_t cacheSize ) :
        _lok(),
        _cache( cacheSize )
    {
    }

    X_API ~XTSCache() throw()
    {
    }

    X_API inline bool IsCached( const XSDK::XString& key ) const
    {
        XGuard g( _lok );
        return _cache.IsCached( key );
    }

    X_API bool Get( const XSDK::XString& key, T& data )
    {
        XGuard g( _lok );
        return _cache.Get( key, data );
    }

    X_API void Put( const XSDK::XString& key, const T& data )
    {
        XGuard g( _lok );
        _cache.Put( key, data );
    }

    X_API void Drop( const XSDK::XString& key )
    {
        XGuard g( _lok );
        _cache.Drop( key );
    }

private:
    mutable XSDK::XMutex _lok;
    XSDK::XCache<T> _cache;
};

}

#endif
