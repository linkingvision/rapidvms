
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XInternalList_h
#define XSDK_XInternalList_h

#include "XSDK/XException.h"

#include "XSDK/IXIterate.h"
#include "XSDK/Errors.h"

#include <cstdio>
#include <cstdlib>


class XInternalListTest;

namespace XSDK
{

#ifdef WIN32
#pragma warning(disable:4355)
#endif

template<class T>
class XInternalList : public IXIterate<T>
{
public:
    class InternalListIter;

private:

    class XInternalListNode
    {
        friend class XInternalList;
        friend class InternalListIter;

    public:
        XInternalListNode() :
            data(),
            next(NULL),
            prev(NULL)
        {
        }

    private:
        XInternalListNode( const XInternalListNode& obj );
        XInternalListNode& operator = ( const XInternalListNode& obj );

        T data;
        XInternalListNode* next;
        XInternalListNode* prev;
    };


public:
    typedef int (*SUCompare)( const T& a, const T& b, void* compareContext );

    class InternalListIter : public IXIterate<T>
    {
        friend class XInternalList;
        friend class ::XInternalListTest;
    public:
        X_API InternalListIter() :
            _current(),
            _pStorage(),
            _forward()
        {}
        X_API InternalListIter(const InternalListIter& other) :
            _current(other._current),
            _pStorage(other._pStorage),
            _forward(other._forward)
        {}

        InternalListIter& operator =(const InternalListIter& other)
        {
            _current = other._current;
            _pStorage = other._pStorage;
            _forward = other._forward;

            return *this;
        }

        X_API virtual ~InternalListIter() throw()
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
                _current = _pStorage->_head;
            else
                _current = _pStorage->_tail;
        }

        /// Advance our iterator to the next element in the list.
        X_API virtual void Iterate()
        {
            if(!_current)
                X_THROW(("XSDK::XInternalList::Iterate(): Cannot iterate an invalid iterator."));

            if(_forward)
                _current = _current->next;
            else
                _current = _current->prev;
        }

        /// Get the data (T*) _currently pointed to by the iterator.
        X_API virtual T* IterData() const
        {
            return _current ? &_current->data : NULL;
        }

        /// Returns whether or not the iterator is currently pointing at a valid
        /// object, or has been incremented off the end of the list.
        X_API virtual bool IterValid() const
        {
            return _current != NULL;
        }

    private:

        X_API InternalListIter(const XInternalList* intList, bool forward) :
            _current(forward ? intList->_head : intList->_tail),
            _pStorage(intList),
            _forward(forward)
        {}

        XInternalListNode* _current;
        const XInternalList*     _pStorage;
        bool   _forward;
    };

    friend class InternalListIter;


    /// Constructor. Initializes internal list maintenance variables.
    X_API XInternalList( SUCompare su = NULL, void* compareContext = NULL ) :
        _head( NULL ),
        _tail( NULL ),
        _elemCount(0),
        _iter(this, true),
        _compFunc(su),
        _compareContext( compareContext )
    {
    }

    /// Copy Constructor.
    X_API XInternalList( const XInternalList& obj ) :
        _head( NULL ),
        _tail( NULL ),
        _elemCount(0),
        _iter(this, true),
        _compFunc(NULL),
        _compareContext(NULL)
    {
        _Copy( obj );
    }

    /// Destructor. Calls Clear() on this container.
    X_API virtual ~XInternalList() throw()
    {
        XInternalListNode* current = _head;

        while(current)
        {
            XInternalListNode* last = current;
            current = current->next;
            delete last;
        }
    }

    /// Assignment operator.
    X_API XInternalList& operator = ( const XInternalList& obj )
    {
        return _Copy( obj );
    }

    /// Appends passed "obj" to end of list.
    /// \param obj the object you're trying to append to this list.
    X_API void Append( const T& obj )
    {
        if(_compFunc)
            X_THROW(("XSDK::XInternalList::Append() Not Supported: The list is sorted."));

        _Append(obj);
    }

    /// Append an entire list to the end of the list.
    /// \param obj The XInternalList<T>& we'd like to append.
    X_API void AppendList( const XInternalList<T>& obj )
    {
        if(_compFunc)
            X_THROW(("XSDK::XInternalList::AppendList() Not Supported: The list is sorted."));

        for(InternalListIter iter = obj.GetIterator(); iter.IterValid(); iter.Iterate())
            _Append(*iter.IterData());
    }

    X_API void SubList( size_t index, size_t count )
    {
        XInternalList<T> tmp( _compFunc, _compareContext );

        InternalListIter iter(this, true);
        for(size_t i = 0; i < index; ++i)
            iter.Iterate();

        for(size_t i = 0; iter.IterValid() && i < count; ++i, iter.Iterate())
            tmp._Append(*iter.IterData());

        *this = tmp;
    }

