
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XHash_h
#define XSDK_XHash_h

#include "XSDK/XException.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "XSDK/IXIterate.h"
#include "XSDK/Errors.h"
#include "XSDK/XString.h"
#include "XSDK/XInternalList.h"
#include "XSDK/XString.h"

/// XHash implements a chaining, automatically resizing associative array.
/// In addition, XHash is fast, providing O(1) amortized time lookups,
/// insertions and deletions for best case keys, and at worst O(N) lookups for
/// pathological cases. XHash is tightly integrated with the rest of the
/// XSDK library, and uses XStrings as keys internally (though the
/// API uses XSDK::XString, so some conversions take place).

class XHashTest; // forward declaration needed so unit tests can get friendly with our privates...

namespace XSDK
{

#ifdef WIN32
#pragma warning(disable:4355)
#endif

template<class T>
class XHash : public XSDK::IXIterate<T>
{
    friend class ::XHashTest;

private:

    class XHashList
    {
        friend class XHash;

    public:

        /// Constructor. Initializes minimal internal data structures to nil.
        XHashList() :
            data(),
            key( "" ),
            next( NULL )
        {
        }

        /// Copy constructor. Sets this object to passed object.
        XHashList( const XHashList& obj )
        {
            _Copy(obj);
        }

        virtual ~XHashList() throw()
        {
        }

        /// Assigns this object to passed object.
        XHashList& operator = ( const XHashList& obj )
        {
            return _Copy(obj);
        }

    private:

        /// Make our object match passed object.
        XHashList& _Copy( const XHashList& obj )
        {
            data = obj.data;
            key = obj.key;
            next = obj.next;

            return *this;
        }

        T data;
        XString key;
        XHashList* next;
    };

public:

    class XHashIter : public IXIterate<T>
    {
        friend class XHash;
    public:
        X_API XHashIter() :
            _pStorage(),
            _keyListIter()
        {
        }

        X_API XHashIter(const XHashIter& other) :
            _pStorage(other._pStorage),
            _keyListIter(other._keyListIter)
        {}

        X_API XHashIter& operator =(const XHashIter& other)
        {
            _pStorage = other._pStorage;
            _keyListIter = other._keyListIter;
            return *this;
        }
        X_API virtual ~XHashIter() throw()
        {}

        /// Returns the current number of elements in the list.
        X_API virtual size_t GetElemCount() const
        {
            return _pStorage->GetElemCount();
        }

        /// Reset our iterator to the head of the list.
        X_API virtual void IterStart(bool forward = true)
        {
            _keyListIter.IterStart(forward);
        }

        /// Advance our iterator to the next element in the list.
        X_API virtual void Iterate()
        {
            _keyListIter.Iterate();
        }

        /// Get the key _currently pointed to by the iterator.
        X_API virtual XString IterKey() const
        {
            return _keyListIter.IterData()->c_str();
        }

        /// Get the data (T*) _currently pointed to by the iterator.
        X_API virtual T* IterData() const
        {
            return _pStorage->_Find(*_keyListIter.IterData());
        }

        /// Returns whether or not the iterator is currently pointing at a valid
        /// object, or has been incremented off the end of the list.
        X_API virtual bool IterValid() const
        {
            return _keyListIter.IterValid();
        }

    private:

        X_API XHashIter(const XHash* hash, bool forward) :
            _pStorage(hash),
            _keyListIter(hash->_keyList.GetIterator(forward))
        {}

        const XHash* _pStorage;
        typename XInternalList<XString>::InternalListIter _keyListIter;
    };

    friend class XHashIter;


    enum XHashEnum
    {
        HASH_DJB2,
        HASH_PJW
    };

    typedef int (*XHashFunction)( size_t entryCount, const char* key, size_t* index );
    typedef int (*KeyComp)(const XString& a, const XString& b, void* compareContext );

    /// Constructor. Initializes table to nil state.
    /// \param buckets Number of buckets initially in table. Optional.
    /// \param hash_type The kind of function to use when hashing a key.
    X_API XHash(KeyComp keyCompFunc = NULL, void* compareContext = NULL, size_t buckets = 20, int hash_type = HASH_DJB2) :
        _keyList(keyCompFunc, compareContext),
        _iter(this, true),
        _table( NULL ),
        _hashFunc( (hash_type == HASH_DJB2) ? _hashDJB2 : _hashPJW ),
        _entryCount( buckets ),
        _entrysAdded( 0 )
    {
        _table = (XHashList **)
            malloc( sizeof(XHashList*) * _entryCount );

        if( !_table )
            X_THROW(( "libc malloc() failed." ));

        memset( _table, 0, sizeof(XHashList *) * _entryCount );
    }

