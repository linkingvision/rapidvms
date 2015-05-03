
#ifndef __ffkit_av_packet_factory_h
#define __ffkit_av_packet_factory_h

#include "cppkit/ck_pool.h"
#include "cppkit/ck_exception.h"
#include "ffkit/av_packet.h"

namespace ffkit
{

class av_packet_factory
{
public:
    virtual std::shared_ptr<av_packet> get( size_t sz ) = 0;
};

class av_packet_factory_default : public av_packet_factory
{
public:
    virtual std::shared_ptr<av_packet> get( size_t sz ) { return std::make_shared<av_packet>( sz ); }
};

class av_packet_factory_preallocated : public av_packet_factory
{
public:
    av_packet_factory_preallocated( size_t numPackets, size_t packetSize ) :
        _pool( std::make_shared<cppkit::ck_pool<av_packet> >( numPackets, packetSize ) ),
        _packetSize( packetSize )
    {
    }

    virtual std::shared_ptr<av_packet> get( size_t sz )
    {
        if( sz > _packetSize )
            CK_THROW(("Requested packet size exceeds preallocated packet size."));

        if( _pool->empty() )
            CK_THROW(("Pool is empty."));

        return _pool->get();
    }

private:
    av_packet_factory_preallocated( const av_packet_factory_preallocated& );
    av_packet_factory_preallocated& operator = ( const av_packet_factory_preallocated& );

    std::shared_ptr<cppkit::ck_pool<av_packet> > _pool;
    size_t _packetSize;
};

}

#endif
