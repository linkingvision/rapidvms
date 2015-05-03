
#ifndef __ffkit_jpeg_encoder_h
#define __ffkit_jpeg_encoder_h

#include "ffkit/ffoptions.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace ffkit
{

const int JPEG_ENCODE_ATTEMPTS = 16;

class jpeg_encoder
{
public:
    CK_API jpeg_encoder( const struct codec_options& options, int encodeAttempts = JPEG_ENCODE_ATTEMPTS );

    CK_API virtual ~jpeg_encoder() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void encode_yuv420p( std::shared_ptr<av_packet> input );

    CK_API std::shared_ptr<av_packet> get();

    CK_API static void write_jpeg_file( const cppkit::ck_string& fileName, std::shared_ptr<av_packet> jpeg );

private:
    jpeg_encoder( const jpeg_encoder& obj );
    jpeg_encoder& operator = ( const jpeg_encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    int _encodeAttempts;
    std::shared_ptr<av_packet> _output;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