    /// Copy Constructor. Initialize state to match obj.
    /// \param obj Object we are copying from.
    X_API XHash(const XHash& obj) :
        _keyList(obj._keyList.GetCompFunc(),obj._keyList.GetCompContext()),
        _iter(this, true),
        _table( NULL ),
        _hashFunc( _hashDJB2 ),
        _entryCount( 20 ),
        _entrysAdded( 0 )
    {
        _table = (XHashList **)
            malloc( sizeof(XHashList*) * _entryCount );

        if( !_table )
            X_THROW(( "libc malloc() failed." ));

        memset( _table, 0, sizeof(XHashList *) * _entryCount );

        _Copy(obj);
    }

    /// Destructor. Calls Clear() on table.
    X_API virtual ~XHash() throw()
    {
        for( size_t i = 0; i < _entryCount; i++ )
        {
            while( XHashList * current = _table[i] )
            {
                _table[i] = _table[i]->next;
                delete current;
            }
        }

        if( _table )
            free(_table);
    }

    /// Allow assignment of one hash table to another.
    /// \param obj The object we copy FROM.
    X_API XHash& operator = (const XHash& obj)
    {
        return _Copy(obj);
    }

    /// Add an element to this table given passed key.
    /// \param key The source data supplied to our hashing function.
    /// \param data The object we want associated with the key.
    X_API void Add(const XString& key, const T& data)
    {
        if(!key.empty())
        {
            if( _entrysAdded >= (_entryCount / 2) )
            {
                ResizeTable();
            }

            size_t  newIndex = 0;
            _hashFunc( _entryCount, key.c_str(), &newIndex );

            XHashList* current = _table[newIndex];

            while( current )
            {
                if( current->key == key )
                    X_THROW(( "An object with key [%s] already exists in the table.", key.c_str() ));

                current = current->next;
            }

            /// If we're here, we know their are no more in the list...

            XHashList* newNode = new (std::nothrow) XHashList;

            if( !newNode )
                X_THROW(( "Unable to allocate new XHashList object." ));

            if( newNode )
            {
                newNode->data = data;
                newNode->key = key;

                newNode->next = (_table[newIndex]) ? _table[newIndex] : NULL;

                // prepend...
                _table[newIndex] = newNode;

                if(_keyList.IsSorted())
                    _keyList.InsertSorted(key);
                else
                    _keyList.Append(key);

                ++_entrysAdded;

                return;
            }
        }

        X_THROW(( "XHash keys must have a Length greater than 0." ));
    }

    X_API XHashIter GetIterator(bool forward = true) const
    {
        return XHashIter(this, forward);
    }

    /// Find the object associated with this key.
    /// \param key The key to lookup in our table.
    X_API T* Find(const XString& key) const
    {
        if(key.empty())
            X_THROW(( "XHash keys must have a Length greater than 0." ));

        return _Find(key.c_str());
    }

    /// Find the object associated with this key or throw.
    /// \param key The key to lookup in our table.
    X_API T operator[](const XString& key) const
    {
        T* item = Find( key );

        if( item == NULL )
            X_THROW(( "Key %s not found.", key.c_str() ));

        return *item;
    }

    /// @true Returns true if the list of keys is sorted.
    X_API bool IsSorted() const
    {
        return _keyList.IsSorted();
    }

    /// @brief Returns true if this list is sorted.
    X_API KeyComp GetCompFunc() const
    {
        return _keyList.GetCompFunc();
    }

    X_API void* GetCompContext() const
    {
        return _keyList.GetCompContext();
    }

    /// Remove an object from our table.
    /// \param key The key to lookup in our table.
    X_API void Remove(const XString& key)
    {
        size_t  newIndex = 0;

        if(!key.empty())
        {
            _hashFunc(_entryCount, key.c_str(), &newIndex);

            XHashList* last = NULL;
            XHashList* current = _table[newIndex];

            while( current && (current->key != key) )
            {
                last = current;
                current = current->next;
            }

            if( current && (current->key == key) )
            {
                if( last )
                    last->next = current->next;
                else _table[newIndex] = current->next;

                delete( current );

                _keyList.Remove( key );

                --_entrysAdded;
            }

            return;
        }

        X_THROW(( "XHash keys must have a Length greater than 0." ));
    }

