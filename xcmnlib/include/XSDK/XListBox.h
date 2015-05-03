
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XListBox_h
#define XSDK_XListBox_h

#include <algorithm>
#include <list>

#include "XSDK/XException.h"

#include "XSDK/IXIterate.h"
#include "XSDK/Errors.h"
#include "XSDK/OS.h"

class XListBoxTest;

namespace XSDK
{

#ifdef WIN32
#pragma warning(disable:4355)
#endif

template<class T>
class XListBox : public IXIterate<T>
{
public:
    typedef int (*SUCompare)(const T& a, const T& b, void* compareContext );

    class ListBoxIter : public IXIterate<T>
    {
        friend class XListBox;
        friend class ::XListBoxTest;
    public:
        X_API ListBoxIter(const ListBoxIter& other) :
            _currIter(other._currIter),
            _currRevIter(other._currRevIter),
            _pStorage(other._pStorage),
            _forward(other._forward)

        {}

        X_API ListBoxIter& operator =(const ListBoxIter& other)
        {
            _currIter = other._currIter;
            _currRevIter = other._currRevIter;
            _pStorage = other._pStorage;
            _forward = other._forward;

            return *this;
        }

        X_API virtual ~ListBoxIter() throw()
        {}


        /// Returns the current number of elements in the list.
        X_API virtual size_t GetElemCount() const
        {
            return _pStorage->GetElemCount();
        }

        /// Reset our iterator to the head of the list.
        X_API virtual void IterStart(bool forward = true)
        {
            _forward = forward;

            if(_forward)
                _currIter = _pStorage->_storage.begin();
            else
                _currRevIter = _pStorage->_storage.rbegin();
        }

        /// Advance our iterator to the next element in the list.
        X_API virtual void Iterate()
        {
            if(_forward)
                ++_currIter;
            else
                ++_currRevIter;
        }

        /// Get the data (T*) _currently pointed to by the iterator.
        X_API virtual T* IterData() const
        {
            return _forward ? &(*_currIter) : &(*_currRevIter);
        }

        /// Returns whether or not the iterator is currently pointing at a valid
        /// object, or has been incremented off the end of the list.
        X_API virtual bool IterValid() const
        {
            return _forward ? _currIter != _pStorage->_storage.end() : _currRevIter != _pStorage->_storage.rend();
        }

    private:

        X_API ListBoxIter(const XListBox* listBox, bool forward) :
            _currIter(),
            _currRevIter(),
            _pStorage(listBox),
            _forward(forward)
        {
            IterStart(forward);
        }

        typename std::list<T>::iterator _currIter;
        typename std::list<T>::reverse_iterator _currRevIter;
        const XListBox* _pStorage;
        bool   _forward;
    };

    friend class ListBoxIter;

    X_API XListBox(std::list<T>& storage, SUCompare su = NULL, void* compareContext = NULL ) :
        _storage(storage),
        _iter(this, true),
        _compFunc(su),
        _compareContext( compareContext )
    {}

    X_API XListBox(const XListBox& obj) :
        _storage(obj._storage),
        _iter(this, true),
        _compFunc(obj._compFunc),
        _compareContext( obj._compareContext )
    {}

    X_API virtual ~XListBox() throw()
    {
    }

    X_API XListBox& operator = ( const XListBox& obj )
    {
        _storage = obj._storage;
        _iter.IterStart(obj._iter._forward);
        _compFunc = obj._compFunc;
        _compareContext = obj._compareContext;

        return *this;
    }

    X_API XListBox& operator = ( const std::list<T>& storage )
    {
        _storage = storage;
        _iter.IterStart();
        _compFunc = NULL;
        _compareContext = NULL;

        return *this;
    }


    X_API void Append( const T& obj )
    {
        if(_compFunc)
            X_THROW(("XSDK::XListBox::Append() Not Supported: The list is sorted."));

        _storage.push_back(obj);
    }

