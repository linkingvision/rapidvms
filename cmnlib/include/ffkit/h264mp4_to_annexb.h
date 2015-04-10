
#ifndef __ffkit_h264mp4_to_annexb_h
#define __ffkit_h264mp4_to_annexb_h

#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace ffkit
{

class h264mp4_to_annexb
{
public:
    CK_API h264mp4_to_annexb( av_demuxer& deMuxer );

    CK_API virtual ~h264mp4_to_annexb() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void transform( std::shared_ptr<av_packet> input, bool keyFrame );

    CK_API std::shared_ptr<av_packet> get();

private:
    h264mp4_to_annexb( const h264mp4_to_annexb& obj );
    h264mp4_to_annexb& operator = ( const h264mp4_to_annexb& );

    void _free_filtered_packet();

    AVBitStreamFilterContext* _bsfc;
    AVCodecContext* _codec;
    AVPacket _filteredPacket;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