    /// Empty this container.
    X_API virtual void Clear()
    {
        for( size_t i = 0; i < _entryCount; i++ )
        {
            while( XHashList* current = _table[i] )
            {
                _table[i] = _table[i]->next;
                delete current;
            }
        }

        _keyList.Clear();

        if( _table )
            free(_table);

        _table = (XHashList**)
            malloc(sizeof(XHashList *) * _entryCount);

        if( !_table )
            X_THROW(( "Unable to allocate new minimum table." ));

        _entryCount = 20;
        _entrysAdded = 0;

        memset(_table, 0, sizeof(XHashList *) * _entryCount);
    }

    /// @brief Whether the XHash is empty.
    X_API virtual bool Empty() const
    {
        return _keyList.Empty();
    }

    /// Returns the number of items stored in the hash...
    X_API virtual size_t GetElemCount() const
    {
        return _keyList.GetElemCount();
    }

    /// Reset our internal iterator to the head, or beginning of the set.
    X_API virtual void IterStart(bool forward = true)
    {
        _iter.IterStart(forward);
    }

    /// Move our iterator to the next item contained in this object.
    X_API virtual void Iterate()
    {
        _iter.Iterate();
    }

    /// Get the key _currently pointed to by the internal iterator.
    X_API virtual XString IterKey() const
    {
        return _iter.IterKey();
    }

    /// Return a pointer to the object currently pointed to by our internal
    /// iterator.
    X_API virtual T* IterData() const
    {
        return _iter.IterData();
    }

    /// Returns whether or not the internal iterator is pointing at something
    /// valid, or has walked off the end of the set.
    X_API virtual bool IterValid() const
    {
        return _iter.IterValid();
    }

private:

    /// Internal function. Copy's the passed hash table into our hash table.
    /// \param obj The object we copy FROM.
    XHash& _Copy(const XHash& obj)
    {
        Clear();

        for(XHashIter iter = obj.GetIterator(); iter.IterValid(); iter.Iterate())
        {
            const XString key = iter.IterKey();
            Add(key, *obj.Find(key));
        }

        return *this;
    }

    /// Find the object associated with this key.
    /// \param key The key to lookup in our table.
    X_API T* _Find(const XString& key) const
    {
        size_t newIndex = 0;
        XHashList* current = NULL;

        if(!key.empty())
        {
            _hashFunc( _entryCount, key.c_str(), &newIndex );

            current = _table[newIndex];

            while( current && (current->key != key) )
                current = current->next;

            return (current) ? &current->data : NULL;
        }

        X_THROW(( "XHash keys must have a Length greater than 0." ));
    }

    /// Creates a new table twice the size of the old table, and re-inserts all
    /// of the data into the new table.
    int ResizeTable()
    {
        size_t newEntryCount = _entryCount * 2;
        XHashList** newTable = (XHashList**)
            malloc(sizeof(XHashList*) * newEntryCount);
        memset(newTable, 0, sizeof(XHashList *) * newEntryCount);

        if( newTable )
        {
            for( size_t i = 0; i < _entryCount; i++ )
            {
                while( XHashList* current = _table[i] )
                {
                    XHashList* newNode = new (std::nothrow) XHashList;
                    if( newNode )
                    {
                        newNode->data = current->data;
                        newNode->key = current->key;

                        size_t newIndex = 0;
                        _hashFunc(newEntryCount, current->key.c_str(), &newIndex);

                        newNode->next = (newTable[newIndex])
                            ? newTable[newIndex]
                            : NULL;

                        // prepend...
                        newTable[newIndex] = newNode;
                    }
                    else
                        return -1;

                    XHashList* tmp = _table[i];
                    _table[i] = _table[i]->next;
                    delete tmp;
                }
            }

            free(_table);

            _entryCount = newEntryCount;
            _table = newTable;

            return 0;
        }
        return -1;
    }

    /// Peter J. Weinberger's hash function.
    static int _hashPJW(size_t entryCount, const char* key, size_t* index)
    {
        unsigned g = 0, i, h;
        for( i = 0, h = (unsigned)entryCount; key[i] != '\0'; i++ )
        {
            h = (h << 4) + key[i];
            if( (g = (h && 0xf0000000)) != 0 )
            {
                h ^= g >> 24;
                h ^= g;
            }
        }
        *index = h % entryCount;
        return 0;
    }

    /// An algorithm produced by Daniel J. Bernstein.
    static int _hashDJB2(size_t entryCount, const char* key, size_t* index)
    {
        size_t hash = 5381;
        int c = 0;
        while( (c = *key++) != 0 )
            hash = ((hash << 5) + hash) + c;
        *index = hash % entryCount;
        return 0;
    }


    XInternalList<XString> _keyList;
    XHashIter _iter;
    XHashList** _table;
    XHashFunction _hashFunc;
    size_t _entryCount;
    size_t _entrysAdded;
};

}

#undef BUILDING_XSDK

#endif