    X_API void AppendList(const XListBox<T>& obj)
    {
        if(_compFunc)
            X_THROW(("XSDK::XListBox::AppendList() Not Supported: The list is sorted."));

        _storage.insert(_storage.end(), obj._storage.begin(), obj._storage.end());
    }

    X_API void SubList(size_t index, size_t count)
    {
        std::list<T> tmp;

        typename std::list<T>::iterator iter = _storage.begin();
        const typename std::list<T>::iterator end = _storage.end();

        for(size_t i = 0; i < index; ++i)
            ++iter;

        for(size_t i = 0; iter != end && i < count; ++i, ++iter)
            tmp.push_back(*iter);

        _storage.swap(tmp);
    }

    X_API void Prepend(const T& obj)
    {
        if(_compFunc)
            X_THROW(("XSDK::XListBox::Prepend() Not Supported: The list is sorted."));

        _storage.push_front( obj );
    }

    X_API void Push(const T& obj)
    {
        if(_compFunc)
            X_THROW(("XSDK::XListBox::Push() Not Supported: The list is sorted."));

        _storage.push_back( obj );
    }

    X_API T Pop()
    {
        if( !_storage.empty() )
        {
            const T data = _storage.back();

            _storage.pop_back();

            return data;
        }
        else
            X_THROW(( "Cannot call Pop() on an empty XSDK::XListBox." ));
    }

    X_API void InsertBeforeIter(const T& obj)
    {
        InsertBeforeIter(obj, _iter);
    }

    X_API void InsertBeforeIter(const T& obj, ListBoxIter iter)
     {
        if(_compFunc)
            X_THROW(("XSDK::XListBox::InsertBeforeIter() Not Supported: The list is sorted."));

        if(!iter.IterValid())
            X_THROW(("XSDK::XListBox::InsertBeforeIter() Given iterator is invalid."));

        if(iter._pStorage != this)
            X_THROW(("XSDK::XListBox::InsertBeforeIter() Given iterator points to a different container."));

        if(!iter._forward)
            X_THROW(("XSDK::XListBox::InsertBeforeIter() You cannot insert with a reverse iterator."));

        if(iter._forward)
            _storage.insert(iter._currIter, obj);
        else
            _storage.insert(iter._currRevIter.base(), obj);
    }

private:
    class CompFuncGreaterThanFunctor : public std::unary_function<T, bool>
    {
    public:
    CompFuncGreaterThanFunctor(SUCompare compFunc, void* compareContext, const T& value)
            : _compFunc(compFunc),
              _compareContext( compareContext ),
              _value(value)
        {}

        bool operator()(const T& other)
        {
            return _compFunc( other, _value, _compareContext ) > 0;
        }

    private:
        CompFuncGreaterThanFunctor& operator=(const CompFuncGreaterThanFunctor& obj);
        SUCompare _compFunc;
        void* _compareContext;
        const T&  _value;
    };

public:

    X_API void InsertSorted(const T& obj)
    {
        if(!_compFunc)
            X_THROW(("XSDK::XListBox::InsertSorted() Not Supported: The list is not sorted."));

        _storage.insert(x_find_if(_storage.begin(), _storage.end(), CompFuncGreaterThanFunctor(_compFunc, _compareContext, obj)),
                        obj);
    }

