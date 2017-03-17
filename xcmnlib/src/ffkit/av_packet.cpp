
#include "ffkit/av_packet.h"
#include "cppkit/ck_exception.h"

extern "C"
{
#include "libavutil/avutil.h"
}

using namespace ffkit;

static const size_t PADDING = 16;

av_packet::av_packet( size_t sz ) :
    _bufferSize( ((sz % PADDING)==0) ? sz : sz + (PADDING - (sz % PADDING)) ),
    _requestedSize( sz ),
    _owning( true ),
    _buffer( NULL ),
    _dataSize( 0 ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( false )
{
    _buffer = (uint8_t*)av_malloc( _bufferSize );
    if( !_buffer )
        CK_THROW(("Unable to allocate packet buffer."));
}

av_packet::av_packet( uint8_t* src, size_t sz, bool owning ) :
    _bufferSize( ((sz % PADDING)==0) ? sz : sz + (PADDING - (sz % PADDING)) ),
    _requestedSize( sz ),
    _owning( owning ),
    _buffer( src ),
    _dataSize( sz ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( false )
{
    if( _owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            CK_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, src, sz );
    }
    else
    {
        _bufferSize = sz;
    }
}

av_packet::av_packet( const av_packet& obj ) :
    _bufferSize( 0 ),
    _owning( false ),
    _buffer( NULL ),
    _dataSize( 0 ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( obj._key )
{
    _clear();

    _bufferSize = obj._bufferSize;
    _requestedSize = obj._requestedSize;
    _dataSize = obj._dataSize;
    _owning = obj._owning;

    if( obj._owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            CK_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, obj._buffer, _bufferSize );
    }
    else
    {
        _buffer = obj._buffer;
    }

    _ts = obj._ts;
    _ticksInSecond = obj._ticksInSecond;
}

av_packet::av_packet( av_packet&& obj ) noexcept :
    _bufferSize( std::move( obj._bufferSize ) ),
    _requestedSize( std::move( obj._requestedSize ) ),
    _owning( std::move( obj._owning ) ),
    _buffer( std::move( obj._buffer ) ),
    _dataSize( std::move( obj._dataSize ) ),
    _ts( std::move( obj._ts ) ),
    _ticksInSecond( std::move( obj._ticksInSecond ) ),
    _key( std::move( obj._key ) )
{
    obj._buffer = NULL;
    obj._bufferSize = 0;
    obj._dataSize = 0;
}

av_packet::~av_packet() noexcept
{
    _clear();
}

av_packet& av_packet::operator = ( const av_packet& obj )
{
    _clear();
    _bufferSize = obj._bufferSize;
    _requestedSize = obj._requestedSize;
    _dataSize = obj._dataSize;
    _owning = obj._owning;

    if( obj._owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            CK_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, obj._buffer, _bufferSize );
    }
    else
    {
        _buffer = obj._buffer;
    }

    _ts = obj._ts;
    _ticksInSecond = obj._ticksInSecond;
    _key = obj._key;

    return *this;
}

av_packet& av_packet::operator = ( av_packet&& obj ) noexcept
{
    _bufferSize = std::move( obj._bufferSize );
    _requestedSize = std::move( obj._requestedSize );
    _owning = std::move( obj._owning );
    _buffer = std::move( obj._buffer );
    _dataSize = std::move( obj._dataSize );
    _ts = std::move( obj._ts );
    _ticksInSecond = std::move( obj._ticksInSecond );
    _key = std::move( obj._key );

    obj._buffer = NULL;
    obj._bufferSize = 0;
    obj._dataSize = 0;

    return *this;
}

uint8_t* av_packet::map() const
{
    return _buffer;
}

size_t av_packet::get_buffer_size() const
{
    return _requestedSize;
}

void av_packet::set_data_size( size_t sz )
{
    if( sz > _requestedSize )
        CK_THROW(("Unable to set data size to amount greater than buffer size."));
    _dataSize = sz;
}

size_t av_packet::get_data_size() const
{
    return _dataSize;
}

void av_packet::set_ts( uint32_t ts )
{
    _ts = ts;
}

uint32_t av_packet::get_ts() const
{
    return _ts;
}

void av_packet::set_ts_freq( uint32_t freq )
{
    _ticksInSecond = freq;
}

uint32_t av_packet::get_ts_freq() const
{
    return _ticksInSecond;
}

void av_packet::set_key( bool key )
{
    _key = key;
}

bool av_packet::is_key() const
{
    return _key;
}

void av_packet::_clear() noexcept
{
    if( _owning && _buffer )
        av_free( _buffer );

    _buffer = NULL;
    _bufferSize = 0;
    _requestedSize = 0;
    _dataSize = 0;
    _ts = 0;
    _ticksInSecond = 90000;
}
