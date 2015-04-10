
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XMemory.h"
#include "XSDK/Errors.h"
#include "XSDK/XException.h"
#include "XSDK/OS.h"
#include "XSDK/XGuard.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace XSDK;
using namespace std;


const static size_t MAX = 1073741824;//1GB
const static size_t HALF_MAX = MAX/2;
const static size_t DEFAULT_ALIGNMENT = 16;

XMemory::XMemory( size_t bufferSize ) :
    _bufferSize( bufferSize ),
    _dataSize( 0 ),
    _buffer( NULL )
{
    _ReallocateBuffer( _bufferSize );
}

XMemory::XMemory( const XMemory& obj ) :
    _bufferSize( obj._bufferSize ),
    _dataSize( obj._dataSize ),
    _buffer( NULL )
{
    if ( obj._buffer )
    {
        _ReallocateBuffer( _bufferSize );
        memcpy( _buffer, obj._buffer, _bufferSize );
    }

    ///MPF-948 - _ReleaseBuffer above sets _dataSize to 0
    _dataSize = obj._dataSize;
}

XMemory::~XMemory() throw()
{
    if ( _buffer )
        X_DELETE_ALIGNED_BUFFER( _buffer );
}

XMemory& XMemory::operator = ( const XMemory& obj )
{
    _bufferSize = obj._bufferSize;

    if ( obj._buffer )
    {
        _ReallocateBuffer( _bufferSize );
        memcpy( _buffer, obj._buffer, _bufferSize );
    }

    ///MPF-948 - _ReleaseBuffer above sets _dataSize to 0
    _dataSize = obj._dataSize;

    return *this;
}

uint8_t& XMemory::Extend( size_t size )
{
    if ( _bufferSize <= 0 )
        _bufferSize = DEFAULT_BUFFER_SIZE;

    if ( !_buffer )
        _ReallocateBuffer( _bufferSize );

    size_t index = _dataSize;

    size_t endSize = index + size;

    if ( (MAX - index) < size )
        X_THROW(("Index(%ul)+size(%ul), exceed max allowed memory allocation(%ul)",index,size,MAX));

    if ( endSize > _bufferSize )
    {
        size_t newBufferSize = _bufferSize;

        while( newBufferSize < endSize )
        {
            if ( newBufferSize >= HALF_MAX )
            {
                newBufferSize = MAX;
                break;
            }
            else newBufferSize *= 2;
        }

        _ResizeBuffer( newBufferSize );
    }

    if ( endSize > _dataSize )
        _dataSize = endSize;

    return _buffer[index];
}

void XMemory::Clear()
{
    if ( _buffer )
    {
        X_DELETE_ALIGNED_BUFFER( _buffer );
        _buffer = NULL;
    }
    _dataSize = 0;
    _bufferSize = DEFAULT_BUFFER_SIZE;
}

void XMemory::ResizeData(size_t size)
{
    if(_bufferSize < size)
        _ReallocateBuffer(size);

    _dataSize = size;
}

XIRef<XMemory> XMemory::Clone() const
{
    XIRef<XMemory> obj = new XMemory;

    *obj = *this;

    return obj;
}

void XMemory::_ReallocateBuffer( size_t bufferSize )
{
    if( _buffer )
        X_DELETE_ALIGNED_BUFFER( _buffer );

    X_NEW_ALIGNED_BUFFER( _buffer, bufferSize, DEFAULT_ALIGNMENT );

    if( !_buffer )
        X_THROW(("Unable to allocate XMemory buffer."));

    _bufferSize = bufferSize;
    _dataSize = 0;
}

void XMemory::_ResizeBuffer( size_t bufferSize )
{
    uint8_t* buffer = NULL;
    X_NEW_ALIGNED_BUFFER( buffer, bufferSize, DEFAULT_ALIGNMENT );

    if( !buffer )
        X_THROW(("Unable to allocate XMemory buffer."));

    if ( bufferSize < _dataSize )
    {
        memcpy( buffer, _buffer, bufferSize );
        _dataSize = bufferSize;
    }
    else
        memcpy( buffer, _buffer, _dataSize );

    X_DELETE_ALIGNED_BUFFER( _buffer );
    _buffer = buffer;
    _bufferSize = bufferSize;
}

XMemoryPool::XMemoryPool( uint32_t bufferSize,
                          uint32_t initialNumBuffers ) :
    XBaseObject(),
    _freeBuffers(),
    _freeBuffersLock(),
    _bufferSize( bufferSize ),
    _initialNumBuffers( initialNumBuffers )
{
    for( uint32_t i = 0; i < _initialNumBuffers; i++ )
    {
        XIRef<XMemory> buffer = new XMemory( _bufferSize );

        _freeBuffers.push_back( buffer );
    }
}

XMemoryPool::~XMemoryPool()
{
}

XIRef<XMemory> XMemoryPool::GetBuffer()
{
    XGuard guard( _freeBuffersLock );

    XIRef<XMemory> buffer;

    if( !_freeBuffers.empty() )
    {
        buffer = _freeBuffers.front();
        _freeBuffers.pop_front();
    }
    else buffer = new XMemory( _bufferSize );

    return buffer;
}

void XMemoryPool::ReturnBuffer( XIRef<XMemory> buffer )
{
    XGuard guard( _freeBuffersLock );

    buffer->ResizeData( 0 );

    _freeBuffers.push_front( buffer );
}

uint32_t XMemoryPool::GetNumFreeBuffers()
{
    XGuard guard( _freeBuffersLock );

    return _freeBuffers.size();
}
