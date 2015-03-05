
#ifndef __ffkit_av_packet_h
#define __ffkit_av_packet_h

#include "cppkit/ck_types.h"

class av_packet_test;

namespace ffkit
{

class av_packet
{
    friend class ::av_packet_test;

public:
    av_packet( size_t sz );
    av_packet( uint8_t* src, size_t sz, bool owning = true );
    av_packet( const av_packet& obj );
    av_packet( av_packet&& obj ) noexcept;
    virtual ~av_packet() noexcept;

    av_packet& operator = ( const av_packet& obj );
    av_packet& operator = ( av_packet&& obj ) noexcept;

    uint8_t* map() const;

    size_t get_buffer_size() const;

    void set_data_size( size_t sz );
    size_t get_data_size() const;

    void set_ts( uint32_t ts );
    uint32_t get_ts() const;

    void set_ts_freq( uint32_t freq );
    uint32_t get_ts_freq() const;

    void set_key( bool key );
    bool is_key() const;

private:
    void _clear() noexcept;

    // The actual size of the buffer we may or may not have allocated (includes padding).
    size_t _bufferSize;
    // The requested buffer size.
    size_t _requestedSize;
    // true if we own the memory of our buffer (and we therefore need to copy the data in), false otherwise.
    bool _owning;
    // a pointer to our buffer (that we may more may not have allocated).
    uint8_t* _buffer;
    // the amount of valid (set) memory in the buffer.
    size_t _dataSize;
    // the current timestamp
    uint32_t _ts;
    // how many _ts ticks are in one second.
    uint32_t _ticksInSecond;
    // whether this packet is a key frame
    bool _key;
};

}

#endif