    /// Inserts the objects in another list into list via insertion sort.
    /// \param otherList The other list to be inserted into this one.
    X_API void InsertListSorted(const XListBox<T>& otherList)
    {
        if(!_compFunc)
            X_THROW(("XSDK::XListBox::InsertListSorted() Not Supported: The list is not sorted."));

        if(otherList.Empty())
            return;

        if(otherList.IsSorted())
        {
            if(Empty())
                _storage.assign(otherList._storage.begin(), otherList._storage.end());
            else
            {
                typename std::list<T>::iterator iter      = _storage.begin();
                typename std::list<T>::iterator end       = _storage.end();
                typename std::list<T>::iterator otherIter = otherList._storage.begin();
                typename std::list<T>::iterator otherEnd  = otherList._storage.end();

                while(otherIter != otherEnd)
                {
                    //Grab place to insert into.
                    iter = x_find_if(iter, end, CompFuncGreaterThanFunctor(_compFunc, _compareContext, *otherIter));

                    //Grab end of set of values to insert before iter.
                    typename std::list<T>::iterator nextOtherIter = otherIter;
                    ++nextOtherIter;

                    if(iter != end)
                    {
                        nextOtherIter = x_find_if(nextOtherIter, otherEnd, CompFuncGreaterThanFunctor(_compFunc, _compareContext, *iter));
                    }
                    else
                        nextOtherIter = otherEnd;

                    //Insert values.
                    _storage.insert(iter, otherIter, nextOtherIter);

                    otherIter = nextOtherIter;
                }
            }
        }
        else
        {
            for(ListBoxIter iter = otherList.GetIterator();
                iter.IterValid();
                iter.Iterate())
            {
                InsertSorted(*iter.IterData());
            }
        }
    }

    X_API ListBoxIter GetIterator(bool forward = true) const
    {
        return ListBoxIter(this, forward);
    }

    X_API T* Get(size_t index) const
    {
        if(index < GetElemCount())
        {
            ListBoxIter iter(this, true);

            for(size_t i = 0; i < index; ++i)
                iter.Iterate();

            return iter.IterData();
        }
        else
            X_THROW(("XSDK::XInternalList::Get() index out of range."));
    }

    X_API bool Contains(const T& obj) const
    {
        const typename std::list<T>::iterator end  = _storage.end();

        return find(_storage.begin(), end, obj) != end;
    }

    /// @brief Returns true if this list is sorted.
    X_API bool IsSorted() const
    {
        return _compFunc != NULL;
    }

    /// @brief Returns true if this list is sorted.
    X_API SUCompare GetCompFunc() const
    {
        return _compFunc;
    }

    X_API void* GetCompContext() const
    {
        return _compareContext;
    }

    X_API void Remove(const T& obj)
    {
        _storage.remove(obj);
    }

    X_API void RemoveAtIter()
    {
        _iter = RemoveAtIter(_iter);
    }

    X_API ListBoxIter RemoveAtIter(ListBoxIter iter)
    {
        if(!iter.IterValid())
            X_THROW(("XSDK::XListBox::RemoveAtIter() Given iterator is invalid."));

        if(iter._pStorage != this)
            X_THROW(("XSDK::XListBox::RemoveAtIter() Given iterator points to a different container."));

        if(!iter._forward)
            X_THROW(("XSDK::XListBox::RemoveAtIter() You cannot remove with a reverse iterator."));

        if(iter._forward)
            iter._currIter = _storage.erase(iter._currIter);
        else
            _storage.erase((++iter._currRevIter).base());

        return iter;
    }

    X_API virtual void Clear()
    {
        _storage.clear();
    }


    /// @brief Returns whether there are any elements in the list.
    X_API virtual bool Empty() const
    {
        return _storage.empty();
    }

    /// Returns the number of T's stored in this collection.
    X_API virtual size_t GetElemCount() const
    {
        return _storage.size();
    }

    /// Resets the internal iterator to the logical beginning of the collection.
    /// \param forward Determines direction of iteration, defaults to forward.
    X_API virtual void IterStart( bool forward = true )
    {
        _iter.IterStart(forward);
    }

    /// Advances the internal iterator to the next element of the collection.
    X_API virtual void Iterate()
    {
        _iter.Iterate();
    }

    /// Returns a pointer to the T stored at the current location of the iterator.
    X_API virtual T* IterData() const
    {
        return _iter.IterData();
    }

    /// Returns true when the iterator currently points at a valid item.
    X_API virtual bool IterValid() const
    {
        return _iter.IterValid();
    }

 private:
    XListBox();

    std::list<T>& _storage;
    ListBoxIter   _iter;
    SUCompare     _compFunc;
    void*         _compareContext;
};

};

#endif
