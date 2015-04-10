
#ifndef __ffkit_yuv420p_to_argb24_h
#define __ffkit_yuv420p_to_argb24_h

#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

namespace ffkit
{

class yuv420p_to_argb24
{
public:
    CK_API yuv420p_to_argb24();

    CK_API virtual ~yuv420p_to_argb24() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void transform( std::shared_ptr<av_packet> input, size_t width, size_t height );

    CK_API std::shared_ptr<av_packet> get();

private:
    yuv420p_to_argb24( const yuv420p_to_argb24& obj );
    yuv420p_to_argb24& operator = ( const yuv420p_to_argb24& );

    std::shared_ptr<av_packet> _rgb24;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
