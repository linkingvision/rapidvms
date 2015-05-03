
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_IXIterate_h
#define XSDK_IXIterate_h

namespace XSDK
{

// Unfortunatley, the macro preprocessor doesn't know about C++ namespaces, so
// we must prefix our foreach macros (to avoid possible name collisions with
// other (I.E. boost) foreach macros.
#define x_foreach(a) for( a.IterStart(); a.IterValid(); a.Iterate() )
#define x_foreach_backward(a) for( a.IterStart( false ); a.IterValid(); a.Iterate() )

///
/// IXIterate
/// Defines a simple iteration interface that supports forward and backward
/// collection traversal. For example:
///
///   // Forward iteration example...
///
///   ListBox<int> lb( storage );
///
///   for( lb.IterStart(); lb.IterValid(); lb.Iterate() )
///   {
///       printf( "%d ", *lb.IterData() );
///   }
///
///   // Reverse iteration example...
///
///   for( lb.IterStart( false ); lb.IterValid(); lb.Iterate() )
///   {
///       printf( "%d ", *lb.IterData() );
///   }
///
///   // foreach usage
///
///   foreach(lb)
///   {
///      printf( "%d ", *lb.IterData() );
///   }

template<class T>
class IXIterate
{
public:

    virtual ~IXIterate() throw() {}

    /// Returns the number of T's stored in this collection.
    virtual size_t GetElemCount() const = 0;

    /// Resets the internal iterator to the logical beginning of the collection.
    /// \param forward Determines direction of iteration, defaults to forward.
    virtual void IterStart( bool forward = true ) = 0;

    /// Advances the internal iterator to the next element of the collection.
    virtual void Iterate() = 0;

    /// Returns a pointer to the T stored at the current location of the
    /// iterator.
    virtual T* IterData() const = 0;

    /// Returns true when the iterator currently points at a valid item.
    virtual bool IterValid() const = 0;
};

}

#endif