    /// Prepends passed "obj" to front of list.
    /// \param obj the object you're trying to Prepend to this list.
    X_API void Prepend( const T& obj)
    {
        if(_compFunc)
            X_THROW(("XSDK::XInternalList::Prepend() Not Supported: The list is sorted."));

        XInternalListNode* nln = new XInternalListNode;

        if( !nln )
            X_THROW(( "XSDK::XInternalList::Prepend() memory allocation failed." ));

        nln->data = obj;

        if (_head == 0)
        {
            nln->prev = 0;
            nln->next = 0;
            _head = nln;
            _tail = nln;
        }
        else
        {
            _head->prev = nln;
            nln->next = _head;
            nln->prev = 0;
            _head = nln;
        }

        ++_elemCount;
    }

    /// Provides stack like interface.
    /// \param obj the object you're trying to push onto this stack.
    X_API void Push( const T& obj)
    {
        if(_compFunc)
            X_THROW(("XSDK::XInternalList::Push() Not Supported: The list is sorted."));

        Append(obj);
    }

    /// Pop's an object off the tail end of this list.
    X_API T Pop()
    {
        XInternalListNode* end = _tail;

        if( !end )
            X_THROW(( "XSDK::XInternalList::Pop() you just popped an empty list." ));

        const T data = end->data;

        if (end->prev)
        {
            end->prev->next = 0;
            _tail = end->prev;
            delete end;
        }
        else
        {
            _head = 0;
            _tail = 0;
            delete end;
        }

        --_elemCount;

        return data;
    }

    X_API void InsertBeforeIter(const T& obj)
    {
        InsertBeforeIter(obj, _iter);
    }

    /// Inserts object between the iterators current position, and the
    /// position before it.
    /// \param obj the object you're trying to insert.
    X_API void InsertBeforeIter(const T& obj, InternalListIter iter)
    {
        if(_compFunc)
            X_THROW(("XSDK::XInternalList::InsertBeforeIter() Not Supported: The list is sorted."));

        if(!iter.IterValid())
            X_THROW(("XSDK::XInternalList::InsertBeforIter() Given iterator is invalid."));

        if(iter._pStorage != this)
            X_THROW(("XSDK::XInternalList::InsertBeforIter() Given iterator points to a different container."));

        if(!iter._forward)
            X_THROW(("XSDK::XInternalList::InsertBeforeIter() You cannot insert with a reverse iterator."));

        _InsertBeforeIter(obj, iter);
    }

    /// Inserts an object into list via insertion sort.
    /// \param obj the object you're trying to insert sorted.
    X_API void InsertSorted( const T& obj)
    {
        if(!_compFunc)
            X_THROW(("XSDK::XInternalList::InsertSorted() Not Supported: The list is not sorted."));

        InternalListIter iter = GetIterator();

        while(iter.IterValid() && _compFunc( obj, *iter.IterData(), _compareContext ) > 0)
            iter.Iterate();

        if(iter.IterValid())
            _InsertBeforeIter(obj, iter);
        else
            _Append(obj);
    }

    /// Inserts the objects in another list into list via insertion sort.
    /// \param otherList The other list to be inserted into this one.
    X_API void InsertListSorted(const XInternalList<T>& otherList)
    {
        if(!_compFunc)
            X_THROW(("XSDK::XInternalList::InsertListSorted() Not Supported: The list is not sorted."));

        for(InternalListIter iter = otherList.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            InsertSorted(*iter.IterData());
        }
    }

    X_API InternalListIter GetIterator(bool forward = true) const
    {
        return InternalListIter(this, forward);
    }

    X_API T* Get(size_t index) const
    {
        InternalListIter iter = GetIterator();

        if(index < GetElemCount())
        {
            for(size_t i = 0; i < index; ++i)
                iter.Iterate();

            return iter.IterData();
        }
        else X_THROW(("XSDK::XInternalList::Get() index out of range."));
    }

    /// Returns true if the list Contains an object == to the passed object.
    /// \param obj The object your checking to see if the list Contains.
    X_API bool Contains( const T& obj ) const
    {
        for( XInternalListNode* current = _head; current; current = current->next )
            if( current->data == obj )
                return true;

        return false;
    }

