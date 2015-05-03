
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XMEMORY_H_
#define _XSDK_XMEMORY_H_

#include "XSDK/XException.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XIRef.h"
#include "XSDK/XMutex.h"
#include <list>

namespace XSDK
{

const X_UUID kClsIdXMemory = {{0x03,0x52,0x52,0xe5,0x3f,0x1d,0x47,0x03,0xb1,0x8d,0xc8,0x41,0x78,0x8b,0x7a,0x04}};

/// @brief XMemory class is a resizable buffer used when retrieving body chunks in Webby.
class XMemory : public XBaseObject
{
public:
    static const size_t DEFAULT_BUFFER_SIZE = 1024;

    X_API explicit XMemory( size_t bufferSize = DEFAULT_BUFFER_SIZE );
    X_API XMemory( const XMemory& obj );
    X_API virtual ~XMemory() throw();

    X_API XMemory& operator = ( const XMemory& obj );

    // Declare IXUnknown
    XSDK_DECLARE_IXUNKNOWN(kClsIdXMemory);
    XSDK_QUERY_INTERFACE_BEGIN()
    XSDK_QUERY_INTERFACE_END(XSDK::XBaseObject)

    X_API inline const uint8_t& Index( size_t index ) const { return _buffer[index]; }
    X_API inline uint8_t& Index( size_t index ) { return _buffer[index]; }

    /// Size of bytes needed to write to and returns a reference to the first byte.
    X_API uint8_t& Extend( size_t size );

    /// Get pointer to buffer
    X_API inline uint8_t* Map() { return _buffer; }
    X_API inline const uint8_t* Map() const { return _buffer; }

    X_API inline uint8_t* begin() { return _buffer; }
    X_API inline const uint8_t* begin() const { return _buffer; }
    X_API inline uint8_t* end() { return _buffer + _dataSize; }
    X_API inline const uint8_t* end() const { return _buffer + _dataSize; }

    /// Get the current size of the internal buffer.
    X_API inline size_t GetBufferSize() const { return _bufferSize; }
    X_API inline size_t GetDataSize() const { return _dataSize; }
    X_API void Clear();
    X_API void ResizeData(size_t size);

    X_API inline bool empty() const { return _dataSize == 0; }

    /// Reads the value from the given index using the given type.
    ///
    /// \param index The index of the value.
    /// \param outdex It's set to the index which is after the value.
    template<typename T>
    X_API T Read(size_t index, size_t* outdex = 0) const;

    /// Writes the given value at the given index.
    ///
    /// The number of bytes written to is sizeof(T).
    ///
    /// \return The index after the value.
    template<typename T>
    X_API size_t Write(size_t index, T value);

    /// Extends by sizeof(T) and writes the given value to those bytes.
    ///
    /// \return The index after the value.
    template<typename T>
    X_API size_t Append(T value);

    /// Create a copy of XMemory object
    X_API XIRef<XSDK::XMemory> Clone() const;

private:
    /// Resize the internal buffer based on the bufferSize given.
    void _ReallocateBuffer( size_t bufferSize );
    void _ResizeBuffer( size_t bufferSize );

    size_t _bufferSize;          ///< Size of internal buffer in bytes.
    size_t _dataSize;            ///< Size of data that has been written to the buffer.
    uint8_t* _buffer;      ///< Pointer to internal buffer space.
};

// The whole reason this class exists is to speed up your code.
//
// To that end, its behaviour is similar to the behaviour of "new": Unless
// we are out of memory, GetBuffer() will return a new buffer and you will
// NOT block (even if that means that this class had to create a new XMemory
// for you).
//
// Because of this, it makes no guarantees as the the "max" number of objects
// it may contain in its free list. It will have as many "free" buffers as you
// put back in it.
//
// It will only have as many buffers as you have need for simultaneous buffers.

class XMemoryPool : public XBaseObject
{
public:
    X_API XMemoryPool( uint32_t bufferSize,
                       uint32_t initialNumBuffers );

    X_API virtual ~XMemoryPool();

    X_API XIRef<XMemory> GetBuffer();

    X_API void ReturnBuffer( XIRef<XMemory> buffer );

    X_API uint32_t GetNumFreeBuffers();

private:
    XMemoryPool( const XMemoryPool& obj );
    XMemoryPool& operator = ( const XMemoryPool& obj );

    std::list<XIRef<XMemory> > _freeBuffers;
    XMutex _freeBuffersLock;
    uint32_t _bufferSize;
    uint32_t _initialNumBuffers;
};


template<typename T>
T XMemory::Read(size_t index, size_t* outdex) const
{
    const size_t next = index + sizeof(T);
    X_ASSERT(_dataSize >= next);

    if(outdex)
        *outdex = next;

    return *((T*)&_buffer[index]);
}

template<typename T>
size_t XMemory::Write(size_t index, T value)
{
    const size_t next = index + sizeof(T);
    X_ASSERT(_dataSize >= next);
    *((T*)&_buffer[index]) = value;
    return next;
}

template<typename T>
size_t XMemory::Append(T value)
{
    size_t index = _dataSize;
    Extend(sizeof(T));
    return Write(index, value);
}

}

#endif /* _XSDK_XMEMORY_H_ */