    /// @brief Returns true if this list is sorted.
    X_API bool IsSorted() const
    {
        return (_compFunc)?true:false;
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

    /// Removes an object from this list.
    /// \param the object you'd like to remove from this list.
    X_API void Remove( const T& obj )
    {
        XInternalListNode* current_node = _head;

        while (current_node && !(current_node->data == obj))
            current_node = current_node->next;

        if (current_node && current_node->data == obj)
        {
            if (current_node->prev)
                current_node->prev->next = current_node->next;
            else
                _head = _head->next;
            if (current_node->next)
                current_node->next->prev = current_node->prev;
            else
                _tail = _tail->prev;

            delete current_node;

            --_elemCount;

            return;
        }

        X_THROW(( "XSDK::XInternalList::Remove() The object your trying to remove was not found." ));
    }

    X_API void RemoveAtIter()
   {
       _iter = RemoveAtIter(_iter);
   }

    /// Removes the object pointed to by the iterator.
    X_API InternalListIter RemoveAtIter(InternalListIter iter)
    {
        if(!iter.IterValid())
            X_THROW(("XSDK::XInternalList::RemoveAtIter() Given iterator is invalid."));

        if(iter._pStorage != this)
            X_THROW(("XSDK::XInternalList::RemoveAtIter() Given iterator points to a different container."));

        if(!iter._forward)
            X_THROW(("XSDK::XInternalList::RemoveAtIter() You cannot insert with a reverse iterator."));


        if(iter._current->prev)
            iter._current->prev->next = iter._current->next;
        else
            _head = _head->next;
        if(iter._current->next)
            iter._current->next->prev = iter._current->prev;
        else
            _tail = _tail->prev;

        XInternalListNode* tmp = iter._current;

        if(iter._forward)
        {
            if(iter._current->next)
                iter._current = iter._current->next;
            else
                iter._current = NULL;
        }
        else
        {
            if(iter._current->prev)
                iter._current = iter._current->prev;
            else
                iter._current = NULL;
        }

        delete tmp;

        --_elemCount;

        return iter;
    }

    /// Empty the list.
    X_API virtual void Clear()
    {
        InternalListIter iter = GetIterator();

        while(iter.IterValid())
            iter = RemoveAtIter(iter);

        _elemCount = 0;
    }

    /// Reset our iterator to the head of our list.
    X_API virtual void IterStart( bool forward = true )
    {
        _iter.IterStart(forward);
    }

    /// Advance our iterator to the next element in this list.
    X_API virtual void Iterate()
    {
        _iter.Iterate();
    }

    /// Get the data (T*) _currently pointed to by our iterator.
    X_API virtual T* IterData() const
    {
        return _iter.IterData();
    }

    /// Returns whether or not the iterator is _currently pointing at a valid
    /// object, or has been incremented off the end of the list.
    X_API virtual bool IterValid() const
    {
        return _iter.IterValid();
    }

    /// @brief Returns whether the list is empty.
    X_API virtual bool Empty() const
    {
        return _elemCount == 0;
    }

    /// Returns the current number of elements in the list.
    X_API virtual size_t GetElemCount() const
    {
        return _elemCount;
    }

 private:

    /// Makes this object == passed object.
    /// \param obj The object to duplicate.
    XInternalList& _Copy(const XInternalList& obj)
    {
        _compFunc = obj._compFunc;
        _compareContext = obj._compareContext;

        Clear();

        for(InternalListIter iter = obj.GetIterator();
            iter.IterValid();
            iter.Iterate())
        {
            _Append(*iter.IterData());
        }

        _iter.IterStart();

        return *this;
    }

    /// @brief Appends passed "obj" to end of list.
    ///
    /// @note We have both _Append() and Append() so that InsertSorted() can append when
    ///       necessary while anyone using XInternalList trying to append to a sorted list will
    ///       get an exception.
    ///
    /// \param obj the object you're trying to append to this list.
    X_API void _Append( const T& obj )
    {
        XInternalListNode* nln = new XInternalListNode;
        if(!nln)
            X_THROW(("XSDK::XInternalList::_Append() memory allocation failed."));

        nln->data = obj;

        if (_tail == 0)
        {
            nln->prev = 0;
            nln->next = 0;
            _head = nln;
            _tail = nln;
        }
        else
        {
            _tail->next = nln;
            nln->prev = _tail;
            nln->next = 0;
            _tail = nln;
        }

        ++_elemCount;
    }

    /// @brief Inserts object between the iterators current position, and the
    ///        position before it.
    ///
    /// @note We have both _InsertBeforeIter() and InsertBeforeIter() so that
    ///       InsertSorted() can append when necessary while anyone using XInternalList
    ///       trying to insert with an iterator to a sorted list will get an exception.
    ///
    /// \param obj the object you're trying to insert.
    X_API void _InsertBeforeIter(const T& obj, InternalListIter iter)
    {
        XInternalListNode* nln = new XInternalListNode;

        if(!nln)
            X_THROW(("XSDK::XInternalList::InsertBeforeIter() memory allocation failed."));

        nln->data = obj;

        if(iter._current->prev)
        {
            iter._current->prev->next = nln;
            nln->prev = iter._current->prev;
            nln->next = iter._current;
            iter._current->prev = nln;
        }
        else
        {
            iter._current->prev = nln;
            nln->prev = 0;
            nln->next = iter._current;
            _head = nln;
        }

        ++_elemCount;
    }


    XInternalListNode* _head;
    XInternalListNode* _tail;
    size_t _elemCount;
    InternalListIter _iter;
    SUCompare _compFunc;
    void* _compareContext;
};

}

#endif
